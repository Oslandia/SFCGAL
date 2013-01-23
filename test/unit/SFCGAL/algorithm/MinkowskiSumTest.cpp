#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/minkowskiSum.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_MinkowskiTest )

BOOST_AUTO_TEST_CASE( testPoint )
{
	std::auto_ptr< Geometry > gA( io::readWkt("POINT(0 0)") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((0 0,0 1,1 1,1 0,0 0))") );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	BOOST_CHECK_EQUAL( sum->asText(0), "POLYGON((0 0,0 1,1 1,1 0,0 0))" );
}
BOOST_AUTO_TEST_CASE( testLineString )
{
	std::auto_ptr< Geometry > gA( io::readWkt("LINESTRING(0 0,5 0)") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((-1 0,0 -1,1 0,0 1,-1 0))") );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	BOOST_CHECK_EQUAL( sum->asText(0), "POLYGON((5 1,0 1,-1 0,0 -1,5 -1,6 0,5 1))" );
}

BOOST_AUTO_TEST_CASE( testLineString2 )
{
	std::auto_ptr< Geometry > gA( io::readWkt("LINESTRING(5 5,0 5,5 0,0 0)") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((-1 0,0 -1,1 0,0 1,-1 0))") );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	BOOST_CHECK_EQUAL( sum->asText(0), "MULTIPOLYGON(((5 1,2 4,5 4,6 5,5 6,0 6,-1 5,0 4,3 1,0 1,-1 0,0 -1,5 -1,6 0,5 1)))" );
}

BOOST_AUTO_TEST_CASE( testPolygonWithHole ){
	std::string wkt = "POLYGON((11.966308 -10.211022,18.007885 1.872133,39.364158 2.434140,53.554839 -6.557975,43.438710 -22.856183,20.396416 -28.476254,5.643728 -25.525717,13.090323 -20.889158,32.479570 -21.310663,38.521147 -15.831093,46.248746 -9.087007,34.446595 -1.359409,22.784946 -14.988082,11.966308 -10.211022),(20.396416 -1.640412,15.900358 -7.260484,18.007885 -9.508513,22.644444 -9.368011,25.173477 -2.342921,20.396416 -1.640412),(41.050179 -0.797401,40.207168 -2.202419,47.934767 -6.557975,48.496774 -5.433961,41.050179 -0.797401))" ;
	std::auto_ptr< Geometry > gA( io::readWkt( wkt ) );
	std::auto_ptr< Geometry > gB( io::readWkt( "POLYGON((-10 0,0 -10,10 0,0 10,-10 0))" ) );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	std::cout << sum->asText(6) << std::endl ;
}

BOOST_AUTO_TEST_SUITE_END()

