#ifndef _SFCGAL_ALGORITHM_FORCE2D_H_
#define _SFCGAL_ALGORITHM_FORCE2D_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Kernel.h>

namespace SFCGAL {
class Geometry ;
}

namespace SFCGAL {
namespace algorithm {

/**
 * @brief force a geometry to be 2D (project on O,x,y)
 * @warning ignore empty geometries
 */
SFCGAL_API void force2D( Geometry& g );

} // namespace algorithm
} // namespace SFCGAL

#endif
