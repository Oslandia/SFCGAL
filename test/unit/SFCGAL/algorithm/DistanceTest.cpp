#include <boost/test/unit_test.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/distance.h>


using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_DistanceTest )


BOOST_AUTO_TEST_CASE( testDistancePointPoint )
{
	BOOST_CHECK_EQUAL( Point(0.0,0.0).distance( Point(0.0,0.0) ), 0.0 );
	BOOST_CHECK_EQUAL( Point(1.0,1.0).distance( Point(4.0,5.0) ), 5.0 );
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

