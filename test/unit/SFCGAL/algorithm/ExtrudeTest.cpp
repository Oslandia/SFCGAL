#include <boost/test/unit_test.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/extrude.h>
#include <SFCGAL/transform/ForceZ.h>

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

