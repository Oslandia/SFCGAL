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

#ifndef _SFCGAL_ALGORITHM_FORCE3D_H_
#define _SFCGAL_ALGORITHM_FORCE3D_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Kernel.h>

namespace SFCGAL {
class Geometry ;
}

namespace SFCGAL {
namespace algorithm {

/**
 * @brief force a 2D geometry to be 3D (replace undefined Z by defaultZ, existing Z values remains unchanged)
 * @warning ignore empty geometries
 */
SFCGAL_API void force3D( Geometry& g, const Kernel::FT& defaultZ = 0 ) ;

} // namespace algorithm
} // namespace SFCGAL

#endif
