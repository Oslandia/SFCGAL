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

#include <boost/test/unit_test.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Plane_3.h>

#include <cmath>


// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( CGAL_GmpqTest )

/*
 * check that CGAL::Gmpq is flooring results
 */
BOOST_AUTO_TEST_CASE( testPositiveRationalFloor )
{
    // 1 / 3 => 0
    {
        CGAL::Gmpq q( 1,3 );
        BOOST_CHECK_EQUAL( q.numerator() / q.denominator(), 0 );
    }
    // 2 / 3 => 0
    {
        CGAL::Gmpq q( 2,3 );
        BOOST_CHECK_EQUAL( q.numerator() / q.denominator(), 0 );
    }
    // 2 / 2 => 1
    {
        CGAL::Gmpq q( 2,2 );
        BOOST_CHECK_EQUAL( q.numerator() / q.denominator(), 1 );
    }
    // 3 / 2 => 1
    {
        CGAL::Gmpq q( 3,2 );
        BOOST_CHECK_EQUAL( q.numerator() / q.denominator(), 1 );
    }
}

/*
 * check that CGAL::Gmpq is flooring results
 */
BOOST_AUTO_TEST_CASE( testNegativeRationalFloor )
{
    // - 1 / 3 => 0
    {
        CGAL::Gmpq q( -1,3 );
        BOOST_CHECK_EQUAL( q.numerator() / q.denominator(), 0 );
    }
    // - 2 / 3 => 0
    {
        CGAL::Gmpq q( -2,3 );
        BOOST_CHECK_EQUAL( q.numerator() / q.denominator(), 0 );
    }
    // - 2 / 2 => 1
    {
        CGAL::Gmpq q( -2,2 );
        BOOST_CHECK_EQUAL( q.numerator() / q.denominator(), -1 );
    }
    // - 3 / 2 => 1
    {
        CGAL::Gmpq q( -3,2 );
        BOOST_CHECK_EQUAL( q.numerator() / q.denominator(), -1 );
    }
}


BOOST_AUTO_TEST_SUITE_END()

