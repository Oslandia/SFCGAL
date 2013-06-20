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
#ifndef _SFCGAL_TRIANGULATE_TRIANGULATEPOLYGON_H_
#define _SFCGAL_TRIANGULATE_TRIANGULATEPOLYGON_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace triangulate {

	/**
	 * @brief Triangulate 3D polygons in a Geometry.
	 *
	 * @param g input geometry
	 * @param triangulatedSurface resulting TriangulatedSurface
	 * @param usePolygonPlanes use polygon plane or Triangulate in OXY plane
	 *
	 * @todo unittest
	 */
	SFCGAL_API void triangulatePolygon3D(
		const Geometry & g,
		TriangulatedSurface & triangulatedSurface
	);
	/**
	 * @brief Triangulate a 3D Polygon
	 *
	 * @todo unittest
	 */
	SFCGAL_API void triangulatePolygon3D(
		const Polygon & g,
		TriangulatedSurface & triangulatedSurface
	);
	/**
	 * @brief Triangulate a 3D Triangle (copy triangle)
	 *
	 * @todo unittest
	 */
	SFCGAL_API void triangulatePolygon3D(
		const Triangle & g,
		TriangulatedSurface & triangulatedSurface
	);
	/**
	 * @brief Triangulate a 3D TriangulatedSurface (copy triangles)
	 *
	 * @todo unittest
	 */
	SFCGAL_API void triangulatePolygon3D(
		const TriangulatedSurface & g,
		TriangulatedSurface & triangulatedSurface
	);
	/**
	 * @brief Triangulate a 3D MultiPolygon
	 *
	 * @todo unittest
	 */
	SFCGAL_API void opentriangulatePolygon3D(
		const GeometryCollection & g,
		TriangulatedSurface & triangulatedSurface
	);
	/**
	 * @brief Triangulate 3D polygons in a PolyhedralSurface.
	 *
	 * @todo unittest
	 */
	SFCGAL_API void triangulatePolygon3D(
		const PolyhedralSurface & polyhedralSurface,
		TriangulatedSurface & triangulatedSurface
	);
	/**
	 * @brief Triangulate a Solid
	 *
	 * @todo unittest
	 */
	SFCGAL_API void triangulatePolygon3D(
		const Solid & g,
		TriangulatedSurface & triangulatedSurface
	);

}//algorithm
}//SFCGAL

#endif

