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

#include <SFCGAL/io/detail/WktReader.h>
#include <SFCGAL/io/detail/CharArrayBuffer.h>
#include <SFCGAL/Exception.h>

using SFCGAL::io::detail::WktReader ;

namespace SFCGAL {
namespace io {


///
///
///
Geometry<Epeck,3> readWkt( std::istream& s )
{
    WktReader wktReader( s );
    return wktReader.readGeometry();
}

///
///
///
Geometry<Epeck,3> readWkt( const std::string& s )
{
    std::istringstream iss( s );
    WktReader wktReader( iss );
    Geometry<Epeck,3> geom = wktReader.readGeometry() ;

    char extra;
    if ( iss >> extra ) {
        std::string remaining( s.substr( int(iss.tellg()) - 1 ) );
        throw WktParseException( "Extra characters in WKT: " + remaining );
    }
    return geom;
}

///
///
///
Geometry<Epeck,3> readWkt( const char* str, size_t len )
{
    CharArrayBuffer buf( str, str + len );
    std::istream istr( &buf );
    WktReader wktReader( istr );
    Geometry<Epeck,3> geom( wktReader.readGeometry() );
    char extra;
    if ( istr >> extra ) {
        std::string remaining( str + int(istr.tellg()) - 1, str + len );
        throw WktParseException( "Extra characters in WKT: " + remaining );
    }
    return geom;
}

}//io
}//SFCGAL
