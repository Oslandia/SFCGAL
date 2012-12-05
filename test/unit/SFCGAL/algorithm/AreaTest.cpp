#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/area.h>


using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_AreaTest )

BOOST_AUTO_TEST_CASE( testArea3D_Triangle1 )
{
	Triangle triangle( Point(0.0,0.0,0.0), Point(0.0,0.0,1.0), Point(0.0,1.0, 0.0) );
	BOOST_CHECK_EQUAL( algorithm::area3D( triangle ), 0.5 );
}

BOOST_AUTO_TEST_CASE( testArea3D_Triangle2 )
{
	Triangle triangle( Point(0.0,0.0,0.0), Point(0.0,0.0,4.0), Point(0.0,4.0, 0.0) );
	BOOST_CHECK_EQUAL( algorithm::area3D( triangle ), 8.0 );
}

BOOST_AUTO_TEST_CASE( testArea2D_Triangle )
{
	Triangle triangle1( Point(0.0,0.0), Point(4.0,0.0), Point(4.0,4.0) );
	// the same, inverted
	Triangle triangle2( Point(0.0,0.0), Point(0.0,4.0), Point(4.0,4.0) );
	BOOST_CHECK_EQUAL( algorithm::area2D( triangle1 ), 8.0 );
	BOOST_CHECK_EQUAL( algorithm::area2D( triangle2 ), 8.0 );
}

BOOST_AUTO_TEST_CASE( testArea3D_Square1x1 )
{
	std::auto_ptr< Geometry > g( io::readWkt( "POLYGON((0.0 0.0 0.0,0.0 0.0 1.0,0.0 1.0 1.0,0.0 1.0 0.0,0.0 0.0 0.0))" ) );
	BOOST_CHECK_EQUAL( g->asText(1), "POLYGON((0.0 0.0 0.0,0.0 0.0 1.0,0.0 1.0 1.0,0.0 1.0 0.0,0.0 0.0 0.0))" );
	BOOST_CHECK_CLOSE( algorithm::area3D( *g ), 1.0, 1e-10 );
}

BOOST_AUTO_TEST_CASE( testArea3D_Square4X4 )
{
	std::string wkt( "POLYGON((0.0 0.0 0.0,0.0 0.0 4.0,0.0 4.0 4.0,0.0 4.0 0.0,0.0 0.0 0.0))" );
	std::auto_ptr< Geometry > g( io::readWkt( wkt ) );
	BOOST_CHECK_CLOSE( algorithm::area3D( *g ), 16.0, 1e-10 );
}

BOOST_AUTO_TEST_CASE( testArea3D_Square4X4WithHole )
{
	std::string wkt( "POLYGON((0.0 0.0 0.0,0.0 0.0 4.0,0.0 4.0 4.0,0.0 4.0 0.0,0.0 0.0 0.0),(0.0 2.0 2.0,0.0 2.0 3.0,0.0 3.0 3.0,0.0 3.0 2.0,0.0 2.0 2.0))" );
	std::auto_ptr< Geometry > g( io::readWkt( wkt ) );
	BOOST_CHECK_CLOSE( algorithm::area3D( *g ), 15.0, 1e-10 );
}



BOOST_AUTO_TEST_SUITE_END()

