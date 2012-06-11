#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

#include <SFCGAL/LineString.h>

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_LineStringTest )

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
	LineString g;
	BOOST_CHECK( g.isEmpty() );
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.numPoints(), 0U );
}


//-- asText

BOOST_AUTO_TEST_CASE( asTextEmpty )
{
	LineString g;
	BOOST_CHECK_EQUAL( g.asText(1), "LINESTRING EMPTY" );
}


BOOST_AUTO_TEST_CASE( asText2d )
{
	LineString g(
		Point(2.0,3.0),
		Point(4.0,5.0)
	);
	BOOST_CHECK_EQUAL( g.asText(3), "LINESTRING(2.000 3.000,4.000 5.000)" );
}
BOOST_AUTO_TEST_CASE( asText3d )
{
	LineString g(
		Point(2.0,3.0,7.0),
		Point(4.0,5.0,8.0)
	);
	BOOST_CHECK_EQUAL( g.asText(3), "LINESTRING(2.000 3.000 7.000,4.000 5.000 8.000)" );
}

BOOST_AUTO_TEST_CASE( isLineString )
{
	LineString g;
	BOOST_CHECK( g.is< LineString >() );
}

BOOST_AUTO_TEST_SUITE_END()




