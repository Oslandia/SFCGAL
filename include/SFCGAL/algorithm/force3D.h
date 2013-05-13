#ifndef _SFCGAL_ALGORITHM_FORCE3D_H_
#define _SFCGAL_ALGORITHM_FORCE3D_H_

#include <SFCGAL/Kernel.h>

namespace SFCGAL {
	class Geometry ;
}

namespace SFCGAL {
namespace algorithm {

	/**
	 * @brief force a 2D geometry to be 3D (replace undefined Z by defaultZ, existing Z values remains unchanged)
	 * @warning ignore empty geometries
	 */
	void force3D( Geometry& g, const Kernel::FT & defaultZ = 0 ) ;

} // namespace algorithm
} // namespace SFCGAL

#endif
