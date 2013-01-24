#include <SFCGAL/algorithm/minkowskiSum.h>
#include <SFCGAL/all.h>

#include <SFCGAL/detail/polygonSetToMultiPolygon.h>

#include <CGAL/minkowski_sum_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>

#include <CGAL/Aff_transformation_2.h>


typedef CGAL::Polygon_2< SFCGAL::Kernel >            Polygon_2 ;
typedef CGAL::Polygon_with_holes_2< SFCGAL::Kernel > Polygon_with_holes_2 ;
typedef CGAL::Polygon_set_2< SFCGAL::Kernel >        Polygon_set_2 ;


namespace SFCGAL {
namespace algorithm {

///
///
///
std::auto_ptr< MultiPolygon > minkowskiSum( const Geometry& gA, const Polygon& gB )
{
	switch ( gA.geometryTypeId() ){
	case TYPE_POINT:
		return minkowskiSum( gA.as< Point >(), gB ) ;
	case TYPE_LINESTRING:
		return minkowskiSum( gA.as< LineString >(), gB ) ;
	case TYPE_POLYGON:
		return minkowskiSum( gA.as< Polygon >(), gB ) ;
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_MULTIPOLYGON:
	case TYPE_GEOMETRYCOLLECTION:
		return minkowskiSum( gA.as< GeometryCollection >(), gB ) ;
	}
	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("minkowskiSum( %s, %s ) is not defined")
			% gA.geometryType()
			% gB.geometryType() ).str()
	));
}


//-- support


/*
 * append gA+gB into the polygonSet
 */
void minkowskiSum( const Point& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ){

	CGAL::Aff_transformation_2< Kernel > translate(
		CGAL::TRANSLATION,
		gA.toVector_2()
	);

	Polygon_2 sum ;
	for ( Polygon_2::Vertex_const_iterator it = gB.vertices_begin();
			it != gB.vertices_end(); ++it ){
		sum.push_back( translate.transform( *it ) );
	}

	if ( sum.is_clockwise_oriented() ){
		sum.reverse_orientation() ;
	}

	if ( polygonSet.is_empty() ){
		polygonSet.insert( sum );
	}else{
		polygonSet.join( sum );
	}
}




/*
 * append gA+gB into the polygonSet
 */
void minkowskiSum( const LineString& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ){
	int npt = gA.numPoints() ;
	for ( int i = 0; i < npt - 1 ; i++ ){
		Polygon_2 P;
		P.push_back( gA.pointN(i).toPoint_2() );
		P.push_back( gA.pointN(i+1).toPoint_2() );

		Polygon_with_holes_2 part = minkowski_sum_2( P, gB );

		// merge into a polygon set
		if ( polygonSet.is_empty() ){
			polygonSet.insert( part );
		}else{
			polygonSet.join( part );
		}
	}
}

/*
 * append gA+gB into the polygonSet
 */
void minkowskiSum( const Polygon& gA, const Polygon_2 & gB, Polygon_set_2 & polygonSet ){
	/*
	 * Invoke minkowski_sum_2 for exterior ring
	 */
	{
		Polygon_with_holes_2 sum = minkowski_sum_2( gA.exteriorRing().toPolygon_2(), gB ) ;
		if ( polygonSet.is_empty() ){
			polygonSet.insert( sum );
		}else{
			polygonSet.join( sum );
		}
	}

	/*
	 * Compute the minkowski sum for each segment of the interior rings
	 * and perform the union of the result.
	 *
	 * Interior rings will corresponds
	 */
	if ( gA.hasInteriorRings() ){
		Polygon_set_2 sumInteriorRings ;
		for ( size_t i = 0; i < gA.numInteriorRings(); i++ ){
			minkowskiSum( gA.interiorRingN(i), gB, sumInteriorRings ) ;
		}

		/*
		 * compute the difference for each holes of the resulting polygons
		 */
		std::list<Polygon_with_holes_2> interiorPolygons ;
		sumInteriorRings.polygons_with_holes( std::back_inserter(interiorPolygons) ) ;
		for ( std::list<Polygon_with_holes_2>::iterator it_p = interiorPolygons.begin();
				it_p != interiorPolygons.end(); ++it_p ){

			for ( Polygon_with_holes_2::Hole_iterator it_hole = it_p->holes_begin();
					it_hole != it_p->holes_end(); ++it_hole ){

				it_hole->reverse_orientation() ;
				polygonSet.difference( *it_hole ) ;
			} // foreach hole
		}// foreach polygon
	}
}



//-- implementation

///
///
///
std::auto_ptr< MultiPolygon > minkowskiSum( const Point& gA, const Polygon& gB )
{
	if ( gA.isEmpty() ){
		return std::auto_ptr< MultiPolygon >( new MultiPolygon ) ;
	}

	Polygon_set_2 polygonSet ;
	minkowskiSum( gA, gB.toPolygon_2(), polygonSet );

	return detail::polygonSetToMultiPolygon( polygonSet ) ;
}

///
///
///
std::auto_ptr< MultiPolygon > minkowskiSum( const LineString& gA, const Polygon& gB )
{
	if ( gA.isEmpty() ){
		return std::auto_ptr< MultiPolygon >( new MultiPolygon ) ;
	}

	Polygon_2 Q = gB.toPolygon_2() ;

	// compute minkowski sum for each LineString (as a collapsed polygon), and join the results
	Polygon_set_2 polygonSet ;
	{
		minkowskiSum( gA, Q, polygonSet );
	}
	return detail::polygonSetToMultiPolygon( polygonSet ) ;
}

///
///
///
std::auto_ptr< MultiPolygon > minkowskiSum( const Polygon& gA, const Polygon& gB )
{
	if ( gA.isEmpty() ){
		return std::auto_ptr< MultiPolygon >( new MultiPolygon ) ;
	}

	Polygon_2 Q = gB.toPolygon_2() ;
	Polygon_set_2 polygonSet ;

	minkowskiSum( gA, Q, polygonSet ) ;

	/*
	 * convert the result into a SFCGAL::MultiPolygon
	 */
	return detail::polygonSetToMultiPolygon( polygonSet ) ;
}



///
///
///
std::auto_ptr< MultiPolygon > minkowskiSum( const GeometryCollection& gA, const Polygon& gB )
{
	if ( gA.isEmpty() ){
		return std::auto_ptr< MultiPolygon >( new MultiPolygon ) ;
	}

	Polygon_2 Q = gB.toPolygon_2() ;

	Polygon_set_2 polygonSet ;
	for ( size_t i = 0; i < gA.numGeometries(); i++ ){
		const Geometry & part = gA.geometryN(i) ;
		switch ( part.geometryTypeId() ){
		case TYPE_POINT:
			minkowskiSum( part.as< Point >(), Q, polygonSet ) ;
			break ;
		case TYPE_LINESTRING:
			minkowskiSum( part.as< LineString >(), Q, polygonSet ) ;
			break ;
		case TYPE_POLYGON:
			minkowskiSum( part.as< Polygon >(), Q, polygonSet ) ;
			break ;
		default:
			BOOST_THROW_EXCEPTION( Exception(
				( boost::format("unsupported geometry type (%s) in GeometryCollection for minkowskiSum") % part.geometryType() ).str()
			) ) ;
			break;
		}
	}

	/*
	 * convert the result into a SFCGAL::MultiPolygon
	 */
	return detail::polygonSetToMultiPolygon( polygonSet ) ;
}


} // namespace algorithm
} // namespace SFCGAL
