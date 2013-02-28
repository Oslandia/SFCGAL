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
#include <map>
#include <sstream>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/detail/intersects.h>
#include <SFCGAL/detail/GeometrySet.h>

//#define CACHE_TRIANGULATION

namespace SFCGAL {
namespace algorithm
{
	bool intersects( const Geometry& ga, const Geometry& gb )
	{
		SFCGAL::detail::GeometrySet<2> gsa( ga );
		SFCGAL::detail::GeometrySet<2> gsb( gb );

		return algorithm::detail::intersects( gsa, gsb );
	}
}
}
