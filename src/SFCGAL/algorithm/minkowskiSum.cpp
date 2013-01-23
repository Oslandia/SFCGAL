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
	return std::auto_ptr< Geometry >( gB.clone() ) ;
}

///
///
///
std::auto_ptr< Geometry > minkowskiSum( const LineString& gA, const Polygon& gB )
{
	Polygon_2 Q = gB.toPolygon_2() ;

	Polygon_set_2 polygonSet ;

	// compute minkowski sum for each LineString (as a collapsed polygon), and join the results
	{
		int npt = gA.numPoints() ;
		for ( int i = 0; i < npt - 1 ; i++ ){
			Polygon_2 P;
			P.push_back( gA.pointN(i).toPoint_2() );
			P.push_back( gA.pointN(i+1).toPoint_2() );

			Polygon_with_holes_2 part = minkowski_sum_2( P, Q );
			// if linestring has only one segment
			if ( npt == 2 ){
				return std::auto_ptr< Geometry >(
					new Polygon( part )
				);
			}

			// merge into a polygon set
			if ( i == 0 ){
				polygonSet.insert( part );
			}else{
				polygonSet.join( part );
			}
		}
	}

	std::auto_ptr< MultiPolygon > result( detail::polygonSetToMultiPolygon( polygonSet ) );
	return std::auto_ptr< Geometry >( result.release() );
}


///
///
///
std::auto_ptr< Geometry > minkowskiSum( const Polygon& gA, const Polygon& gB )
{
	Polygon_with_holes_2 sum = minkowski_sum_2( gA.toPolygon_2(), gB.toPolygon_2() );
	return std::auto_ptr< Geometry >( new Polygon( sum ) );
}



} // namespace algorithm
} // namespace SFCGAL
