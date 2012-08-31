#ifndef _SFCGAL_ALGORITHM_CONVEXHULL_H_
#define _SFCGAL_ALGORITHM_CONVEXHULL_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Compute the 2D convex hull for a geometry
	 */
	std::auto_ptr<Geometry> convexHull( const Geometry & g ) ;

	/**
	 * Compute the 3D convex hull for a geometry
	 * @todo improve to handle collinear points and coplanar points
	 */
	std::auto_ptr<Geometry> convexHull3D( const Geometry & g ) ;


}//algorithm
}//SFCGAL


#endif
