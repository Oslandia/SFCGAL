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

#include <SFCGAL/visitor.h>
#include <SFCGAL/io/detail/WktReader.h>
#include <SFCGAL/io/detail/WktWriter.h>
#include <SFCGAL/io/detail/CharArrayBuffer.h>

namespace SFCGAL {
namespace io {
    
    
    
    template < typename K >
    void toWkt( std::ostream& s, const Geometry<K> & g ){
        detail::WktWriter<K> writer(s);
        SFCGAL::apply_visitor(writer,g);
    }
    
    template < typename K >
    std::string toWkt( const Geometry<K> & g ){
        std::ostringstream oss;
        toWkt<K>(oss,g);
        return oss.str();
    }

    /**
     * Read a WKT geometry from an input stream
     */
    template < typename K >
    std::unique_ptr< Geometry<K> > readWkt( std::istream& s ) {
        detail::WktReader<K> wktReader( s );
        return std::unique_ptr< Geometry<K> >( wktReader.readGeometry() ) ;
    }

    /**
     * Read a WKT geometry from a string
     */
    template < typename K >
    std::unique_ptr< Geometry<K> > readWkt( const std::string& s ) {
        std::istringstream iss( s );
        detail::WktReader<K> wktReader( iss );
        std::unique_ptr< Geometry<K> > result( wktReader.read() );
        char extra;
        if ( iss >> extra ) {
            std::string remaining( s.substr( int(iss.tellg()) - 1 ) );
            throw WktParseException( "Extra characters in WKT: " + remaining );
        }
        return result;
    }

    /**
     * Read a WKT geometry from a char*
     */
    template < typename K >
    std::unique_ptr< Geometry<K> > readWkt( const char* str, size_t len)
    {
        CharArrayBuffer buf( str, str + len );
        std::istream istr( &buf );
        detail::WktReader<K> wktReader( istr );
        std::unique_ptr< Geometry<K> > result( wktReader.read() );
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
