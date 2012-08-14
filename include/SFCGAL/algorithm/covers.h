#ifndef SFCGAL_COVERS_ALGORITHM
#define SFCGAL_COVERS_ALGORITHM

#include <vector>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
    namespace algorithm {
	/*
	 * Cover test on 2D geometries. Force projection to z=0 if needed
	 */
	bool covers( const Geometry& ga, const Geometry& gb );

	/*
	 * Cover test on 3D geometries. Assume z = 0 if needed
	 */
	bool covers3D( const Geometry& ga, const Geometry& gb );

	/*
	 * Cover test on a list of points against a Solid
	 */
	bool covers3D( std::vector<const Point*>& pts, const Solid& solid );
    }
}

#endif
