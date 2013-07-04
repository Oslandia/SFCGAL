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
#ifndef _SFCGAL_ALGORITHM_LENGTH_H_
#define _SFCGAL_ALGORITHM_LENGTH_H_

#include <SFCGAL/config.h>

namespace SFCGAL {
class Geometry ;
class LineString ;
class GeometryCollection ;
} // namespace SFCGAL

namespace SFCGAL {
namespace algorithm {

/**
 * @brief Compute the 2D length for a Geometry (0 for incompatible types)
 */
SFCGAL_API double length( const Geometry& g ) ;
/**
 * @brief Compute the 2D length for a LineString
 */
SFCGAL_API double length( const LineString& g ) ;
/**
 * @brief Compute the 2D length for a GeometryCollection
 */
SFCGAL_API double length( const GeometryCollection& g ) ;

/**
 * @brief Compute the 2D length for a geometry
 * @return the length of the Geometry, 0 for incompatible types
 */
SFCGAL_API double length3D( const Geometry& g ) ;
/**
 * @brief Compute the 3D length for a LineString
 */
SFCGAL_API double length3D( const LineString& g ) ;
/**
 * @brief Compute the 3D length for a GeometryCollection
 */
SFCGAL_API double length3D( const GeometryCollection& g ) ;


}//algorithm
}//SFCGAL


#endif
