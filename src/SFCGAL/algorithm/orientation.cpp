#include <SFCGAL/algorithm/orientation.h>

#include <SFCGAL/algorithm/ConsistentOrientationBuilder.h>

#include <SFCGAL/graph/GeometryGraph.h>
#include <SFCGAL/graph/GeometryGraphBuilder.h>
#include <SFCGAL/graph/algorithm/isHalfEdge.h>

#include <SFCGAL/graph/algorithm/orientation.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
bool hasConsistentOrientation3D( const TriangulatedSurface & g )
{
	using namespace graph ;

	if ( g.isEmpty() )
		return true ;


	GeometryGraph graph ;
	GeometryGraphBuilder graphBuilder( graph ) ;
	graphBuilder.addTriangulatedSurface( g );
	return graph::algorithm::isHalfEdge( graph ) ;
}


///
///
///
bool hasConsistentOrientation3D( const PolyhedralSurface & g )
{
	using namespace graph ;

	if ( g.isEmpty() )
		return true ;

	GeometryGraph graph ;
	GeometryGraphBuilder graphBuilder( graph ) ;
	graphBuilder.addPolyhedralSurface( g );
	return graph::algorithm::isHalfEdge( graph ) ;
}


///
///
///
void makeConsistentOrientation3D( TriangulatedSurface & g )
{
	ConsistentOrientationBuilder builder ;
	builder.addTriangulatedSurface(g);
	g = builder.buildTriangulatedSurface() ;
}

///
///
///
bool isCounterClockWiseOriented( const LineString& ls )
{
	// Compute the 'z' part of the Newell's formula
	// and test against 0
	Kernel::FT z = 0 ;
	for ( size_t i = 0; i < ls.numPoints() - 1; ++i )
	{
		const Point& pi = ls.pointN(i);
		const Point& pj = ls.pointN( (i+1) % ls.numPoints() );
		z += ( pi.x() - pj.x() ) * ( pi.y() + pj.y() );
	}
	return z > 0;
}

///
///
///
bool isCounterClockWiseOriented( const Triangle& tri )
{
	// Compute the 'z' part of the cross product

	return (tri.vertex(2).x() - tri.vertex(1).x()) * (tri.vertex(0).y() - tri.vertex(1).y()) -
		(tri.vertex(2).y() - tri.vertex(1).y()) * (tri.vertex(0).x() - tri.vertex(1).x()) > 0;
}

///
///
///
bool isCounterClockWiseOriented( const Polygon& poly )
{
	return isCounterClockWiseOriented( poly.exteriorRing() );
}

}//algorithm
}//SFCGAL

