/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _SFCGAL_ALGORITHM_DISTANCE_H_
#define _SFCGAL_ALGORITHM_DISTANCE_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Geometry.h>


namespace SFCGAL {
namespace algorithm {
struct NoValidityCheck;

/**
 * Compute the distance between two Geometries
 * @ingroup public_api
 * @pre gA is a valid geometry
 * @pre gB is a valid geometry
 */
SFCGAL_API double distance( const Geometry& gA, const Geometry& gB ) ;

/**
 * Compute the distance between two Geometries
 * @ingroup detail
 * @pre gA is a valid geometry
 * @pre gB is a valid geometry
 * @warning No actual validity check is done
 */
SFCGAL_API double distance( const Geometry& gA, const Geometry& gB, NoValidityCheck ) ;

/**
 * dispatch distance from Point to Geometry
 * @ingroup detail
 */
SFCGAL_API double distancePointGeometry( const Point& gA, const Geometry& gB ) ;
/**
 * distance between two Points
 * @ingroup detail
 */
SFCGAL_API double distancePointPoint( const Point& gA, const Point& gB ) ;
/**
 * distance between a Point and a LineString
 * @ingroup detail
 */
SFCGAL_API double distancePointLineString( const Point& gA, const LineString& gB ) ;
/**
 * distance between a Point and a Polygon
 * @ingroup detail
 */
SFCGAL_API double distancePointPolygon( const Point& gA, const Polygon& gB ) ;
/**
 * distance between a Point and a Triangle
 * @ingroup detail
 */
SFCGAL_API double distancePointTriangle( const Point& gA, const Triangle& gB ) ;

/**
 * dispatch distance from LineString to Geometry
 * @ingroup detail
 */
SFCGAL_API double distanceLineStringGeometry( const LineString& gA, const Geometry& gB ) ;
/**
 * distance between two LineStrings
 * @ingroup detail
 */
SFCGAL_API double distanceLineStringLineString( const LineString& gA, const LineString& gB ) ;
/**
 * distance between a LineString and a Polygon
 * @ingroup detail
 */
SFCGAL_API double distanceLineStringPolygon( const LineString& gA, const Polygon& gB ) ;
/**
 * distance between a LineString and a Triangle
 * @ingroup detail
 */
SFCGAL_API double distanceLineStringTriangle( const LineString& gA, const Triangle& gB ) ;

/**
 * dispatch distance from Polygon to Geometry
 * @ingroup detail
 */
SFCGAL_API double distancePolygonGeometry( const Polygon& gA, const Geometry& gB ) ;
/**
 * distance between two Polygons
 * @ingroup detail
 */
SFCGAL_API double distancePolygonPolygon( const Polygon& gA, const Polygon& gB ) ;
/**
 * distance between a Polygon and a Triangle
 * @ingroup detail
 */
SFCGAL_API double distancePolygonTriangle( const Polygon& gA, const Triangle& gB ) ;


/**
 * dispatch distance from a Triangle to a Geometry
 * @ingroup detail
 */
SFCGAL_API double distanceTriangleGeometry( const Triangle& gA, const Geometry& gB ) ;


/**
 * dispatch distance from a collection of geometry (gA) to a Geometry (gB)
 * @ingroup detail
 */
SFCGAL_API double distanceGeometryCollectionToGeometry( const Geometry& gA, const Geometry& gB ) ;



/**
 * @ingroup detail
 */
SFCGAL_API double distancePointSegment( const Point& p, const Point& a, const Point& b ) ;

/**
 * @ingroup detail
 */
SFCGAL_API double distanceSegmentSegment( const Point& a, const Point& b, const Point& c, const Point& d );

}//namespace algorithm
}//namespace SFCGAL

#endif
