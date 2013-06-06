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
#ifndef _SFCGAL_TRIANGULATE_IN_GEOMETRY_SET_H_
#define _SFCGAL_TRIANGULATE_IN_GEOMETRY_SET_H_

#include <SFCGAL/config.h>

#include <SFCGAL/detail/GeometrySet.h>

namespace SFCGAL {
namespace triangulate {

	/**
	 * Populate the GeometrySet<3> geometry with the triangulation (list of triangles) of a polyhedron
	 */
	SFCGAL_API void triangulate( const MarkedPolyhedron& polyhedron, GeometrySet<3>& geometry );
	/**
	 * Populate the GeometrySet<2> geometry with the triangulation (list of polygons) of a polygon
	 */
	SFCGAL_API void triangulate( const CGAL::Polygon_with_holes_2<Kernel>& polygon, GeometrySet<2>& geometry );

}//algorithm
}//SFCGAL

#endif

