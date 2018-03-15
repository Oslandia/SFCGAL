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

#ifdef CGAL_USE_GMPXX
///
///
///
mpz_class floor( const mpq_class& v )
{
    return v.get_num() / v.get_den() ;
}

///
///
///
mpz_class ceil( const mpq_class& v )
{
    mpz_class result( 0 ) ;
    mpz_cdiv_q( result.get_mpz_t(), v.get_num().get_mpz_t(), v.get_den().get_mpz_t() ) ;
    return result ;
}

///
///
///
mpz_class round( const mpq_class& v )
{
    if ( v < 0 ) {
        //ceil( v - 0.5 ) ;
        mpq_class tmp = v - mpq_class( 1,2 );
        return ceil( tmp );
    }
    else if ( v == 0 ) {
        return 0 ;
    }
    else {
        //floor( v + 0.5 ) ;
        mpq_class tmp = v + mpq_class( 1,2 );
        return floor( tmp );
    }
}
#endif



}//SFCGAL


