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

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/extrude.h>
#include <SFCGAL/transform/ForceZ.h>
#include <SFCGAL/io/wkt.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_ExtrudeTest )


BOOST_AUTO_TEST_CASE( testExtrudePoint )
{
	Point g(0.0,0.0,0.0);
	std::auto_ptr< Geometry > ext( algorithm::extrude( g, 0.0, 0.0, 1.0 ) );
	BOOST_CHECK( ext->is< LineString >() );
	BOOST_CHECK( ext->as< LineString >().is3D() );
	BOOST_CHECK_EQUAL( ext->asText(1), "LINESTRING(0.0 0.0 0.0,0.0 0.0 1.0)" );
}


BOOST_AUTO_TEST_CASE( testExtrudeLineString )
{
	LineString g(
		Point(0.0,0.0,0.0),
		Point(1.0,0.0,0.0)
	);
	std::auto_ptr< Geometry > ext( algorithm::extrude( g, 0.0, 0.0, 1.0 ) );
	BOOST_CHECK( ext->is< PolyhedralSurface >() );
	BOOST_CHECK( ext->as< PolyhedralSurface >().is3D() );
	BOOST_CHECK_EQUAL( ext->asText(1), "POLYHEDRALSURFACE(((0.0 0.0 0.0,1.0 0.0 0.0,1.0 0.0 1.0,0.0 0.0 1.0,0.0 0.0 0.0)))" );
}



BOOST_AUTO_TEST_CASE( testExtrudeSquare )
{
	std::vector< Point > points;
	points.push_back( Point(0.0,0.0,0.0) );
	points.push_back( Point(1.0,0.0,0.0) );
	points.push_back( Point(1.0,1.0,0.0) );
	points.push_back( Point(0.0,1.0,0.0) );
	points.push_back( Point(0.0,0.0,0.0) );

	LineString exteriorRing( points ) ;
	Polygon g( exteriorRing );
	std::auto_ptr< Geometry > ext( algorithm::extrude( g, 0.0, 0.0, 1.0 ) );
	BOOST_CHECK( ext->is< Solid >() );
	BOOST_CHECK_EQUAL( ext->as< Solid >().numShells(), 1U );
	BOOST_CHECK_EQUAL( ext->as< Solid >().exteriorShell().numPolygons(), 6U );
}

BOOST_AUTO_TEST_CASE( testExtrudePolyhedral )
{
	std::auto_ptr<Geometry> g = io::readWkt( "POLYHEDRALSURFACE(((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)))" );

	std::auto_ptr< Geometry > ext = algorithm::extrude( *g, 0.0, 0.0, 1.0 );
	BOOST_CHECK( ext->is< Solid >() );
	BOOST_CHECK_EQUAL( ext->as< Solid >().numShells(), 1U );
}

BOOST_AUTO_TEST_CASE( testExtrudeMultiPolygon )
{
	std::vector< Point > points;
	points.push_back( Point(0.0,0.0,0.0) );
	points.push_back( Point(1.0,0.0,0.0) );
	points.push_back( Point(1.0,1.0,0.0) );
	points.push_back( Point(0.0,1.0,0.0) );
	points.push_back( Point(0.0,0.0,0.0) );

	std::vector< Point > points2;
	points2.push_back( Point(1.0,0.0,0.0) );
	points2.push_back( Point(2.0,0.0,0.0) );
	points2.push_back( Point(2.0,1.0,0.0) );
	points2.push_back( Point(1.0,1.0,0.0) );
	points2.push_back( Point(1.0,0.0,0.0) );

	LineString exteriorRing( points ) ;
	LineString exteriorRing2( points2 ) ;
	Polygon g1( exteriorRing );
	Polygon g2( exteriorRing2 );
	MultiPolygon mp;
	mp.addGeometry( g1 );
	mp.addGeometry( g2 );

	std::auto_ptr< Geometry > ext( algorithm::extrude( mp, 0.0, 0.0, 1.0 ) );
	BOOST_CHECK( ext->is< MultiSolid >() );
	BOOST_CHECK_EQUAL( ext->as<MultiSolid>().numGeometries(), 2U );
}


BOOST_AUTO_TEST_CASE( testExtrudeSquareWithHole )
{
	std::vector< LineString > rings;
	{
		std::vector< Point > points;
		points.push_back( Point(0.0,0.0,0.0) );
		points.push_back( Point(1.0,0.0,0.0) );
		points.push_back( Point(1.0,1.0,0.0) );
		points.push_back( Point(0.0,1.0,0.0) );
		points.push_back( Point(0.0,0.0,0.0) );
		rings.push_back( LineString(points) );
	}
	{
		std::vector< Point > points;
		points.push_back( Point(0.2,0.2,0.0) );
		points.push_back( Point(0.8,0.2,0.0) );
		points.push_back( Point(0.8,0.8,0.0) );
		points.push_back( Point(0.2,0.8,0.0) );
		points.push_back( Point(0.2,0.2,0.0) );

		std::reverse( points.begin(), points.end() );

		rings.push_back( LineString(points) );
	}

	Polygon g( rings );
	std::auto_ptr< Geometry > ext( algorithm::extrude( g, 0.0, 0.0, 1.0 ) );
	BOOST_CHECK( ext->is< Solid >() );
	BOOST_CHECK_EQUAL( ext->as< Solid >().numShells(), 1U );
	BOOST_CHECK_EQUAL( ext->as< Solid >().exteriorShell().numPolygons(), 10U );
}



BOOST_AUTO_TEST_SUITE_END()

