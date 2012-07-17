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



BOOST_AUTO_TEST_CASE( testExtrudeWKT )
{
	transform::ForceZ forceZ ;

//	std::string wkt("MULTIPOLYGON(((2.100000 4.500000,4.100000 2.500000,6.600000 0.000000,10.300000 3.800000,12.900000 1.200000,15.700000 2.000000,16.300000 1.400000,19.100000 4.300000,14.600000 8.700000,13.800000 8.000000,11.500000 10.200000,12.400000 11.000000,9.700000 13.700000,5.300000 9.100000,3.100000 11.300000,0.000000 8.100000,2.900000 5.300000,2.100000 4.500000),(1.752842 8.293951,3.013240 9.663949,4.931237 7.800751,3.588639 6.321154,1.752842 8.293951),(6.876634 8.074751,7.972632 9.472149,12.301825 8.293951,11.342827 4.266157,4.684637 4.183957,8.822031 6.512953,6.876634 8.074751),(16.110419 4.978556,14.713022 6.786953,13.014224 6.348554,13.041624 3.581158,16.658419 2.841359,16.110419 4.978556),(9.260430 12.458744,7.589033 10.348947,8.849431 9.746148,11.562027 8.732350,11.973026 9.307749,10.466028 10.129748,11.507227 11.170946,10.109829 12.540944,9.260430 12.458744)))");
	std::string wkt("LINESTRING(0.0 0.0,1.0 0.0,2.0 2.0)");

	std::auto_ptr< Geometry > g( io::readWkt( wkt ) );
	g->accept(forceZ);

	std::auto_ptr< Geometry > extruded( algorithm::extrude( *g, 0.0, 0.0, 10.0 ) ) ;
	std::cout << extruded->asText(1) << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()

