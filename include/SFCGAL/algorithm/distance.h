#ifndef _SFCGAL_ALGORITHM_DISTANCE_H_
#define _SFCGAL_ALGORITHM_DISTANCE_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {
	/**
	 * distance between two Points
	 *
	 * @warning no empty geometries
	 */
	double distancePointPoint( const Point & a, const Point & b );
	/**
	 * distance between a Point and a Segment
	 *
	 * @warning no empty geometries
	 */
	double distancePointSegment( const Point & p, const Point & a, const Point & b );
	/**
	 * distance between a Point and a LineString
	 *
	 * @warning no empty geometries
	 */
	double distancePointLineString( const Point & a, const LineString& lineString );
	/**
	 * distance between a Point and a Polygon
	 *
	 * @warning no empty geometries
	 */
	double distancePointPolygon( const Point & point, const Polygon& polygon );

	/**
	 * distance between two Segments
	 *
	 * @warning no empty geometries
	 */
	double distanceSegmentSegment(
		const Point & a, const Point & b,
		const Point & c, const Point & d
	);

	/**
	 * distance between two LineStrings
	 *
	 * @warning no empty geometries
	 * @todo cull distance bbox? bounding sphere?
	 */
	double distanceLineStringLineString(
		const LineString & gA,
		const LineString & gB
	);

	/**
	 * distance between two polygons
	 */
	double distancePolygonPolygon(
		const Polygon & gA,
		const Polygon & gB
	);

}//namespace algorithm
}//namespace SFCGAL

#endif
