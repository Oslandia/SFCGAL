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

#include <SFCGAL/Geometry.h>
#include <SFCGAL/io/detail/WktReader.h>
#include <SFCGAL/io/detail/WktWriter.h>
#include <SFCGAL/io/detail/CharArrayBuffer.h>

namespace SFCGAL {
namespace io {
    
    
    
    template < typename K, int N >
    void toWkt( std::ostream& s, const Geometry<K,N> & g ){
        detail::WktWriter<K,N> writer(s);
        boost::apply_visitor(writer,g);
    }
    
    template < typename K, int N >
    std::string toWkt( const Geometry<K,N> & g ){
        std::ostringstream oss;
        toWkt<K,N>(oss,g);
        return oss.str();
    }

    /**
     * Read a WKT geometry from an input stream
     */
    template < typename K, int N >
    Geometry<K,N> readWkt( std::istream& s ) {
        detail::WktReader<K,N> wktReader( s );
        return wktReader.readGeometry();
    }

    /**
     * Read a WKT geometry from a string
     */
    template < typename K, int N >
    Geometry<K,N> readWkt( const std::string& s ) {
        std::istringstream iss( s );
        detail::WktReader<K,N> wktReader( iss );
        Geometry<K,N> result = wktReader.read() ;
        char extra;
        if ( iss >> extra ) {
            std::string remaining( s.substr( int(iss.tellg()) - 1 ) );
            throw WktParseException( "Extra characters in WKT : " + remaining );
        }
        return result;
    }

    /**
     * Read a WKT geometry from a char*
     */
    template < typename K, int N >
    Geometry<K,N> readWkt( const char* str, size_t len)
    {
        CharArrayBuffer buf( str, str + len );
        std::istream istr( &buf );
        detail::WktReader<K,N> wktReader( istr );
        Geometry<K,N> result = wktReader.read() ;
        char extra;
        if ( istr >> extra ) {
            std::string remaining( str + int(istr.tellg()) - 1, str + len );
            throw WktParseException( "Extra characters in WKT: " + remaining );
        }
        return result;
    }


} // io
} // SFCGAL

#endif
