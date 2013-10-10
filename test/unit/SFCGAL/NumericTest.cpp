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
#include <boost/test/unit_test.hpp>

#include <exception>

#include <SFCGAL/numeric.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_NumericTest )

BOOST_AUTO_TEST_CASE( testNaNAndIsNaN )
{
    BOOST_CHECK( NaN() != NaN() );
    BOOST_CHECK( ! isNaN( 0.0 ) );
    BOOST_CHECK( isNaN( NaN() ) );
}

BOOST_AUTO_TEST_CASE( testFloorRational )
{
    BOOST_CHECK_EQUAL( SFCGAL::floor( CGAL::Gmpq( 0 ) ), 0 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::floor( CGAL::Gmpq( 1,2 ) ), 0 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::floor( CGAL::Gmpq( 1,3 ) ), 0 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::floor( CGAL::Gmpq( 2,3 ) ), 0 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::floor( CGAL::Gmpq( 1,1 ) ), 1 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::floor( CGAL::Gmpq( 4,3 ) ), 1 ) ;
}

BOOST_AUTO_TEST_CASE( testCeilRational )
{
    BOOST_CHECK_EQUAL( SFCGAL::ceil( CGAL::Gmpq( 0 ) ), 0 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::ceil( CGAL::Gmpq( 1,2 ) ), 1 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::ceil( CGAL::Gmpq( 1,3 ) ), 1 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::ceil( CGAL::Gmpq( 1,1 ) ), 1 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::ceil( CGAL::Gmpq( 4,3 ) ), 2 ) ;
}

BOOST_AUTO_TEST_CASE( testRoundRational )
{
    BOOST_CHECK_EQUAL( SFCGAL::round( CGAL::Gmpq( 0 ) ), 0 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::round( CGAL::Gmpq( 1,2 ) ), 1 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::round( CGAL::Gmpq( 1,3 ) ), 0 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::round( CGAL::Gmpq( 1,1 ) ), 1 ) ;
    BOOST_CHECK_EQUAL( SFCGAL::round( CGAL::Gmpq( 4,3 ) ), 1 ) ;
}

BOOST_AUTO_TEST_SUITE_END()




