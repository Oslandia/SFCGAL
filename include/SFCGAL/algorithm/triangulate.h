/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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
#ifndef _SFCGAL_ALGORITHM_TRIANGULATE_H_
#define _SFCGAL_ALGORITHM_TRIANGULATE_H_

#include <SFCGAL/Geometry.h>
#include <SFCGAL/Kernel.h>
#include <CGAL/Polyhedron_3.h>

namespace SFCGAL {
	namespace detail {
		template <int Dim> class GeometrySet;
	}
namespace algorithm {
	/**
	 * Convert a 2D/3D [Multi]Polygon to a TriangulatedSurface.
	 */
	void triangulate( const Geometry & geometry, TriangulatedSurface & triangulatedSurface ) ;
	/**
	 * Convert a 2D Geometry to a TriangulatedSurface.
	 */
	void triangulate2D( const Geometry & geometry, TriangulatedSurface & triangulatedSurface ) ;
	/**
	 * Convert a 2D/3D point set to a TriangulatedSurface.
	 * @warning projects points in the OXY plane
	 */
	void triangulate( const MultiPoint & geometry, TriangulatedSurface & triangulatedSurface ) ;
	/**
	 * Convert a 2D/3D polygon to a TriangulatedSurface.
	 * @warning projects points in the polygon plane
	 */
	void triangulate( const Polygon & polygon, TriangulatedSurface & triangulatedSurface ) ;
	/**
	 * Convert a 2D polygon to a TriangulatedSurface.
	 */
	void triangulate2D( const Polygon & polygon, TriangulatedSurface & triangulatedSurface ) ;
	/**
	 * Convert a 2D/3D MultiPolygon to a TriangulatedSurface. Each polygon is triangulated invidualy
	 * @warning projects points in the polygon plane
	 */
	void triangulate( const MultiPolygon & polygon, TriangulatedSurface & triangulatedSurface ) ;
	/**
	 * Convert a 2D MultiPolygon to a TriangulatedSurface. Each polygon is triangulated invidualy
	 */
	void triangulate2D( const MultiPolygon & polygon, TriangulatedSurface & triangulatedSurface ) ;
	/**
	 * Convert a 2D/3D PolyhedralSurface to a TriangulatedSurface. Each polygon is triangulated invidualy
	 * @warning projects points in the polygon plane
	 */
	void triangulate( const PolyhedralSurface & poly, TriangulatedSurface & triangulatedSurface ) ;
	/**
	 * Convert a 2D PolyhedralSurface to a TriangulatedSurface. Each polygon is triangulated invidualy
	 */
	void triangulate2D( const PolyhedralSurface & poly, TriangulatedSurface & triangulatedSurface ) ;

	void triangulate( const CGAL::Polyhedron_3<Kernel>& polyhedron, SFCGAL::detail::GeometrySet<3>& geometry );
}//algorithm
}//SFCGAL

#endif

