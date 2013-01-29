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
#ifndef _SFCGAL_TRANSLATE_H_
#define _SFCGAL_TRANSLATE_H_

#include <SFCGAL/Kernel.h>

namespace SFCGAL {
	class Geometry ;
}

namespace SFCGAL {
namespace algorithm {

	/**
	 * @brief translate a geometry from a given vector
	 */
	void       translate( Geometry & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz ) ;

} // namespace algorithm
} // namespace SFCGAL

#endif

