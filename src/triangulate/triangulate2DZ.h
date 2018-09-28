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

#ifndef _SFCGAL_TRIANGULATE_TRIANGULATE2DZ_H_
#define _SFCGAL_TRIANGULATE_TRIANGULATE2DZ_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/detail/triangulate/ConstraintDelaunayTriangulation.h>

namespace SFCGAL {
namespace triangulate {
/**
 * @brief Constraint 2DZ Delaunay Triangulation (keep Z if defined, a projectionPlane may be provided)
 */
SFCGAL_CXX_API void triangulate2DZ( const Geometry& g, ConstraintDelaunayTriangulation& triangulate );
/**
 * @brief Constraint 2DZ Delaunay Triangulation (keep Z if defined, project points in OXY plane)
 */
SFCGAL_CXX_API ConstraintDelaunayTriangulation triangulate2DZ( const Geometry& g );

}//algorithm
}//SFCGAL

#endif

