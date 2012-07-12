#ifndef SFCGAL_INTERSECTION_ALGORITHM
#define SFCGAL_INTERSECTION_ALGORITHM

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
    namespace algorithm {
	/*
	 * Intersection on 2D geometries.
	 */
	std::auto_ptr<Geometry> intersection( const Geometry& ga, const Geometry& gb );

	/*
	 * Intersection on 3D geometries. Assume z = 0 if needed
	 */
	std::auto_ptr<Geometry> intersection3D( const Geometry& ga, const Geometry& gb );
    }
}

#endif
