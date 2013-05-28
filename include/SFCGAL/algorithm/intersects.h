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
#ifndef SFCGAL_INTERSECTS_ALGORITHM
#define SFCGAL_INTERSECTS_ALGORITHM

#include <SFCGAL/config.h>

namespace SFCGAL {
	class Geometry;
	template <int Dim> class GeometrySet;
	template <int Dim> class PrimitiveHandle;

	namespace algorithm {
	/*
	 * Intersection test on 2D geometries. Force projection to z=0 if needed
	 */
	SFCGAL_API bool intersects( const Geometry& ga, const Geometry& gb );

	/*
	 * Intersection test on 3D geometries. Assume z = 0 if needed
	 */
	SFCGAL_API bool intersects3D( const Geometry& ga, const Geometry& gb );

	template <int Dim>
	bool intersects( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b );

	template <int Dim>
	bool intersects( const PrimitiveHandle<Dim>& a, const PrimitiveHandle<Dim>& b );

    }
}

#endif
