#ifndef _SFCGAL_ALGORITHM_MINKOWSKISUM_H_
#define _SFCGAL_ALGORITHM_MINKOWSKISUM_H_

#include <memory>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * @brief 2D minkowski sum (p+q)
	 *
	 * @todo missing cases (union)
	 */
	std::auto_ptr< MultiPolygon > minkowskiSum( const Geometry& gA, const Polygon& gB ) ;


} // namespace algorithm
} // namespace SFCGAL

#endif
