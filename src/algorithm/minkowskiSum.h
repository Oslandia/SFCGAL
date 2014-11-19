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

#ifndef _SFCGAL_ALGORITHM_MINKOWSKISUM_H_
#define _SFCGAL_ALGORITHM_MINKOWSKISUM_H_

#include <SFCGAL/config.h>

#include <memory>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {
struct NoValidityCheck;

/**
 * @brief 2D minkowski sum (p+q)
 *
 * @warning If gA is a polygon, its orientation is taken into account. A "reversed" polygon (with a clockwise-oriented exterior ring) will involve a minkowski difference rather than a sum.
 *
 * @todo missing cases (union)
 * @pre gA and gB are valid geometries
 * @ingroup public_api
 */
SFCGAL_API std::auto_ptr< Geometry > minkowskiSum( const Geometry& gA, const Polygon& gB ) ;

/**
 * @brief 2D minkowski sum (p+q)
 *
 * @warning If gA is a polygon, its orientation is taken into account. A "reversed" polygon (with a clockwise-oriented exterior ring) will involve a minkowski difference rather than a sum.
 *
 * @todo missing cases (union)
 * @pre gA and gB are valid geometries
 * @ingroup detail
 * @warning@ No actual validity check is done.
 */
SFCGAL_API std::auto_ptr< Geometry > minkowskiSum( const Geometry& gA, const Polygon& gB, NoValidityCheck ) ;

} // namespace algorithm
} // namespace SFCGAL

#endif
