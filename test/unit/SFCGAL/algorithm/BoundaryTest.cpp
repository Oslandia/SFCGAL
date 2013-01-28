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

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_BoundaryTest )

//-- Point

BOOST_AUTO_TEST_CASE( emptyPoint )
{
	Point g;
	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->is< GeometryCollection >() );
	BOOST_CHECK( boundary->isEmpty() );
}
BOOST_AUTO_TEST_CASE( pointXY )
{
	Point g(2.0,3.0);
	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->is< GeometryCollection >() );
	BOOST_CHECK( boundary->isEmpty() );
}

//-- LineString

BOOST_AUTO_TEST_CASE( emptyLineString )
{
	LineString g;
	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->is< GeometryCollection >() );
	BOOST_CHECK( boundary->isEmpty() );
}

BOOST_AUTO_TEST_CASE( lineStringSegment )
{
	BOOST_TEST_MESSAGE( "check that a LineSegment has a MultiPoint boundary composed of 2 points" );

	LineString g;
	g.addPoint( Point(0.0,0.0) );
	g.addPoint( Point(1.0,1.0) );
	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->is< MultiPoint >() );
	BOOST_CHECK_EQUAL( boundary->as< MultiPoint >().numGeometries(), 2U );
}

BOOST_AUTO_TEST_CASE( lineStringWithThreePoints )
{
	BOOST_TEST_MESSAGE( "check that a LineString with 3 points has a MultiPoint boundary composed of 2 points" );

	LineString g;
	g.addPoint( Point(0.0,0.0) );
	g.addPoint( Point(1.0,1.0) );
	g.addPoint( Point(2.0,2.0) );
	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->is< MultiPoint >() );
	BOOST_CHECK_EQUAL( boundary->as< MultiPoint >().numGeometries(), 2U );
}

BOOST_AUTO_TEST_CASE( lineStringClosed )
{
	BOOST_TEST_MESSAGE( "check that a closed LineString has empty boundary" );

	LineString g;
	g.addPoint( Point(0.0,0.0) );
	g.addPoint( Point(1.0,1.0) );
	g.addPoint( Point(2.0,2.0) );
	g.addPoint( Point(0.0,0.0) );

	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->isEmpty() );
	BOOST_CHECK( boundary->is< GeometryCollection >() );
}

//-- MultiLineString

BOOST_AUTO_TEST_CASE( multiLineStringSimple )
{
	BOOST_TEST_MESSAGE( "check common point between two LineStrings" );

	MultiLineString g ;
	g.addGeometry( LineString( Point(0.0,0.0), Point(1.0,1.0) ) );
	g.addGeometry( LineString( Point(0.0,0.0), Point(1.0,2.0) ) );

	g.addGeometry( LineString( Point(1.0,1.0), Point(1.0,2.0) ) );

	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->isEmpty() );
	BOOST_CHECK( boundary->is< GeometryCollection >() );
}



//-- MultiPolygon

//TODO TDD

BOOST_AUTO_TEST_CASE( multiPolygonSimple )
{
	BOOST_TEST_MESSAGE( "check common point between two LineStrings" );

	MultiPolygon g ;
	{
		LineString ring;
		ring.addPoint( Point( 0.0, 0.0 ) );
		ring.addPoint( Point( 1.0, 0.0 ) );
		ring.addPoint( Point( 1.0, 1.0 ) );
		ring.addPoint( Point( 0.0, 1.0 ) );
		ring.addPoint( Point( 0.0, 0.0 ) );

		g.addGeometry( Polygon( ring ) );
	}
	{
		LineString ring;
		ring.addPoint( Point( 1.0, 0.0 ) );
		ring.addPoint( Point( 2.0, 0.0 ) );
		ring.addPoint( Point( 2.0, 1.0 ) );
		ring.addPoint( Point( 1.0, 1.0 ) );
		ring.addPoint( Point( 1.0, 0.0 ) );

		g.addGeometry( Polygon( ring ) );
	}

	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( ! boundary->isEmpty() );
	BOOST_CHECK( boundary->is< MultiLineString >() );
}



BOOST_AUTO_TEST_SUITE_END()

