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
#ifndef _SFCGAL_ALGORITHM_ORIENTATION_H_
#define _SFCGAL_ALGORITHM_ORIENTATION_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {
	/**
	 * Test if a Geometry has a consistent orientation
	 */
	bool hasConsistentOrientation3D( const TriangulatedSurface & g ) ;
	/**
	 * Test if a PolyhedralSurface has a consistent orientation
	 */
	bool hasConsistentOrientation3D( const PolyhedralSurface & g ) ;


	/**
	 * Try to make consistent orientation in a TriangulatedSurface
	 */
	void makeConsistentOrientation3D( TriangulatedSurface & g ) ;

	/**
	 * Test if a 2D surface is oriented counter clockwise
	 */
	bool isCounterClockWiseOriented( const Polygon& );

	/**
	 * Test if a 2D surface is oriented counter clockwise
	 */
	bool isCounterClockWiseOriented( const Triangle& );

	/**
	 * Test if a 2D surface is oriented counter clockwise
	 */
	bool isCounterClockWiseOriented( const LineString& );

}//algorithm
}//SFCGAL


#endif
