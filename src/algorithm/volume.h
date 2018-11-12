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

#ifndef _SFCGAL_ALGORITHM_VOLUME_H_
#define _SFCGAL_ALGORITHM_VOLUME_H_

#include <SFCGAL/export.h>
#include <SFCGAL/Kernel.h>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

struct NoValidityCheck;

/**
 * Computes the volume of a geometry
 * @pre g is a valid Geometry
 * @ingroup public_api
 */
SFCGAL_CXX_API const Kernel::FT volume( const Geometry& g );

/**
 * Computes the volume of a Solid
 * @pre (not checked) volume is closed and consistently oriented
 * @ingroup detail
 */
SFCGAL_CXX_API const Kernel::FT volume( const Solid& g, NoValidityCheck );

}
}

#endif
