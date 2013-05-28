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
#include <boost/test/unit_test.hpp>

#include <exception>

#include <SFCGAL/Envelope.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/MultiPolygon.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_MultiPolygonTest )

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
	MultiPolygon g;
	BOOST_CHECK( g.isEmpty() ) ;
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.numGeometries(), 0U );
}

BOOST_AUTO_TEST_CASE( testGeometryTypeId ){
	MultiPolygon g;
	BOOST_CHECK_EQUAL( g.geometryTypeId(), TYPE_MULTIPOLYGON );
}

//-- addAllowedGeometry
BOOST_AUTO_TEST_CASE( addPolygon )
{
	MultiPolygon g;
	g.addGeometry( new Polygon() );
	BOOST_CHECK_EQUAL( g.numGeometries(), 1U );
}
//-- addForbidenGeometry
BOOST_AUTO_TEST_CASE( addLineStringThrow )
{
	MultiPolygon g;
	BOOST_CHECK_THROW( g.addGeometry( LineString() ), std::exception );
}

//-- asText

BOOST_AUTO_TEST_CASE( asTextEmpty )
{
	MultiPolygon g;
	BOOST_CHECK_EQUAL( g.asText(1), "MULTIPOLYGON EMPTY" );
}
BOOST_AUTO_TEST_CASE( asText2d )
{
	MultiPolygon g;
	g.addGeometry( Envelope(0.0,1.0,0.0,1.0).toPolygon().release() );
	g.addGeometry( Envelope(2.0,3.0,4.0,5.0).toPolygon().release() );
	BOOST_CHECK_EQUAL( g.asText(3), "MULTIPOLYGON(((0.000 0.000,1.000 0.000,1.000 1.000,0.000 1.000,0.000 0.000)),((2.000 4.000,3.000 4.000,3.000 5.000,2.000 5.000,2.000 4.000)))" );
}

//-- is< T >

BOOST_AUTO_TEST_CASE( isGeometryCollection )
{
	MultiPolygon g;
	BOOST_CHECK( g.is< GeometryCollection >() );
}

BOOST_AUTO_TEST_CASE( isMultiPolygon )
{
	MultiPolygon g;
	BOOST_CHECK( g.is< MultiPolygon >() );
}

BOOST_AUTO_TEST_SUITE_END()




