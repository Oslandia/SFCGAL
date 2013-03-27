/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
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
