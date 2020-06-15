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
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SFCGAL_ALGORITHM_EXTRUDE_H_
#define _SFCGAL_ALGORITHM_EXTRUDE_H_

// SFCGAL
#include <SFCGAL/config.h>
#include <SFCGAL/Kernel.h>
#include <SFCGAL/Geometry.h>

namespace SFCGAL
{
namespace algorithm
{

// Class forward declarations.
struct NoValidityCheck;

/**
 * @brief Returns a Geometry equal to the specified Geometry,
 *   extruded by the specified displacement.
 * @param g The specified Geometry.
 * @param dx The component of the specified displacement in
 *   the x-direction.
 * @param dy The component of the specified displacement in
 *   the y-direction.
 * @param dz The component of the specified displacement in
 *   the z-direction.
 * @return A Geometry equal to g extruded by the displacement
 *   vector {dx, dy, dz}.
 * @pre g must be a valid geometry.
 * @pre dx, dy and dz must all be finite.
 * @note If g is such that g.isMeasured() is true, then,
 *   since there is no common expectation of the
 *   values of the measures on the returned Geometry,
 *   all measures from the result are removed.
 * @ingroup public_api
 */
SFCGAL_API std::unique_ptr< Geometry > extrude( const Geometry& g
                                              , Kernel::FT dx
                                              , Kernel::FT dy
                                              , Kernel::FT dz
                                              ) ;

/**
 * @brief Returns a Geometry equal to the specified Geometry,
 *   extruded by the specified displacement.
 * @param g The specified Geometry.
 * @param dx The component of the specified displacement in
 *   the x-direction.
 * @param dy The component of the specified displacement in
 *   the y-direction.
 * @param dz The component of the specified displacement in
 *   the z-direction.
 * @param nvc A NoValidityCheck object.
 * @return A Geometry equal to g extruded by the displacement
 *   vector {dx, dy, dz}.
 * @pre g must be a valid geometry.
 * @pre dx, dy and dz must all be finite.
 * @note If g is such that g.isMeasured() is true, then,
 *   since there is no common expectation of the
 *   values of the measures on the returned Geometry,
 *   all measures from the result are removed.
 * @ingroup detail
 * @warning No actual validity check is conducted.
 */
SFCGAL_API std::unique_ptr< Geometry > extrude( const Geometry& g
                                              , Kernel::FT dx
                                              , Kernel::FT dy
                                              , Kernel::FT dz
                                              , NoValidityCheck nvc
                                              ) ;

/**
 * @brief Returns a Geometry equal to the specified Geometry,
 *   extruded by the specified displacement.
 * @param g The specified Geometry.
 * @param dx The component of the specified displacement in
 *   the x-direction.
 * @param dy The component of the specified displacement in
 *   the y-direction.
 * @param dz The component of the specified displacement in
 *   the z-direction.
 * @return A Geometry equal to g extruded by the displacement
 *   vector {dx, dy, dz}.
 * @pre g must be a valid geometry.
 * @pre dx, dy and dz must all be finite.
 * @note If g is such that g.isMeasured() is true, then,
 *   since there is no common expectation of the
 *   values of the measures on the returned Geometry,
 *   all measures from the result are removed.
 * @ingroup detail
 * @warning No actual validity check is conducted.
 */
SFCGAL_API std::unique_ptr< Geometry > extrude( const Geometry& g
                                              , const double& dx
                                              , const double& dy
                                              , const double& dz
                                              );

/**
 * @brief Returns a Geometry equal to the specified Geometry,
 *   extruded by the specified displacement vector.
 * @param g The specified Geometry.
 * @param v The specified displacement vector.
 * @return A Geometry equal to g extruded by the displacement
 *   vector v.
 * @pre g must be a valid geometry.
 * @note If g is such that g.isMeasured() is true, then,
 *   since there is no common expectation of the
 *   values of the measures on the returned Geometry,
 *   all measures from the result are removed.
 * @todo Improve extrude for 3D surfaces - Extrude only
 *   faces whose scalar_product(v,normal) > 0 and use
 *   Polyhedron union to get output geometries with a clean
 *   topology.
 * @ingroup detail
 */
SFCGAL_API std::unique_ptr< Geometry > extrude( const Geometry& g
                                              , const Kernel::Vector_3& v
                                              ) ;

} // ! namespace algorithm
} // ! namespace SFCGAL

#endif // ! _SFCGAL_ALGORITHM_EXTRUDE_H_
