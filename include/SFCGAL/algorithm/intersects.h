#ifndef SFCGAL_INTERSECTS_ALGORITHM
#define SFCGAL_INTERSECTS_ALGORITHM

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
    namespace algorithm {
	/*
	 * Intersection test on 2D geometries. Force projection to z=0 if needed
	 */
	bool intersects( const Geometry& ga, const Geometry& gb );

	/*
	 * Intersection test on 3D geometries. Assume z = 0 if needed
	 */
	bool intersects3D( const Geometry& ga, const Geometry& gb );
    }
}

#endif
