/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SFCGAL_ALGORITHM_STRAIGHTSKELETON_H_
#define _SFCGAL_ALGORITHM_STRAIGHTSKELETON_H_

#include <SFCGAL/config.h>

#include <memory>

namespace SFCGAL {
class Geometry ;
class Polygon ;
class MultiPolygon ;
class MultiLineString ;
}

namespace SFCGAL {
namespace algorithm {
struct NoValidityCheck;

/**
 * @brief build an approximate medial axis for a Polygon
 * @param g input geometry
 * @ingroup public_api
 * @pre g is a valid geometry
 */
SFCGAL_CXX_API std::auto_ptr< MultiLineString > approximateMedialAxis( const Geometry& g );

/**
 * @brief build a 2D straight skeleton for a Polygon
 * @todo add supports for TriangulatedSurface and PolyhedralSurface
 * @param g input geometry
 * @param autoOrientation check and fix polygon orientation
 * @param outputM whether to output the distance to border as M
 * @ingroup public_api
 * @pre g is a valid geometry
 */
SFCGAL_CXX_API std::auto_ptr< MultiLineString > straightSkeleton( const Geometry& g, bool autoOrientation = true, bool innerOnly = false, bool outputDistanceInM = false ) ;

/**
 * @brief build a 2D straight skeleton for a Polygon
 * @param g input geometry
 * @param autoOrientation check and fix polygon orientation
 * @param outputM whether to output the distance to border as M
 * @ingroup public_api
 * @pre g is a valid geometry
 * @warning No actual validity check is done
 */
SFCGAL_CXX_API std::auto_ptr< MultiLineString > straightSkeleton( const Geometry& g, bool autoOrientation, NoValidityCheck, bool innerOnly = false, bool outputDistanceInM = false ) ;

/**
 * @brief build a 2D straight skeleton for a Polygon
 * @ingroup detail
 */
SFCGAL_CXX_API std::auto_ptr< MultiLineString > straightSkeleton( const Polygon& g, bool autoOrientation = true, bool innerOnly = false, bool outputDistanceInM = false ) ;
/**
 * @brief build a 2D straight skeleton for a Polygon
 * @ingroup detail
 */
SFCGAL_CXX_API std::auto_ptr< MultiLineString > straightSkeleton( const MultiPolygon& g, bool autoOrientation = true, bool innerOnly = false, bool outputDistanceInM = false ) ;

}//namespace algorithm
}//namespace SFCGAL


#endif
