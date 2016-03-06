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
#include <memory>
#include <string>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;
using namespace SFCGAL::io ;


BOOST_AUTO_TEST_SUITE( SFCGAL_io_WktWriterTest )

//-- WKT POINT

BOOST_AUTO_TEST_CASE( pointAsTextEmpty )
{
    Point<Epeck> g;
    BOOST_CHECK_EQUAL( toWkt<Epeck>( g ), "POINT EMPTY" );
}
BOOST_AUTO_TEST_CASE( pointAsText2d )
{
    Point<Epeck> g( 2.0,3.0 );
    BOOST_CHECK_EQUAL( toWkt<Epeck>( g ), "POINT(2.000 3.000)" );
}
BOOST_AUTO_TEST_CASE( pointAsText3d )
{
    Point<Epeck> g( 2.0,3.0,4.0 );
    BOOST_CHECK_EQUAL( toWkt<Epeck>( g ), "POINT(2.000 3.000 4.000)" );
}


//-- WKT MultiPoint


BOOST_AUTO_TEST_CASE( asTextEmpty )
{
    MultiPoint<Epeck> g;
    BOOST_CHECK_EQUAL( g.asText( 1 ), "MULTIPOINT EMPTY" );
}

BOOST_AUTO_TEST_CASE( asText2d )
{
    MultiPoint<Epeck> g;
    g.addGeometry( Point( 2.0,3.0 ) );
    g.addGeometry( Point( 3.0,4.0 ) );
    BOOST_CHECK_EQUAL( g.asText( 3 ), "MULTIPOINT((2.000 3.000),(3.000 4.000))" );
}


BOOST_AUTO_TEST_SUITE_END()



