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

#ifndef _SFCGAL_IO_ASC_H_
#define _SFCGAL_IO_ASC_H_

#include <SFCGAL/config.h>

#include <sstream>

#include <SFCGAL/Grid.h>

namespace SFCGAL {
namespace io {

/**
 * @brief read ASC grid from input stream
 * @see http://en.wikipedia.org/wiki/Esri_grid
 */
SFCGAL_API std::auto_ptr< Grid > readASC( std::istream& s ) ;


} // namespace io
} // namespace SFCGAL

#endif
