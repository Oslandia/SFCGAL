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

#ifndef _SFCGAL_ALGORITHM_ISVALID_H_
#define _SFCGAL_ALGORITHM_ISVALID_H_

#include <SFCGAL/Geometry.h>
#include <SFCGAL/Validity.h>
#include <SFCGAL/algorithm/force2D.h>
#include <SFCGAL/algorithm/force3D.h>

namespace SFCGAL {

/**
 * Functions used to assert for geometry validity
 * @note exception message is apparently limited in length, thus print the reason for invalidity before its text representation (that can be very long)
 */
void SFCGAL_CXX_API SFCGAL_ASSERT_GEOMETRY_VALIDITY( const Geometry& g );
void SFCGAL_CXX_API SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( const Geometry& g );
void SFCGAL_CXX_API SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( const Geometry& g );
void SFCGAL_CXX_API SFCGAL_ASSERT_GEOMETRY_VALIDITY_ON_PLANE( const Geometry& g );

namespace algorithm {

/**
 * @brief Check validity of a geometry
 * @ingroup public_api
 */
SFCGAL_CXX_API const Validity isValid( const Geometry& g, const double& toleranceAbs= 1e-9 );

/**
 * Sets the geometry flag on a geometry and propagate to every internal geometries
 * @ingroup public_api
 */
SFCGAL_CXX_API void propagateValidityFlag( Geometry& g, bool valid );

/**
 * Tag used for variants of algorithm that do not do validity check
 * @ingroup public_api
 */
struct NoValidityCheck {};

}//algorithm
}//SFCGAL


#endif
