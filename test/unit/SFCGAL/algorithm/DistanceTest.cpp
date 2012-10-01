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
	BOOST_CHECK_EQUAL( algorithm::distancePointPoint( Point(0.0,0.0), Point(0.0,0.0) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::distancePointPoint( Point(1.0,1.0), Point(4.0,5.0) ), 5.0 );
}

//testPointLineString
BOOST_AUTO_TEST_CASE( testDistancePointLineString_pointOnLineString )
{
	Point point(1.0,1.0);
	LineString lineString(
		Point(0.0,0.0),
		Point(2.0,2.0)
	);
	BOOST_CHECK_EQUAL( algorithm::distancePointLineString( point, lineString ), 0.0 );
}
BOOST_AUTO_TEST_CASE( testDistancePointLineString_pointOutOfLineString )
{
	Point point(0.0,1.0);
	LineString lineString(
		Point(0.0,0.0),
		Point(2.0,2.0)
	);
	BOOST_CHECK_EQUAL( algorithm::distancePointLineString( point, lineString ), sqrt(2.0)/2.0 );
}

//testPointPolygon




BOOST_AUTO_TEST_SUITE_END()

