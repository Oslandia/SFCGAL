/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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

#include <SFCGAL/Polygon.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_PolygonTest )

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
	Polygon g;
	BOOST_CHECK( g.isEmpty() );
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.numInteriorRings(), 0U );
}

BOOST_AUTO_TEST_CASE( exteriorRingConstructor )
{
	LineString exteriorRing ;
	exteriorRing.addPoint( Point(0.0,0.0) );
	exteriorRing.addPoint( Point(1.0,0.0) );
	exteriorRing.addPoint( Point(1.0,1.0) );
	exteriorRing.addPoint( Point(0.0,1.0) );
	exteriorRing.addPoint( Point(0.0,0.0) );

	Polygon g( exteriorRing );
	BOOST_CHECK( ! g.isEmpty() );
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.numInteriorRings(), 0U );
	BOOST_CHECK_EQUAL( g.exteriorRing().numPoints(), 5U );
}

BOOST_AUTO_TEST_CASE( testClone )
{
	LineString exteriorRing ;
	exteriorRing.addPoint( Point(0.0,0.0) );
	exteriorRing.addPoint( Point(1.0,0.0) );
	exteriorRing.addPoint( Point(1.0,1.0) );
	exteriorRing.addPoint( Point(0.0,1.0) );
	exteriorRing.addPoint( Point(0.0,0.0) );

	Polygon g( exteriorRing );
	std::auto_ptr< Polygon > copy( g.clone() );

	BOOST_CHECK( ! copy->isEmpty() );
	BOOST_CHECK( ! copy->is3D() );
	BOOST_CHECK_EQUAL( copy->numInteriorRings(), 0U );
	BOOST_CHECK_EQUAL( copy->exteriorRing().numPoints(), 5U );
}



BOOST_AUTO_TEST_CASE( testPolygon3d )
{
	Polygon g;
	g.exteriorRing().addPoint( Point(0.0,0.0,2.0) );
	g.exteriorRing().addPoint( Point(1.0,0.0,2.0) );
	g.exteriorRing().addPoint( Point(1.0,1.0,2.0) );
	g.exteriorRing().addPoint( Point(0.0,1.0,2.0) );
	g.exteriorRing().addPoint( Point(0.0,0.0,2.0) );

	BOOST_CHECK( ! g.isEmpty() );
	BOOST_CHECK( g.is3D() );
	BOOST_CHECK_EQUAL( g.numInteriorRings(), 0U );
}



BOOST_AUTO_TEST_CASE( asTextEmpty )
{
	Polygon g;
	BOOST_CHECK_EQUAL( g.asText(1), "POLYGON EMPTY" );
}


BOOST_AUTO_TEST_CASE( asText2d )
{
	Polygon g;
	g.exteriorRing().addPoint( Point(0.0,0.0) );
	g.exteriorRing().addPoint( Point(1.0,0.0) );
	g.exteriorRing().addPoint( Point(1.0,1.0) );
	g.exteriorRing().addPoint( Point(0.0,1.0) );
	g.exteriorRing().addPoint( Point(0.0,0.0) );

	BOOST_CHECK_EQUAL( g.asText(1), "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0))" );
}



BOOST_AUTO_TEST_CASE( asText3d )
{
	Polygon g;
	g.exteriorRing().addPoint( Point(0.0,0.0,2.0) );
	g.exteriorRing().addPoint( Point(1.0,0.0,2.0) );
	g.exteriorRing().addPoint( Point(1.0,1.0,2.0) );
	g.exteriorRing().addPoint( Point(0.0,1.0,2.0) );
	g.exteriorRing().addPoint( Point(0.0,0.0,2.0) );

	BOOST_CHECK_EQUAL( g.asText(1), "POLYGON((0.0 0.0 2.0,1.0 0.0 2.0,1.0 1.0 2.0,0.0 1.0 2.0,0.0 0.0 2.0))" );
}


BOOST_AUTO_TEST_CASE( isPolygon )
{
	Polygon g;
	BOOST_CHECK( g.is< Polygon >() );
}

BOOST_AUTO_TEST_SUITE_END()




