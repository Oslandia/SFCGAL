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

#ifndef _SFCGAL_ALGORITHM_FORCE2D_H_
#define _SFCGAL_ALGORITHM_FORCE2D_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Kernel.h>

namespace SFCGAL {
class Geometry ;
}

namespace SFCGAL {
namespace algorithm {

/**
 * @brief force a geometry to be 2D (project on O,x,y)
 * @warning ignore empty geometries
 */
SFCGAL_API void force2D( Geometry& g );

} // namespace algorithm
} // namespace SFCGAL

#endif
