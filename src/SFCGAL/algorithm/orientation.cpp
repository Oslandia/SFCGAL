#include <SFCGAL/algorithm/orientation.h>

#include <SFCGAL/graph/GeometryGraph.h>
#include <SFCGAL/graph/GeometryGraphBuilder.h>
#include <SFCGAL/graph/algorithm/isHalfEdge.h>

#include <SFCGAL/graph/algorithm/makeConsistentOrientation.h>



typedef CGAL::Cartesian< double >     Kernel ;
typedef CGAL::Point_3< Kernel >       Point_3 ;
typedef CGAL::Polyhedron_3< Kernel >  Polyhedron_3 ;

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


/*
 * get the indices of the neighbors to each triangle in a TriangulatedSurface
 */
std::vector< std::set< size_t > > getTriangleNeighbors( TriangulatedSurface & g )
{
	using namespace graph ;

	std::vector< std::set< size_t > > result( g.numTriangles() ) ;

	/*
	 * build a graph with the triangles where
	 */
	typedef GeometryGraphT< Vertex, size_t >  graph_t ;
	typedef graph_t::vertex_descriptor        vertex_descriptor ;
	typedef graph_t::edge_descriptor          edge_descriptor ;
	typedef graph_t::directed_edge_descriptor directed_edge_descriptor ;

	graph_t graph ;
	GeometryGraphBuilderT< graph_t > graphBuilder( graph ) ;
	std::vector< std::vector< edge_descriptor > > triangles( g.numTriangles() );
	for ( size_t i = 0; i < g.numTriangles(); i++ ){
		triangles[i] = graphBuilder.addTriangle( g.triangleN(i), i ) ;
	}

	/*
	 * get neighbor for each triangle
	 */
	for ( size_t i = 0; i < triangles.size(); i++ ){
		const std::vector< edge_descriptor > & triangle = triangles[i] ;
		for ( size_t j = 0; j < triangle.size(); j++ ){
			vertex_descriptor source = graph.source( triangle[j] ) ;
			vertex_descriptor target = graph.target( triangle[j] ) ;

			//get neighbor edges
			std::vector< directed_edge_descriptor > neighborEdges = graph.edges( source, target );
			//use marker to fill neighborGraph
			for ( size_t k = 0; k < neighborEdges.size(); k++ ){
				size_t idOtherTriangle = graph[ neighborEdges[k].first ];
				if ( idOtherTriangle == i )
					continue ;

				result[i].insert( idOtherTriangle );
			}
		}
	}

	return result ;
}


///
///
///
void makeConsistentOrientation3D( TriangulatedSurface & g )
{
	using namespace graph ;

	if ( g.isEmpty() )
		return ;

	//fill an HalfEdge structure with triangles
	typedef GeometryGraphT< Vertex, size_t >  graph_t ;
	typedef graph_t::vertex_descriptor        vertex_descriptor ;
	typedef graph_t::edge_descriptor          edge_descriptor ;
	typedef graph_t::directed_edge_descriptor directed_edge_descriptor ;

	/*
	 * build a graph with the Triangles
	 */
	graph_t graph ;
	GeometryGraphBuilderT< graph_t > graphBuilder( graph ) ;
	std::vector< std::vector< edge_descriptor > > triangles = graphBuilder.addTriangulatedSurface( g );

	std::vector< std::set< size_t > > neigbors = getTriangleNeighbors( g ) ;

	/*
	 * Parcours de graphe : Propagation d'une orientation de reference au triangle voisin
	 *
	 * Chaque triangle est (atteint/non atteint, traite/non traite = a deja servi de reference).
	 *
	 * -- on marque comment atteint le premier triangle (initialialisation)
	 *
	 * -- on recupere comme triangle courant :
	 * ---- un triangle atteint, non traite
	 * ---- un triangle non traite si les atteints sont tous traites (cas non connexe)
	 *
	 * - recuperer les triangles voisins
	 * -- Si triangle voisin deja atteint (deja en coherence avec une autre face), alors on ne peut pas l'inverser
	 *  (cas bande moebius, non manifold)
	 * -- Sinon, on essaie de le mettre en coherence avec le triangle courant (graph::algorithm::makeConsistentOrientation)
	 * -- on marque le triangle comme atteint
	 *
	 *
	 * -- on boucle tant qu'il reste des triangles a traiter
	 */
}


}//algorithm
}//SFCGAL

