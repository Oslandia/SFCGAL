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
#include <SFCGAL/algorithm/distance.h>

#include <SFCGAL/tools/Registry.h>
#include <SFCGAL/tools/Log.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_DistanceTest )

/*
 * check that distance between empty points is infinity
 */
BOOST_AUTO_TEST_CASE( testDistanceBetweenEmptyPointsIsInfinity )
{
	BOOST_CHECK_EQUAL( Point().distance( Point() ), std::numeric_limits< double >::infinity() );
}

//TODO enable when implement is complete
#if 0

/*
 * check that distance between all kinds of empty geometry is infinity
 */
BOOST_AUTO_TEST_CASE( testDistanceBetweenEmptyGeometriesIsDefined )
{
	tools::Registry & registry = tools::Registry::instance() ;

	std::vector< std::string > geometryTypes = tools::Registry::instance().getGeometryTypes() ;
	for ( size_t i = 0; i < geometryTypes.size(); i++ ){
		for ( size_t j = 0; j < geometryTypes.size(); j++ ){
			BOOST_TEST_MESSAGE( boost::format("distance(%s,%s)") % geometryTypes[i] % geometryTypes[j] );

			std::auto_ptr< Geometry > gA( registry.newGeometryByTypeName( geometryTypes[i] ) );
			std::auto_ptr< Geometry > gB( registry.newGeometryByTypeName( geometryTypes[j] ) );

			double dAB ;
			BOOST_CHECK_NO_THROW( dAB = gA->distance( *gB ) ) ;
			BOOST_CHECK_EQUAL( dAB, std::numeric_limits< double >::infinity() );
		}
	}
}
/*
 * check that distance3D between all kinds of empty geometry is infinity
 */
BOOST_AUTO_TEST_CASE( testDistance3DBetweenEmptyGeometriesIsDefined )
{
	tools::Registry & registry = tools::Registry::instance() ;

	std::vector< std::string > geometryTypes = tools::Registry::instance().getGeometryTypes() ;
	for ( size_t i = 0; i < geometryTypes.size(); i++ ){
		for ( size_t j = 0; j < geometryTypes.size(); j++ ){
			BOOST_TEST_MESSAGE( boost::format("distance3D(%s,%s)") % geometryTypes[i] % geometryTypes[j] );

			std::auto_ptr< Geometry > gA( registry.newGeometryByTypeName( geometryTypes[i] ) );
			std::auto_ptr< Geometry > gB( registry.newGeometryByTypeName( geometryTypes[j] ) );

			double dAB ;
			BOOST_CHECK_NO_THROW( dAB = gA->distance3D( *gB ) ) ;
			BOOST_CHECK_EQUAL( dAB, std::numeric_limits< double >::infinity() );
		}
	}
}

#endif


BOOST_AUTO_TEST_CASE( testDistancePointPoint )
{
	BOOST_CHECK_EQUAL( Point(0.0,0.0).distance( Point(0.0,0.0) ), 0.0 );
	BOOST_CHECK_EQUAL( Point(1.0,1.0).distance( Point(4.0,5.0) ), 5.0 );
}
BOOST_AUTO_TEST_CASE( testDistancePointPoint3D )
{
	BOOST_CHECK_EQUAL( Point(0.0,0.0,0.0).distance3D( Point(0.0,0.0,0.0) ), 0.0 );
	BOOST_CHECK_EQUAL( Point(1.0,1.0,1.0).distance3D( Point(4.0,1.0,5.0) ), 5.0 );
}


//testPointLineString
BOOST_AUTO_TEST_CASE( testDistancePointLineString_pointOnLineString )
{
	Point point(1.0,1.0);
	LineString lineString(
		Point(0.0,0.0),
		Point(2.0,2.0)
	);
	BOOST_CHECK_EQUAL( point.distance( lineString ), 0.0 );
}
BOOST_AUTO_TEST_CASE( testDistancePointLineString_pointOnLineString_badLineStringDefinition )
{
	Point point(3.0,4.0);
	LineString lineString ;
	lineString.addPoint( Point(0.0,0.0) );
	BOOST_CHECK_EQUAL( point.distance( lineString ), std::numeric_limits< double >::infinity() );
}
BOOST_AUTO_TEST_CASE( testDistancePointLineString_pointOnLineString_collapsedSegments )
{
	Point point(3.0,4.0);
	LineString lineString ;
	lineString.addPoint( Point(0.0,0.0) );
	lineString.addPoint( Point(0.0,0.0) );
	BOOST_CHECK_EQUAL( point.distance( lineString ), 5.0 );
}



BOOST_AUTO_TEST_CASE( testDistancePointLineString_pointOutOfLineString )
{
	Point point(0.0,1.0);
	LineString lineString(
		Point(0.0,0.0),
		Point(2.0,2.0)
	);
	BOOST_CHECK_EQUAL( point.distance(lineString), sqrt(2.0)/2.0 );
}

//testPointPolygon
BOOST_AUTO_TEST_CASE( testDistancePointPolygon_pointInPolygon )
{
	std::auto_ptr< Geometry > gA( io::readWkt("POINT(0.5 0.5)") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0))") );
	BOOST_CHECK_EQUAL( gA->distance( *gB ), 0.0 );
}
BOOST_AUTO_TEST_CASE( testDistancePointPolygon_pointOutOfPolygon )
{
	std::auto_ptr< Geometry > gA( io::readWkt("POINT(0.0 1.0)") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((0.0 0.0,2.0 2.0,2.0 0.0,0.0 0.0))") );
	BOOST_CHECK_EQUAL( gA->distance( *gB ), sqrt(2.0)/2.0 );
}


// LineString / Triangle
BOOST_AUTO_TEST_CASE( testDistance3DLineStringTriangle_lineStringInTriangle )
{
	std::auto_ptr< Geometry > gA( io::readWkt("LINESTRING(-1.0 0.0 1.0,1.0 0.0 1.0)") );
	std::auto_ptr< Geometry > gB( io::readWkt("TRIANGLE((-4.0 0.0 1.0,4.0 0.0 1.0,0.0 4.0 1.0,-4.0 0.0 1.0))") );
	BOOST_CHECK_EQUAL( gA->distance3D( *gB ), 0.0 );
}
BOOST_AUTO_TEST_CASE( testDistance3DLineStringTriangle_lineStringStartPointIsNearest )
{
	std::auto_ptr< Geometry > gA( io::readWkt("LINESTRING(-1.0 0.0 2.0,1.0 0.0 3.0)") );
	std::auto_ptr< Geometry > gB( io::readWkt("TRIANGLE((-4.0 0.0 1.0,4.0 0.0 1.0,0.0 4.0 1.0,-4.0 0.0 1.0))") );
	BOOST_CHECK_EQUAL( gA->distance3D( *gB ), 1.0 );
}

// Triangle / Triangle
BOOST_AUTO_TEST_CASE( testDistance3DTriangleTriangle_contained )
{
	std::auto_ptr< Geometry > gA( io::readWkt("TRIANGLE((-3.0 0.0 1.0,3.0 0.0 1.0,0.0 3.0 1.0,-3.0 0.0 1.0))") );
	std::auto_ptr< Geometry > gB( io::readWkt("TRIANGLE((-4.0 0.0 1.0,4.0 0.0 1.0,0.0 4.0 1.0,-4.0 0.0 1.0))") );
	BOOST_CHECK_EQUAL( gA->distance3D( *gB ), 0.0 );
}
BOOST_AUTO_TEST_CASE( testDistance3DTriangleTriangle_parallel )
{
	std::auto_ptr< Geometry > gA( io::readWkt("TRIANGLE((-3.0 0.0 1.0,3.0 0.0 1.0,0.0 3.0 1.0,-3.0 0.0 1.0))") );
	std::auto_ptr< Geometry > gB( io::readWkt("TRIANGLE((-4.0 0.0 2.0,4.0 0.0 2.0,0.0 4.0 2.0,-4.0 0.0 2.0))") );
	BOOST_CHECK_EQUAL( gA->distance3D( *gB ), 1.0 );
}

// Polygon / Polygon

BOOST_AUTO_TEST_CASE( testDistancePolygonPolygon_disjoint )
{
	std::auto_ptr< Geometry > gA( io::readWkt("POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0))") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((2.0 0.0,3.0 0.0,3.0 1.0,2.0 1.0,2.0 0.0))") );
	BOOST_CHECK_EQUAL( gA->distance( *gB ), 1.0 );
}

BOOST_AUTO_TEST_CASE( testDistanceMultiPointMultiPoint_disjoint )
{
	std::auto_ptr< Geometry > gA( io::readWkt("MULTIPOINT((0.0 0.0),(1.0 0.0),(1.0 1.0),(0.0 1.0)))") );
	std::auto_ptr< Geometry > gB( io::readWkt("MULTIPOINT((8.0 8.0),(4.0 5.0))") );
	BOOST_CHECK_EQUAL( gA->distance( *gB ), 5.0 );
}


BOOST_AUTO_TEST_SUITE_END()

