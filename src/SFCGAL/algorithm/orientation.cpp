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


}//algorithm
}//SFCGAL

