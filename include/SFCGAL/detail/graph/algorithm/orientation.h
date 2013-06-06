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
#ifndef _SFCGAL_GRAPH_ALGORITHM_MAKECONSISTENTORIENTATION_H_
#define _SFCGAL_GRAPH_ALGORITHM_MAKECONSISTENTORIENTATION_H_

#include <map>
#include <SFCGAL/detail/graph/GeometryGraph.h>

namespace SFCGAL {
namespace graph {
namespace algorithm {

	/**
	 * @brief [private]Study orientation between two EdgeStrings
	 * @return true on success
	 */
	template < typename Graph >
	void studyOrientation(
		Graph & graph,
		std::vector< typename Graph::edge_descriptor > & reference,
		std::vector< typename Graph::edge_descriptor > & target,
		bool & hasOppositeEdge,
		bool & hasParallelEdge
	){
		/*
		 * look for opposite or parallel edges in reference and target
		 */
		hasOppositeEdge = false ;
		hasParallelEdge = false ;
		for ( size_t i = 0; i < reference.size(); i++ ){
			for ( size_t j = 0; j < target.size(); j++ ){
				if ( graph.areOpposite( reference[i], target[j] ) )
					hasOppositeEdge = true ;
				if ( graph.areParallel( reference[i], target[j] ) )
					hasParallelEdge = true ;
			}
		}
	}


	/**
	 * Try to build consistent orientation between two edge string
	 * @return true on success
	 */
	template < typename Graph >
	bool makeConsistentOrientation(
		Graph & graph,
		std::vector< typename Graph::edge_descriptor > & reference,
		std::vector< typename Graph::edge_descriptor > & target
	){
		/*
		 * look for opposite or parallel edges in "reference" and "target" edge sets
		 */
		bool hasOppositeEdge, hasParallelEdge ;
		studyOrientation(graph,reference,target,hasOppositeEdge,hasParallelEdge);

		/*
		 * if both opposite and parallel edge are found, there is no possible
		 * consistent orientation
		 */
		if ( hasOppositeEdge && hasParallelEdge ){
			BOOST_THROW_EXCEPTION(Exception(
				"can't make consistent orientation between EdgeStrings (both opposite and parallel edge found)"
			));
		}
		/*
		 * Only parallel edge found, lets revert the orientation of the "target" edge set
		 */
		if ( hasParallelEdge ){
			graph.reverse( target );
			return true ;
		}else{
			return false ;
		}
	}


}//algorithm
}//graph
}//SFCGAL

#endif
