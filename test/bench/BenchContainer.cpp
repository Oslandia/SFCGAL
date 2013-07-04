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
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include "../test_config.h"

#include "Bench.h"

#include <boost/test/unit_test.hpp>


using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_BenchContainer )

#if 0

//
// Test limit case
BOOST_AUTO_TEST_CASE( testVectorOfPoint2 )
{
    const int N = 100000 ;

    // create points
    bench().start( "std::vector< Kernel::Point_2 > create" ) ;
    std::vector< Kernel::Point_2 > points ;
    points.reserve( N ) ;

    for ( int i = 0; i < N; i++ ) {
        points.push_back( Kernel::Point_2( i,i )  ) ;
    }

    bench().stop();

    // centroid
    bench().start( "std::vector< Kernel::Point_2 > access" ) ;
    Kernel::FT x = 0 , y = 0 ;

    for ( std::vector< Kernel::Point_2 >::const_iterator it = points.begin(); it != points.end(); ++it ) {
        x += it->x() ;
        y += it->y() ;
    }

    Kernel::Point_2 centroid( x / Kernel::FT( ( int )points.size() ), y / Kernel::FT( ( int )points.size() ) );
    bench().stop() ;

    // reverse
    bench().start( "std::vector< Kernel::Point_2 > reverse" ) ;
    std::reverse( points.begin(), points.end() ) ;
    bench().stop();

    // clear
    bench().start( "std::vector< Kernel::Point_2 > clear" ) ;
    points.clear() ;
    bench().stop() ;
}

//
// Test limit case
BOOST_AUTO_TEST_CASE( testVectorOfPoint )
{
    const int N = 100000 ;

    // create points
    bench().start( "std::vector< Point > create" );
    std::vector< Point > points ;
    points.reserve( N ) ;

    for ( int i = 0; i < N; i++ ) {
        points.push_back( Point( i,i )  ) ;
    }

    bench().stop();

    // centroid
    bench().start( "std::vector< Point > access" );
    Kernel::FT x = 0 , y = 0 ;

    for ( std::vector< Point >::const_iterator it = points.begin(); it != points.end(); ++it ) {
        x += it->x() ;
        y += it->y() ;
    }

    Point centroid( x / Kernel::FT( ( int )points.size() ), y / Kernel::FT( ( int )points.size() ) );
    bench().stop();


    // reverse
    bench().start( "std::vector< Point > reverse" ) ;
    std::reverse( points.begin(), points.end() ) ;
    bench().stop() ;

    // clear
    bench().start( "std::vector< Point > clear" ) ;
    points.clear() ;
    bench().stop() ;
}


//
// Test limit case
BOOST_AUTO_TEST_CASE( testPtrVectorOfPoint )
{
    const int N = 100000 ;

    boost::timer::cpu_timer timer;

    // create points
    bench().start( "boost::ptr_vector< Point > create" ) ;
    boost::ptr_vector< Point > points ;
    points.reserve( N ) ;

    for ( int i = 0; i < N; i++ ) {
        points.push_back( new Point( i,i )  ) ;
    }

    bench().stop() ;

    // centroid
    bench().start( "boost::ptr_vector< Point > access" ) ;
    Kernel::FT x = 0 , y = 0 ;

    for ( boost::ptr_vector< Point >::const_iterator it = points.begin(); it != points.end(); ++it ) {
        x += it->x() ;
        y += it->y() ;
    }

    Point centroid( x / Kernel::FT( ( int )points.size() ), y / Kernel::FT( ( int )points.size() ) );
    bench().stop();


    // reverse
    bench().start( "boost::ptr_vector< Point > reverse" );
    std::reverse( points.begin(), points.end() ) ;
    bench().stop();

    // clear
    bench().start( "boost::ptr_vector< Point > clear" );
    points.clear() ;
    bench().stop();
}

#endif

BOOST_AUTO_TEST_SUITE_END()




