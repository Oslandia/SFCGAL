#ifndef _SFCGAL_ALGORITHM_BOUNDARY_H_
#define _SFCGAL_ALGORITHM_BOUNDARY_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Compute the boundary for a Point
	 */
	Geometry* boundary( const Point & g ) ;
	/**
	 * Compute the boundary for a Point
	 */
	Geometry* boundary( const LineString & g ) ;
	/**
	 * Compute the boundary for a Point
	 */
	Geometry* boundary( const LineString & g ) ;


}//algorithm
}//SFCGAL


#endif
