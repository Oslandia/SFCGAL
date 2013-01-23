#ifndef _SFCGAL_TRANSLATE_H_
#define _SFCGAL_TRANSLATE_H_

#include <SFCGAL/Kernel.h>

namespace SFCGAL {
	class Geometry ;
}

namespace SFCGAL {
namespace algorithm {

	/**
	 * @brief translate a geometry from a given vector
	 */
	void       translate( Geometry & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;

} // namespace algorithm
} // namespace SFCGAL

#endif

