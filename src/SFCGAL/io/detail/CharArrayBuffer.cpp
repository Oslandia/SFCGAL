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

#include <iostream>

#include <SFCGAL/io/detail/CharArrayBuffer.h>

#include <cstring>

CharArrayBuffer::CharArrayBuffer( const char* begin, const char* end ) :
    begin_( begin ),
    end_( end ),
    current_( begin_ )
{
}

CharArrayBuffer::CharArrayBuffer( const char* str ) :
    begin_( str ),
    end_( begin_ + std::strlen( str ) ),
    current_( begin_ )
{
}


std::streampos CharArrayBuffer::seekoff( std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode )
{
    if ( way == std::ios_base::cur ) {
        if ( current_ + off < end_ ) {
            current_ += off;
        }
        else {
            return -1;
        }
    }
    else if ( way == std::ios_base::beg ) {
        if ( begin_ + off < end_ ) {
            current_ = begin_ + off;
        }
        else {
            return -1;
        }
    }

    return current_ - begin_;
}

std::streampos CharArrayBuffer::seekpos( std::streampos pos, std::ios_base::openmode )
{
    if ( begin_ + pos >= end_ ) {
        return -1;
    }

    current_ = begin_ + pos;
    return current_ - begin_;
}

CharArrayBuffer::int_type CharArrayBuffer::underflow()
{
    if ( current_ == end_ ) {
        return traits_type::eof();
    }

    return traits_type::to_int_type( *current_ );
}

CharArrayBuffer::int_type CharArrayBuffer::uflow()
{
    if ( current_ == end_ ) {
        return traits_type::eof();
    }

    return traits_type::to_int_type( *current_++ );
}

CharArrayBuffer::int_type CharArrayBuffer::pbackfail( int_type ch )
{
    if ( current_ == begin_ || ( ch != traits_type::eof() && ch != current_[-1] ) ) {
        return traits_type::eof();
    }

    return traits_type::to_int_type( *--current_ );
}

std::streamsize CharArrayBuffer::showmanyc()
{
    return end_ - current_;
}
