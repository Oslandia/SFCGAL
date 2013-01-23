#ifndef _SFCGAL_ALGORITHM_MINKOWSKISUM_H_
#define _SFCGAL_ALGORITHM_MINKOWSKISUM_H_

#include <memory>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * @brief 2D minkowski sum (p+q)
	 *
	 * @todo missing cases (union) and input polygon (gA) with holes (decompose polygon/union)
	 */
	std::auto_ptr< Geometry > minkowskiSum( const Geometry& gA, const Polygon& gB ) ;

	/**
	 * @brief 2D minkowski sum (p+q)
	 */
	std::auto_ptr< Geometry > minkowskiSum( const Point& gA, const Polygon& gB ) ;
	/**
	 * @brief 2D minkowski sum (p+q)
	 */
	std::auto_ptr< Geometry > minkowskiSum( const LineString& gA, const Polygon& gB ) ;
	/**
	 * @brief 2D minkowski sum (p+q)
	 */
	std::auto_ptr< Geometry > minkowskiSum( const Polygon& gA, const Polygon& gB ) ;

} // namespace algorithm
} // namespace SFCGAL

#endif
