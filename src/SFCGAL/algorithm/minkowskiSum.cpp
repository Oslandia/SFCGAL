#include <SFCGAL/algorithm/minkowskiSum.h>
#include <SFCGAL/all.h>

#include <SFCGAL/detail/polygonSetToMultiPolygon.h>

#include <CGAL/minkowski_sum_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>


typedef CGAL::Polygon_2< SFCGAL::Kernel >            Polygon_2 ;
typedef CGAL::Polygon_with_holes_2< SFCGAL::Kernel > Polygon_with_holes_2 ;
typedef CGAL::Polygon_set_2< SFCGAL::Kernel >        Polygon_set_2 ;


namespace SFCGAL {
namespace algorithm {

///
///
///
std::auto_ptr< Geometry > minkowskiSum( const Geometry& gA, const Polygon& gB )
{
	switch ( gA.geometryTypeId() ){
	case TYPE_POINT:
		return minkowskiSum( gA.as< Point >(), gB ) ;
	case TYPE_LINESTRING:
		return minkowskiSum( gA.as< LineString >(), gB ) ;
	case TYPE_POLYGON:
		return minkowskiSum( gA.as< Polygon >(), gB ) ;
	}
	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("minkowskiSum( %s, %s ) is not defined")
			% gA.geometryType()
			% gB.geometryType() ).str()
	));
}

///
///
///
std::auto_ptr< Geometry > minkowskiSum( const Point& gA, const Polygon& gB )
{
	if ( gA.isEmpty() ){
		return std::auto_ptr< Geometry >( new GeometryCollection ) ;
	}
	return std::auto_ptr< Geometry >( gB.clone() ) ;
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

///
///
///
std::auto_ptr< Geometry > minkowskiSum( const LineString& gA, const Polygon& gB )
{
	if ( gA.isEmpty() ){
		return std::auto_ptr< Geometry >( new GeometryCollection ) ;
	}

	Polygon_2 Q = gB.toPolygon_2() ;

	// compute minkowski sum for each LineString (as a collapsed polygon), and join the results
	Polygon_set_2 polygonSet ;
	{
		minkowskiSum( gA, Q, polygonSet );
	}
	std::auto_ptr< MultiPolygon > result( detail::polygonSetToMultiPolygon( polygonSet ) );
	return std::auto_ptr< Geometry >( result.release() );
}

///
///
///
std::auto_ptr< Geometry > minkowskiSum( const Polygon& gA, const Polygon& gB )
{
	if ( gA.isEmpty() ){
		return std::auto_ptr< Geometry >( new GeometryCollection ) ;
	}

	Polygon_2 Q = gB.toPolygon_2() ;

	/*
	 * Basic invocation if the polygon has no holes
	 */
	if ( ! gA.hasInteriorRings() ){
		Polygon_with_holes_2 sum = minkowski_sum_2( gA.toPolygon_2(), Q );
		return std::auto_ptr< Geometry >( new Polygon( sum ) );
	}

	/*
	 * Invoke minkowski_sum_2 for exterior ring
	 */
	Polygon_set_2 polygonSet ;
	polygonSet.insert( minkowski_sum_2( gA.exteriorRing().toPolygon_2(), Q ) );

	/*
	 * Compute the minkowski sum for each segment of the interior rings
	 * and perform the union of the result.
	 *
	 * Interior rings will corresponds
	 */
	if ( gA.hasInteriorRings() ){
		Polygon_set_2 sumInteriorRings ;
		for ( size_t i = 0; i < gA.numInteriorRings(); i++ ){
			minkowskiSum( gA.interiorRingN(i), Q, sumInteriorRings ) ;
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

	/*
	 * convert the result into a SFCGAL::MultiPolygon
	 */
	std::auto_ptr< MultiPolygon > result( detail::polygonSetToMultiPolygon( polygonSet ) );
	return std::auto_ptr< Geometry >( result.release() );
}



} // namespace algorithm
} // namespace SFCGAL
