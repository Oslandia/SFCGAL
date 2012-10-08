#include <SFCGAL/algorithm/distance.h>
#include <SFCGAL/all.h>
#include <SFCGAL/Exception.h>

#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <SFCGAL/transform/AffineTransform3.h>
#include <SFCGAL/algorithm/intersects.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel ;
typedef Kernel::Point_2                                   Point_2 ;
typedef Kernel::Vector_2                                  Vector_2 ;
typedef Kernel::Segment_2                                 Segment_2 ;

typedef CGAL::Polygon_2< Kernel >                         Polygon_2 ;
typedef CGAL::Polygon_with_holes_2< Kernel >              Polygon_with_holes_2 ;


namespace SFCGAL {
namespace algorithm {

///
///
///
double distancePointPoint( const Point & a, const Point & b )
{
	BOOST_ASSERT( ! a.isEmpty() );
	BOOST_ASSERT( ! b.isEmpty() );

	return CGAL::sqrt(
		CGAL::to_double(
			CGAL::squared_distance( a.toPoint_2< Kernel >(), b.toPoint_2< Kernel >() )
		)
	);
}

///
///
///
double distancePointSegment( const Point & p, const Point & a, const Point & b )
{
	BOOST_ASSERT( ! p.isEmpty() );
	BOOST_ASSERT( ! a.isEmpty() );
	BOOST_ASSERT( ! b.isEmpty() );

	return CGAL::sqrt(
		CGAL::to_double(
			CGAL::squared_distance(
				p.toPoint_2< Kernel >(),
				Segment_2(
					a.toPoint_2< Kernel >(),
					b.toPoint_2< Kernel >()
				)
			)
		)
	);
}

///
///
///
double distancePointLineString( const Point & point, const LineString& lineString )
{
	BOOST_ASSERT( lineString.numPoints() >= 2 );
	size_t numSegments = lineString.numPoints() - 1 ;

	double dMin = SFCGAL::NaN() ;
	for ( size_t i = 0; i < numSegments; i++ ){
		double d = distancePointSegment( point, lineString.pointN(i), lineString.pointN(i+1) );
		if ( i == 0 || d < dMin ){
			dMin = d ;
		}
	}
	return dMin ;
}


///
///
///
double distancePointPolygon( const Point & point, const Polygon& polygon )
{
	if ( intersects(point,polygon) ){
		return 0.0 ;
	}

	double dMin = 0.0 ;
	//check if the point is in the polygon
	for ( size_t i = 0; i < polygon.numRings(); i++ ){
		double d = distancePointLineString( point, polygon.ringN(i) );
		if ( i == 0 || d < dMin ){
			dMin = d ;
		}
	}
	return dMin ;
}

///
///
///
double distanceSegmentSegment(
	const Point & a, const Point & b,
	const Point & c, const Point & d
)
{
	return CGAL::sqrt( CGAL::to_double(
		CGAL::squared_distance(
			CGAL::Segment_2< Kernel >( a.toPoint_2< Kernel >(), b.toPoint_2< Kernel >() ),
			CGAL::Segment_2< Kernel >( c.toPoint_2< Kernel >(), d.toPoint_2< Kernel >() )
		)
	) );
}

///
///
///
double distanceLineStringLineString(
	const LineString & gA,
	const LineString & gB
)
{
	BOOST_ASSERT( gA.numPoints() >= 2 );
	BOOST_ASSERT( gB.numPoints() >= 2 );
	size_t nsA = gA.numPoints() - 1 ;
	size_t nsB = gB.numPoints() - 1 ;

	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < nsA; i++ ){
		for ( size_t j = 0; j < nsB; j++ ){
			double d = distanceSegmentSegment(
				gA.pointN(i), gA.pointN(i+1),
				gB.pointN(j), gB.pointN(j+1)
			);
			if ( d < dMin )
				dMin = d ;
		}
	}
	return dMin ;
}

///
///
///
double distanceLineStringPolygon(
	const LineString & gA,
	const Polygon & gB
)
{
	if ( intersects(gA, gB) ){
		return 0.0 ;
	}
	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gB.numRings(); i++ ){
		double d = distanceLineStringLineString( gA, gB.ringN(i) );
		if ( d < dMin )
			dMin = d ;
	}
	return dMin ;
}


///
///
///
double distancePolygonPolygon(
	const Polygon & gA,
	const Polygon & gB
)
{
	if ( intersects(gA, gB) ){
		return 0.0 ;
	}
	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gA.numRings(); i++ ){
		for ( size_t j = 0; j < gB.numRings(); j++ ){
			double d = distanceLineStringLineString( gA.ringN(i), gB.ringN(j) );
			if ( d < dMin )
				dMin = d ;
		}
	}
	return dMin ;
}


}//namespace algorithm
}//namespace SFCGAL

