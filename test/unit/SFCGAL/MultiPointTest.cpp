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
#include <SFCGAL/MultiPoint.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_MultiPointTest )

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
    MultiPoint g;
    BOOST_CHECK( g.isEmpty() ) ;
    BOOST_CHECK( ! g.is3D() );
    BOOST_CHECK_EQUAL( g.numGeometries(), 0U );
}

BOOST_AUTO_TEST_CASE( testGeometryTypeId )
{
    MultiPoint g;
    BOOST_CHECK_EQUAL( g.geometryTypeId(), TYPE_MULTIPOINT );
}

//-- addAllowedGeometry
BOOST_AUTO_TEST_CASE( addPoint )
{
    MultiPoint g;
    g.addGeometry( new Point( 2.0, 3.0 ) );
    BOOST_CHECK_EQUAL( g.numGeometries(), 1U );
}
//-- addForbidenGeometry
BOOST_AUTO_TEST_CASE( addLineStringThrow )
{
    MultiPoint g;
    BOOST_CHECK_THROW( g.addGeometry( LineString() ), std::exception );
}

//-- asText

BOOST_AUTO_TEST_CASE( asTextEmpty )
{
    MultiPoint g;
    BOOST_CHECK_EQUAL( g.asText( 1 ), "MULTIPOINT EMPTY" );
}

BOOST_AUTO_TEST_CASE( asText2d )
{
    MultiPoint g;
    g.addGeometry( Point( 2.0,3.0 ) );
    g.addGeometry( Point( 3.0,4.0 ) );
    BOOST_CHECK_EQUAL( g.asText( 3 ), "MULTIPOINT((2.000 3.000),(3.000 4.000))" );
}

//-- is< T >

BOOST_AUTO_TEST_CASE( isGeometryCollection )
{
    MultiPoint g;
    BOOST_CHECK( g.is< GeometryCollection >() );
}

BOOST_AUTO_TEST_CASE( isMultiPoint )
{
    MultiPoint g;
    BOOST_CHECK( g.is< MultiPoint >() );
}

BOOST_AUTO_TEST_SUITE_END()




