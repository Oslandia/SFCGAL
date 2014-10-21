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

#ifndef _SFCGAL_ALGORITHM_ORIENTATION_H_
#define _SFCGAL_ALGORITHM_ORIENTATION_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Geometry.h>

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

namespace SFCGAL {
namespace algorithm {

/**
 * Make valid 2D orientation
 */
SFCGAL_API void makeValidOrientation( CGAL::Polygon_2< Kernel > & polygon ) ;
/**
 * Make valid 2D orientation
 */
SFCGAL_API void makeValidOrientation( CGAL::Polygon_with_holes_2< Kernel > & polygon ) ;
/**
 * Make valid 2D orientation
 */
SFCGAL_API void makeValidOrientation( Polygon& polygon ) ;


/**
 * Test if a Geometry has a consistent orientation
 */
SFCGAL_API bool hasConsistentOrientation3D( const TriangulatedSurface& g ) ;
/**
 * Test if a PolyhedralSurface has a consistent orientation
 */
SFCGAL_API bool hasConsistentOrientation3D( const PolyhedralSurface& g ) ;


/**
 * Try to make consistent orientation in a TriangulatedSurface
 */
SFCGAL_API void makeConsistentOrientation3D( TriangulatedSurface& g ) ;

/**
 * Test if a 2D surface is oriented counter clockwise
 */
SFCGAL_API bool isCounterClockWiseOriented( const Polygon& );

/**
 * Test if a 2D surface is oriented counter clockwise
 */
SFCGAL_API bool isCounterClockWiseOriented( const Triangle& );

/**
 * Test if a 2D surface is oriented counter clockwise
 */
SFCGAL_API bool isCounterClockWiseOriented( const LineString& );

}//algorithm
}//SFCGAL


#endif
