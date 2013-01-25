#ifndef _SFCGAL_GRAPH_ALGORITHM_ISCONNECTED_H_
#define _SFCGAL_GRAPH_ALGORITHM_ISCONNECTED_H_

#include <set>
#include <complex>

#include <SFCGAL/detail/ComplexComparator.h>

#include <SFCGAL/graph/GeometryGraph.h>

#include <boost/graph/copy.hpp>
#include <boost/graph/connected_components.hpp>

namespace SFCGAL {
namespace graph {
namespace algorithm {


	/**
	 * Test if a bidirectional graph is an half-edge (in order to validate orientation)
	 */
	template < typename V, typename E >
	bool isHalfEdge( const GeometryGraphT<V,E> & graph ){
		typedef typename GeometryGraphT<V,E>::vertex_descriptor vertex_descriptor ;
		typedef typename GeometryGraphT<V,E>::edge_descriptor   edge_descriptor ;
		typedef typename GeometryGraphT<V,E>::edge_iterator     edge_iterator ;

		/*
		 * try to insert all edges in a map, return false if an edge already exists (i.e. there are parallel edges)
		 */
		std::set< std::complex< vertex_descriptor >, detail::ComplexComparator > edges ;
		edge_iterator it,end ;
		for ( boost::tie(it,end) = graph.edges(); it != end; ++it ){
			std::complex< vertex_descriptor > cedge(
				graph.source(*it),
				graph.target(*it)
			);

			if ( edges.find(cedge) != edges.end() ){
				return false ;
			}else{
				edges.insert(cedge);
			}
 		}

		return true ;
	}


}//algorithm
}//graph
}//SFCGAL

#endif
