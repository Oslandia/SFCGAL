/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
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
#include <SFCGAL/numeric.h>

namespace SFCGAL {

///
///
///
CGAL::Gmpz floor( const CGAL::Gmpq& v )
{
    return v.numerator() / v.denominator() ;
}

///
///
///
CGAL::Gmpz ceil( const CGAL::Gmpq& v )
{
    CGAL::Gmpz result( 0 ) ;
    mpz_cdiv_q( result.mpz(), v.numerator().mpz(), v.denominator().mpz() ) ;
    return result ;
}

///
///
///
CGAL::Gmpz round( const CGAL::Gmpq& v )
{
    if ( v < 0 ) {
        //ceil( v - 0.5 ) ;
        return ceil( v - CGAL::Gmpq( 1,2 ) );
    }
    else if ( v == 0 ) {
        return 0 ;
    }
    else {
        //floor( v + 0.5 ) ;
        return floor( v + CGAL::Gmpq( 1,2 ) );
    }
}

}//SFCGAL


