#ifndef _SFCGAL_GRAPH_EDGE_H_
#define _SFCGAL_GRAPH_EDGE_H_


namespace SFCGAL {
namespace graph {

	/**
	 * An edge in a GeometryGraph with minimal requirements (some algorithms could need
	 * more informations)
	 */
	struct Edge {
		Edge( const int & face_ = -1 );

		int face ;
	};

}//graph
}//SFCGAL

#endif
