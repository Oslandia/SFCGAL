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
BOOST_AUTO_TEST_SUITE_END()

