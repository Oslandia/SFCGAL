/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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
