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
#ifndef _SFCGAL_ALGORITHM_DETAIL_INTERSECTS_
#define _SFCGAL_ALGORITHM_DETAIL_INTERSECTS_

#include <SFCGAL/detail/GeometrySet.h>

namespace SFCGAL {
namespace algorithm {
namespace detail {

	template <int Dim>
	bool intersects( const SFCGAL::detail::GeometrySet<Dim>& a, const SFCGAL::detail::GeometrySet<Dim>& b );

} // detail
} // algorithm
} // SFCGAL

#endif
