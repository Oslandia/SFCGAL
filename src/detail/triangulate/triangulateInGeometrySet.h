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

#ifndef _SFCGAL_TRIANGULATE_IN_GEOMETRY_SET_H_
#define _SFCGAL_TRIANGULATE_IN_GEOMETRY_SET_H_

#include <SFCGAL/config.h>

#include <SFCGAL/detail/GeometrySet.h>

namespace SFCGAL {
namespace triangulate {

/**
 * Populate the GeometrySet<3> geometry with the triangulation (list of triangles) of a polyhedron
 */
SFCGAL_API void triangulate( const detail::MarkedPolyhedron& polyhedron, detail::GeometrySet<3>& geometry );
/**
 * Populate the GeometrySet<2> geometry with the triangulation (list of polygons) of a polygon
 */
SFCGAL_API void triangulate( const CGAL::Polygon_with_holes_2<Kernel>& polygon, detail::GeometrySet<2>& geometry );

}//algorithm
}//SFCGAL

#endif

