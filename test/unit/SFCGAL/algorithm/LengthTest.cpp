#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/length.h>


using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_LengthTest )

BOOST_AUTO_TEST_CASE( testZeroLength )
{
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("POINT(0.0 0.0)" ) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("LINESTRING EMPTY" ) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("POLYGON((0 0,0 1,1 1,1 0,0 0))" ) ), 0.0 );
}

BOOST_AUTO_TEST_CASE( testZeroLengthVertical )
{
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("LINESTRING(0.0 0.0 0.0,0.0 0.0 1.0)" ) ), 0.0 );
}

BOOST_AUTO_TEST_CASE( testLengthLineString )
{
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("LINESTRING(0.0 0.0,3.0 4.0)" ) ), 5.0 );
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("LINESTRING(0.0 0.0,0.0 1.0,1.0 1.0)" ) ), 2.0 );
}

//-- 3D


BOOST_AUTO_TEST_CASE( test3DZeroLength )
{
	BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt("POINT(0.0 0.0)" ) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt("LINESTRING EMPTY" ) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt("POLYGON((0 0,0 1,1 1,1 0,0 0))" ) ), 0.0 );
}
BOOST_AUTO_TEST_CASE( test3DLengthVertical )
{
	BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt("LINESTRING(0.0 0.0 0.0,0.0 0.0 1.0)" ) ), 1.0 );
}
BOOST_AUTO_TEST_CASE( test3DLengthLineString )
{
	BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt("LINESTRING(0.0 0.0 0.0,0.0 1.0 0.0,0.0 1.0 1.0)" ) ), 2.0 );
}

BOOST_AUTO_TEST_SUITE_END()

