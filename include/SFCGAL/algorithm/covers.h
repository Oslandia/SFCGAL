#ifndef SFCGAL_COVERS_ALGORITHM
#define SFCGAL_COVERS_ALGORITHM

#include <vector>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
    namespace algorithm {
	/*
	 * Cover test on 2D geometries. Checks if gA covers gB. Force projection to z=0 if needed
	 */
	bool covers( const Geometry& ga, const Geometry& gb );

	/*
	 * Cover test on 3D geometries. Checks if gA covers gB. Assume z = 0 if needed
	 */
	bool covers3D( const Geometry& ga, const Geometry& gb );

	/*
	 * Cover test on a list of points against a Solid.  Checks if the solid covers one of the points
	 */
	bool covers3D( const Solid& solid, std::vector<const Point*>& pts );
    }
}

#endif
