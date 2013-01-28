#ifndef _SFCGAL_ALGORITHM_OFFSET_H_
#define _SFCGAL_ALGORITHM_OFFSET_H_

#include <memory>

namespace SFCGAL {
	class Geometry ;
	class MultiPolygon ;
}

namespace SFCGAL {
namespace algorithm {

	/**
	 * @brief [experimental]compute polygon offset
	 *
	 * @warning test in order to compare with minkowski sum, supports only LineString and Polygon
	 */
	std::auto_ptr< MultiPolygon > offset( const Geometry & g, const double & r ) ;


}//namespace algorithm
}//namespace SFCGAL

#endif
