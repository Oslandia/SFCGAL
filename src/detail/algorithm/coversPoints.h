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

#ifndef SFCGAL_COVERS_POINTS_ALGORITHM
#define SFCGAL_COVERS_POINTS_ALGORITHM

#include <SFCGAL/config.h>

#include <vector>

namespace SFCGAL {
class Geometry;
namespace detail {
namespace algorithm {


/**
 * Pseudo cover test on 2D geometries. Collect points of gb and tests if no points of gb is outside ga
 * @ingroup@ detail
 */
SFCGAL_CXX_API bool coversPoints( const Geometry& ga, const Geometry& gb );

/**
 * Pseudo cover test on 3D geometries. Collect points of gb and tests if no points of gb is outside ga
 * @ingroup@ detail
 */
SFCGAL_CXX_API bool coversPoints3D( const Geometry& ga, const Geometry& gb );
}
}
}

#endif
