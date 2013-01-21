#ifndef _SFCGAL_GENERATOR_SIERPINSKI_H_
#define _SFCGAL_GENERATOR_SIERPINSKI_H_

#include <memory>

namespace SFCGAL {
	class MultiPolygon ;
}

namespace SFCGAL {
namespace generator {

	/**
	 * generate hoch snowflake
	 */
	std::auto_ptr< MultiPolygon > sierpinski( const unsigned int & order ) ;

} // namespace generator
} // namespace SFCGAL

#endif
