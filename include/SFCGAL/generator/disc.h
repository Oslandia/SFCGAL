#ifndef _SFCGAL_GENERATOR_DISC_H_
#define _SFCGAL_GENERATOR_DISC_H_

#include <memory>

namespace SFCGAL {
	class Point ;
	class Polygon ;
}

namespace SFCGAL {
namespace generator {

	/**
	 * Generate a discrete circle
	 */
	std::auto_ptr< Polygon > disc(
		const Point& center,
		const double & radius,
		const unsigned int & nQuadrantSegments = 8U
	) ;

} // namespace generator
} // namespace SFCGAL

#endif
