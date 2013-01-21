#ifndef _SFCGAL_GENERATOR_HOCH_H_
#define _SFCGAL_GENERATOR_HOCH_H_

#include <memory>

namespace SFCGAL {
	class Polygon ;
}

namespace SFCGAL {
namespace generator {

	/**
	 * generate hoch snowflake
	 */
	std::auto_ptr< Polygon > hoch( const unsigned int & order ) ;

} // namespace generator
} // namespace SFCGAL

#endif
