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

#ifndef _SFCGAL_IO_WKT_H_
#define _SFCGAL_IO_WKT_H_

#include <SFCGAL/config.h>

#include <sstream>
#include <string>
#include <memory>

namespace SFCGAL {
class Geometry ;
}

namespace SFCGAL {
namespace io {
/**
 * Read a WKT geometry from an input stream
 */
SFCGAL_CXX_API std::auto_ptr< Geometry > readWkt( std::istream& s ) ;
/**
 * Read a WKT geometry from a string
 */
SFCGAL_CXX_API std::auto_ptr< Geometry > readWkt( const std::string& s ) ;
/**
 * Read a WKT geometry from a char*
 */
SFCGAL_CXX_API std::auto_ptr< Geometry > readWkt( const char*, size_t );
}
}

#endif
