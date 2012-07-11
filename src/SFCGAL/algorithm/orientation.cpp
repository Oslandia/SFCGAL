#include <SFCGAL/algorithm/orientation.h>

#include <SFCGAL/graph/GeometryGraph.h>
#include <SFCGAL/graph/GeometryGraphBuilder.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
void makeConsistentOrientation( TriangulatedSurface & g )
{
	using namespace graph ;

	typedef GeometryGraph::edge_descriptor edge_descriptor ;


	/*
	 * build a graph with the polygons of the PolyhedralSurface
	 */

	//the list of polygons
	std::vector< Triangle* >                         triangles ;
	//the list of shared polygons
	std::vector< std::vector< edge_descriptor > >    sharedTriangles ;

	GeometryGraph graph ;
	GeometryGraphBuilder graphBuilder( graph ) ;
	sharedTriangles = graphBuilder.addTriangulatedSurface( g );

	std::cout << "sharedTriangles.size() " << sharedTriangles.size() << std::endl;

}


}//algorithm
}//SFCGAL

