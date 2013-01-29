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
#ifndef SFCGAL_COVERS_ALGORITHM
#define SFCGAL_COVERS_ALGORITHM

#include <vector>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
    namespace algorithm {
	/*
	 * Cover test on 2D geometries. Checks if gA covers gB. Force projection to z=0 if needed
	 */
	bool covers( const Geometry& ga, const Geometry& gb );

	/*
	 * Cover test on 3D geometries. Checks if gA covers gB. Assume z = 0 if needed
	 */
	bool covers3D( const Geometry& ga, const Geometry& gb );

	/*
	 * Cover test on a list of points against a Solid.  Checks if the solid covers one of the points
	 */
	bool covers3D( const Solid& solid, std::vector<const Point*>& pts );
    }
}

#endif
