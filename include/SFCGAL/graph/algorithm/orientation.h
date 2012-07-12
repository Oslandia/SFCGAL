#ifndef _SFCGAL_GRAPH_ALGORITHM_MAKECONSISTENTORIENTATION_H_
#define _SFCGAL_GRAPH_ALGORITHM_MAKECONSISTENTORIENTATION_H_

#include <map>
#include <SFCGAL/graph/GeometryGraph.h>

namespace SFCGAL {
namespace graph {
namespace algorithm {

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
		 * look for opposite or parallel edges in reference and target
		 */
		bool hasOppositeEdge = false ;
		bool hasParallelEdge = false ;
		for ( size_t i = 0; i < reference.size(); i++ ){
			for ( size_t j = 0; j < target.size(); j++ ){
				if ( graph.areOpposite( reference[i], target[j] ) )
					hasOppositeEdge = true ;
				if ( graph.areParallel( reference[i], target[j] ) )
					hasParallelEdge = true ;
			}
		}

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
		 * Only parallel edge found, lets revert target order
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
