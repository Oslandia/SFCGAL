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
	 * @todo missing cases (union)
	 * @pre gA and gB are valid geometries
	 * @ingroup public_api
	 */
	SFCGAL_API std::auto_ptr< Geometry > minkowskiSum( const Geometry& gA, const Polygon& gB ) ;

	/**
	 * @brief 2D minkowski sum (p+q)
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
