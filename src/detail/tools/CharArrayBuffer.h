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

#ifndef _SFCGAL_TOOLS_CHARARRAYBUFFER_H_
#define _SFCGAL_TOOLS_CHARARRAYBUFFER_H_

#include <SFCGAL/config.h>

#include <streambuf>


///
/// Streambuf on a char*
///
/// http://www.mr-edd.co.uk/blog/beginners_guide_streambuf
///
class SFCGAL_API CharArrayBuffer : public std::streambuf {
public:
    CharArrayBuffer( const char* begin, const char* end );
    explicit CharArrayBuffer( const char* str );

private:
    int_type underflow();
    int_type uflow();
    int_type pbackfail( int_type ch );
    std::streamsize showmanyc();

    // copy ctor and assignment not implemented;
    // copying not allowed
    CharArrayBuffer( const CharArrayBuffer& );
    CharArrayBuffer& operator= ( const CharArrayBuffer& );

    std::streampos seekpos( std::streampos pos, std::ios_base::openmode );
    std::streampos seekoff ( std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode );
private:
    const char* const begin_;
    const char* const end_;
    const char* current_;
};


#endif

