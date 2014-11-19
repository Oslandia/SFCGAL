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

#ifndef _SFCGAL_ALGORITHM_EXTRUDE_H_
#define _SFCGAL_ALGORITHM_EXTRUDE_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {
struct NoValidityCheck;

/**
 * extrude a Geometry with a direction
 * @pre g is a valid geometry
 * @ingroup public_api
 */
SFCGAL_API std::auto_ptr< Geometry > extrude( const Geometry& g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;

/**
 * extrude a Geometry with a direction
 * @pre g is a valid geometry
 * @ingroup detail
 * @warning No actual validity check is done.
 */
SFCGAL_API std::auto_ptr< Geometry > extrude( const Geometry& g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz, NoValidityCheck ) ;

/**
 * extrude a Geometry with a direction
 * @pre g is a valid geometry
 * @pre dx, dy and dz are finite values
 * @ingroup detail
 * @warning No actual validity check is done.
 */
SFCGAL_API std::auto_ptr< Geometry > extrude( const Geometry& g, const double& dx, const double& dy, const double& dz );

/**
 * @brief extrude a Geometry by a given vector
 * @todo Improve 3D surface extrude (Extrude only faces whose dot( v, normal ) > 0 and use Polyhedron union to
 *         get output geometries with a clean topology)
 * @ingroup detail
 */
SFCGAL_API std::auto_ptr< Geometry > extrude( const Geometry& g, const Kernel::Vector_3& v ) ;

}//algorithm
}//SFCGAL

#endif
