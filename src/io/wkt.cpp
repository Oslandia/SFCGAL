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

#include <SFCGAL/io/wkt.h>

#include <SFCGAL/detail/io/WktReader.h>
#include <SFCGAL/detail/io/WktWriter.h>
#include <SFCGAL/detail/tools/CharArrayBuffer.h>
#include <SFCGAL/Exception.h>

using namespace SFCGAL::detail::io;

namespace SFCGAL {
namespace io {

///
///
///
std::auto_ptr< Geometry > readWkt( std::istream& s )
{
    WktReader wktReader( s );
    return std::auto_ptr< Geometry >( wktReader.readGeometry() );
}

///
///
///
std::auto_ptr< Geometry > readWkt( const std::string& s )
{
    std::istringstream iss( s );
    WktReader wktReader( iss );
    std::auto_ptr< Geometry > geom( wktReader.readGeometry() );
    if ( !wktReader.eof() ) {
        std::string remaining( s.substr( iss.tellg() ) );
        throw WktParseException( "Extra characters in WKT: " + remaining );
    }
    return geom;
}

///
///
///
std::auto_ptr< Geometry > readWkt( const char* str, size_t len )
{
    CharArrayBuffer buf( str, str + len );
    std::istream istr( &buf );
    WktReader wktReader( istr );
    std::auto_ptr< Geometry > geom( wktReader.readGeometry() );
    if ( !wktReader.eof() ) {
        std::string remaining( str + istr.tellg(), str + len );
        throw WktParseException( "Extra characters in WKT: " + remaining );
    }
    return geom;
}

}//io
}//SFCGAL


