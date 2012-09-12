#ifndef _SFCGAL_GRAPH_ALGORITHM_ISCONNECTED_H_
#define _SFCGAL_GRAPH_ALGORITHM_ISCONNECTED_H_

#include <map>
#include <SFCGAL/graph/GeometryGraph.h>

#include <boost/graph/copy.hpp>
#include <boost/graph/connected_components.hpp>

namespace SFCGAL {
namespace graph {
namespace algorithm {

	/**
	 * Test if a bidirectional graph is connected (all node reachable using direct or reverse edges)
	 */
	template < typename V, typename E >
	bool isConnected( const GeometryGraphT<V,E> & graph ){
		BOOST_ASSERT( graph.numVertices() != 0 );

		typedef typename GeometryGraphT<V,E>::vertex_descriptor vertex_descriptor ;
		typedef typename GeometryGraphT<V,E>::vertex_iterator   vertex_iterator ;

		typedef typename GeometryGraphT<V,E>::edge_descriptor edge_descriptor ;
		typedef typename GeometryGraphT<V,E>::edge_iterator   edge_iterator ;

		typedef typename std::map< vertex_descriptor , size_t >   VertexIndexMap;

		//assign a number to each vertex
		std::map< vertex_descriptor, size_t > mapVertexIndex ;
		{
			vertex_iterator it, end;
			for ( boost::tie(it,end) = graph.vertices(); it != end; ++it ){
				mapVertexIndex.insert( std::make_pair( *it, mapVertexIndex.size() ) );
			}
		}

		// an undirected is need
		boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, size_t > undirectedGraph( mapVertexIndex.size() ) ;
		// copy edges
		{
			edge_iterator it,end;
			for ( boost::tie(it,end) = graph.edges(); it != end; ++it ){
				boost::add_edge(
					mapVertexIndex[ graph.source( *it ) ],
					mapVertexIndex[ graph.target( *it ) ],
					undirectedGraph
				);
			}
		}

		BOOST_ASSERT( graph.numVertices() == boost::num_vertices( undirectedGraph ) );
		BOOST_ASSERT( graph.numEdges()    == boost::num_edges( undirectedGraph ) );

		std::vector< int > component( graph.numVertices() );
		int numComponents = boost::connected_components( undirectedGraph, &component[0] );

		return numComponents == 1 ;
	}


}//algorithm
}//graph
}//SFCGAL

#endif
