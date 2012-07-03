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

		typedef typename std::map< vertex_descriptor , size_t >   VertexIndexMap;

		// an undirected is need
		boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS > undirectedGraph ;

		std::map< vertex_descriptor, size_t > mapVertexIndex ;
		{
			vertex_iterator it, end;
			for ( boost::tie(it,end) = graph.vertices(); it != end; ++it ){
				mapVertexIndex.insert( std::make_pair( *it, mapVertexIndex.size() ) );
			}
		}

		boost::const_associative_property_map< VertexIndexMap >   pmVertexIndex( mapVertexIndex );
		boost::copy_graph(
			graph.graph(),
			undirectedGraph,
			vertex_index_map( pmVertexIndex )
		);

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
