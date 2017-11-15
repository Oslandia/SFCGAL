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

#if CGAL_USE_GMPXX
SFCGAL_API ::mpz_class myfloor( const ::mpq_class& v ) {
    ::mpz_class q;
    mpz_fdiv_q(q.get_mpz_t(), v.get_num().get_mpz_t(), v.get_den().get_mpz_t());
    return q;
}
SFCGAL_API ::mpz_class floor( const ::mpq_class& v ) {
    return myfloor(v);
}

/**
 * @brief ceil a rational to an integer
 */
SFCGAL_API ::mpz_class myceil( const ::mpq_class& v ) {
    ::mpz_class q;
    mpz_cdiv_q(q.get_mpz_t(), v.get_num().get_mpz_t(), v.get_den().get_mpz_t());
    return q;
}
SFCGAL_API ::mpz_class ceil( const ::mpq_class& v ) {
    return myceil(v);
}

/**
 * @brief round a rational to an integer
 */
SFCGAL_API ::mpz_class round( const ::mpq_class& v ) {
    if ( v < 0 ) {
        return myceil( v - ::mpq_class( 1,2 ) );
    }
    else if ( v == 0 ) {
        return 0 ;
    }
    else {
        return myfloor( v + ::mpq_class( 1,2 ) );
    }
}
#endif


}//SFCGAL


