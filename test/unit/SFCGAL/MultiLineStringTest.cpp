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

#include <SFCGAL/LineString.h>
#include <SFCGAL/MultiLineString.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_MultiLineStringTest )

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
    MultiLineString g;
    BOOST_CHECK( g.isEmpty() ) ;
    BOOST_CHECK( ! g.is3D() );
    BOOST_CHECK_EQUAL( g.numGeometries(), 0U );
}


//-- addAllowedGeometry
BOOST_AUTO_TEST_CASE( addLineString )
{
    MultiLineString g;
    g.addGeometry( new LineString() );
    BOOST_CHECK_EQUAL( g.numGeometries(), 1U );
}
//-- addForbidenGeometry
BOOST_AUTO_TEST_CASE( addLineStringThrow )
{
    MultiLineString g;
    BOOST_CHECK_THROW( g.addGeometry( Point() ), std::exception );
}

//-- asText

BOOST_AUTO_TEST_CASE( asTextEmpty )
{
    MultiLineString g;
    BOOST_CHECK_EQUAL( g.asText( 1 ), "MULTILINESTRING EMPTY" );
}

BOOST_AUTO_TEST_CASE( asText2d )
{
    MultiLineString g;
    g.addGeometry( LineString( Point( 0.0,0.0 ), Point( 1.0,1.0 ) ) );
    g.addGeometry( LineString( Point( 1.0,1.0 ), Point( 2.0,2.0 ) ) );
    BOOST_CHECK_EQUAL( g.asText( 1 ), "MULTILINESTRING((0.0 0.0,1.0 1.0),(1.0 1.0,2.0 2.0))" );
}

//-- is< T >

BOOST_AUTO_TEST_CASE( isGeometryCollection )
{
    MultiLineString g;
    BOOST_CHECK( g.is< GeometryCollection >() );
}

BOOST_AUTO_TEST_CASE( isMultiLineString )
{
    MultiLineString g;
    BOOST_CHECK( g.is< MultiLineString >() );
}

BOOST_AUTO_TEST_SUITE_END()




