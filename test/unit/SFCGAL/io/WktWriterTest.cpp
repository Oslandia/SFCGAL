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

#include <SFCGAL/types.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;
using namespace SFCGAL::io ;


BOOST_AUTO_TEST_SUITE( SFCGAL_io_WktWriterTest )

//TYPE_POINT               = 1,
BOOST_AUTO_TEST_CASE( pointAsTextEmpty )
{
    Point<Epeck> g;
    BOOST_CHECK_EQUAL( toWkt<Epeck>( g ), "POINT EMPTY" );
}
BOOST_AUTO_TEST_CASE( pointAsTextWithoutPrecision )
{
    Point<Epeck> g( 2.0,3.0 );
    BOOST_CHECK_EQUAL( toWkt<Epeck>( g ), "POINT(2 3)" );
}
BOOST_AUTO_TEST_CASE( pointAsText2d )
{
    Point<Epeck> g( 2.0,3.0 );
    BOOST_CHECK_EQUAL( toWkt<Epeck>( g, 3 ), "POINT(2.000 3.000)" );
}
BOOST_AUTO_TEST_CASE( pointAsText3d )
{
    Point<Epeck> g( 2.0,3.0,4.0 );
    BOOST_CHECK_EQUAL( toWkt<Epeck>( g, 3 ), "POINT(2.000 3.000 4.000)" );
}

//TYPE_LINESTRING          = 2,
//std::string          Geometry::asText( const int & numDecimals = -1 ) const ;
BOOST_AUTO_TEST_CASE( lineStringAsTextEmpty )
{
    LineString<Epeck> g;
    BOOST_CHECK_EQUAL( toWkt<Epeck>(g), "LINESTRING EMPTY" );
}
BOOST_AUTO_TEST_CASE( lineStringAsText2d )
{
    LineString<Epeck> g{
        Point<Epeck>( 2.0,3.0 ),
        Point<Epeck>( 4.0,5.0 )
    };
    BOOST_CHECK_EQUAL( toWkt<Epeck>(g,3), "LINESTRING(2.000 3.000,4.000 5.000)" );
}
BOOST_AUTO_TEST_CASE( lineStringAsText3d )
{
    LineString<Epeck> g{
        Point<Epeck>( 2.0,3.0,7.0 ),
        Point<Epeck>( 4.0,5.0,8.0 )
    };
    BOOST_CHECK_EQUAL( toWkt<Epeck>(g,3), "LINESTRING(2.000 3.000 7.000,4.000 5.000 8.000)" );
}


//TYPE_POLYGON             = 3,
BOOST_AUTO_TEST_CASE( polygonAsTextEmpty )
{
    Polygon<Epeck> g;
    BOOST_CHECK_EQUAL( toWkt<Epeck>(g,1), "POLYGON EMPTY" );
}
BOOST_AUTO_TEST_CASE( polygonAsText2d )
{
    Polygon<Epeck> g;
    g.exteriorRing().push_back( Point<Epeck>( 0.0,0.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 1.0,0.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 1.0,1.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 0.0,1.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 0.0,0.0 ) );

    BOOST_CHECK_EQUAL( toWkt<Epeck>(g,1), "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0))" );
}
BOOST_AUTO_TEST_CASE( polygonAsText3d )
{
    Polygon<Epeck> g;
    g.exteriorRing().push_back( Point<Epeck>( 0.0,0.0,2.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 1.0,0.0,2.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 1.0,1.0,2.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 0.0,1.0,2.0 ) );
    g.exteriorRing().push_back( Point<Epeck>( 0.0,0.0,2.0 ) );

    BOOST_CHECK_EQUAL( toWkt<Epeck>(g,1), "POLYGON((0.0 0.0 2.0,1.0 0.0 2.0,1.0 1.0 2.0,0.0 1.0 2.0,0.0 0.0 2.0))" );
}

//TYPE_MULTIPOINT          = 4,
BOOST_AUTO_TEST_CASE( multiPointAsTextEmpty )
{
    MultiPoint<Epeck> g;
    BOOST_CHECK_EQUAL( toWkt<Epeck>( g ), "MULTIPOINT EMPTY" );
}

BOOST_AUTO_TEST_CASE( multiPointAsText2d )
{
    MultiPoint<Epeck> g{
        Point<Epeck>( 2.0,3.0 ),
        Point<Epeck>( 3.0,4.0 )
    };
    BOOST_CHECK_EQUAL( toWkt<Epeck>( g, 3 ), "MULTIPOINT((2.000 3.000),(3.000 4.000))" );
}

//TYPE_MULTILINESTRING     = 5,
//TODO 

//TYPE_MULTIPOLYGON        = 6,
//TODO 

//TYPE_GEOMETRYCOLLECTION  = 7,
//TODO 

//TYPE_POLYHEDRALSURFACE   = 15,
//TODO 

//TYPE_TRIANGULATEDSURFACE = 16,
//TODO 

//TYPE_TRIANGLE            = 100, //17 in Wikipedia???
//TODO 

//TYPE_SOLID               = 101,
//TODO 

//TYPE_MULTISOLID          = 102
//TODO 



BOOST_AUTO_TEST_SUITE_END()



