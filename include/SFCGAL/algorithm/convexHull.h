#ifndef _SFCGAL_ALGORITHM_CONVEXHULL_H_
#define _SFCGAL_ALGORITHM_CONVEXHULL_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Compute the 3D convex hull for a geometry
	 * @todo improve to handle collinear points and coplanar points
	 */
	Geometry* convexHull3D( const Geometry & g ) ;


}//algorithm
}//SFCGAL


#endif
