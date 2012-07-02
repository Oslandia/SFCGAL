#ifndef _SFCGAL_GRAPH_VERTEX_H_
#define _SFCGAL_GRAPH_VERTEX_H_

#include <SFCGAL/Coordinate.h>

namespace SFCGAL {
namespace graph {

	/**
	 * A vertex in a GeometryGraph with minimal requirements (some algorithms could need
	 * a richer class)
	 */
	struct Vertex {
		/**
		 * [requirement]Constructor with coordinate
		 */
		Vertex( const Coordinate & coordinate_ = Coordinate() ) ;

		Coordinate coordinate ;
	};

}//graph
}//SFCGAL

#endif
