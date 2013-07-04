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
#ifndef _SFCGAL_ALGORITHM_CONVEXHULL_H_
#define _SFCGAL_ALGORITHM_CONVEXHULL_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

/**
 * Compute the 2D convex hull for a geometry
 */
SFCGAL_API std::auto_ptr<Geometry> convexHull( const Geometry& g ) ;

/**
 * Compute the 3D convex hull for a geometry
 * @todo improve to handle collinear points and coplanar points
 */
SFCGAL_API std::auto_ptr<Geometry> convexHull3D( const Geometry& g ) ;


}//algorithm
}//SFCGAL


#endif
