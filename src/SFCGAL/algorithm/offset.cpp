#include <SFCGAL/algorithm/offset.h>
#include <SFCGAL/all.h>
#include <SFCGAL/detail/polygonSetToMultiPolygon.h>


#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>

#include <CGAL/minkowski_sum_2.h>
#include <CGAL/approximated_offset_2.h>
#include <CGAL/offset_polygon_2.h>

typedef CGAL::Polygon_2< SFCGAL::Kernel >            Polygon_2 ;
typedef CGAL::Polygon_with_holes_2< SFCGAL::Kernel > Polygon_with_holes_2 ;
typedef CGAL::Polygon_set_2< SFCGAL::Kernel >        Polygon_set_2 ;

typedef CGAL::Gps_circle_segment_traits_2< SFCGAL::Kernel >    Gps_traits_2;
typedef Gps_traits_2::Polygon_2                                Offset_polygon_2;
typedef Gps_traits_2::Polygon_with_holes_2                     Offset_polygon_with_holes_2;
typedef CGAL::General_polygon_set_2< Gps_traits_2 >            Offset_polygon_set_2 ;

#define SFCGAL_OFFSET_ACCURACY 0.0001

namespace SFCGAL {
namespace algorithm {


/**
 * @brief approximate an Offset_polygon_2 (filter null segments)
 */
Polygon_2 approximate( const Offset_polygon_2 & polygon, const int & n = 0 ){
	std::list<std::pair<double, double> > pair_list;

	/*
	 * iterate X_monotone_curve_2 components
	 */
	for ( Offset_polygon_2::Curve_const_iterator it = polygon.curves_begin();
			it != polygon.curves_end(); ++it ){
		it->approximate(std::back_inserter(pair_list), n ) ;
	}

	// remove duplicated last point
	pair_list.pop_back() ;

	/*
	 * convertr to polygon
	 */
	Polygon_2 result ;

	bool isFirst = true ;
	Kernel::Point_2 last ;
	for ( std::list<std::pair<double, double> >::const_iterator it = pair_list.begin(); it != pair_list.end(); ++it ){
		Kernel::Point_2 point( it->first, it->second ) ;
		if ( isFirst ){
			isFirst = false ;
		}else if ( point == last ){
			continue ;
		}
		result.push_back( point ) ;
		last = point ;
	}
	return result ;
}

/**
 * @brief approximate an Offset
 */
Polygon_with_holes_2 approximate( const Offset_polygon_with_holes_2 & polygon, const int & n = 0 ){
	Polygon_with_holes_2 result( approximate( polygon.outer_boundary(), n ) );
	for ( Offset_polygon_with_holes_2::Hole_const_iterator it = polygon.holes_begin(); it != polygon.holes_end(); ++it ){
		result.add_hole( approximate( *it, n ) );
	}
	return result ;
}


/**
 * @brief convert Offset_polygon_set_2 to MultiPolygon
 */
std::auto_ptr< MultiPolygon > polygonSetToMultiPolygon( const Offset_polygon_set_2 & polygonSet, const int & n )
{
	std::list<Offset_polygon_with_holes_2> res;
	polygonSet.polygons_with_holes( std::back_inserter(res) ) ;

	std::auto_ptr< MultiPolygon > result( new MultiPolygon );
	for ( std::list<Offset_polygon_with_holes_2>::const_iterator it = res.begin(); it != res.end(); ++it ){
		result->addGeometry( new Polygon( approximate( *it, n ) ) );
	}
	return result ;
}




/**
 * @brief build LineString offset
 */
void offset( const LineString & lineString, const double& radius, Offset_polygon_set_2 & polygonSet ){
	for ( size_t i = 0; i < lineString.numSegments(); i++ ){
		Polygon_2 P ;
		P.push_back( lineString.pointN(i).toPoint_2() );
		P.push_back( lineString.pointN(i+1).toPoint_2() );
		Offset_polygon_with_holes_2  offset = CGAL::approximated_offset_2( P, radius, SFCGAL_OFFSET_ACCURACY ) ;

		if ( polygonSet.is_empty() ){
			polygonSet.insert( offset );
		}else{
			polygonSet.join( offset );
		}
	}
}


///
///
///
void offset( const Polygon & gA, const double& radius, Offset_polygon_set_2 & polygonSet ){
	if ( gA.isEmpty() ){
		return ;
	}

	/*
	 * Invoke minkowski_sum_2 for exterior ring
	 */
	{
		Offset_polygon_with_holes_2  offset = CGAL::approximated_offset_2( gA.exteriorRing().toPolygon_2(), radius, SFCGAL_OFFSET_ACCURACY ) ;
		if ( polygonSet.is_empty() ){
			polygonSet.insert( offset );
		}else{
			polygonSet.join( offset );
		}
	}

	/*
	 * Compute the Minkowski sum for each segment of the interior rings
	 * and perform the union of the result. The result is a polygon, and its holes
	 * correspond to the inset.
	 *
	 */
	if ( gA.hasInteriorRings() ){
		Offset_polygon_set_2 sumInteriorRings ;
		for ( size_t i = 0; i < gA.numInteriorRings(); i++ ){
			offset( gA.interiorRingN(i), radius, sumInteriorRings ) ;
		}

		/*
		 * compute the difference for each hole of the resulting polygons
		 */
		std::list< Offset_polygon_with_holes_2 > interiorPolygons ;
		sumInteriorRings.polygons_with_holes( std::back_inserter(interiorPolygons) ) ;
		for ( std::list< Offset_polygon_with_holes_2 >::iterator it_p = interiorPolygons.begin();
				it_p != interiorPolygons.end(); ++it_p ){

			for ( Offset_polygon_with_holes_2::Hole_iterator it_hole = it_p->holes_begin();
					it_hole != it_p->holes_end(); ++it_hole ){

				it_hole->reverse_orientation() ;
				polygonSet.difference( *it_hole ) ;
			} // foreach hole
		}// foreach polygon
	}
}



///
///
///
void offset( const Geometry & g, const double & r, Offset_polygon_set_2 & polygonSet )
{
	switch ( g.geometryTypeId() ){
	case TYPE_LINESTRING:
		return offset( g.as< LineString >(), r, polygonSet ) ;
	case TYPE_POLYGON:
		return offset( g.as< Polygon >(), r, polygonSet ) ;
	}
}

//-- public interface

///
///
///
std::auto_ptr< MultiPolygon > offset( const Geometry & g, const double & r )
{
	Offset_polygon_set_2 polygonSet ;
	offset( g, r, polygonSet ) ;
	return polygonSetToMultiPolygon( polygonSet, 8 );
}



}//namespace algorithm
}//namespace SFCGAL

