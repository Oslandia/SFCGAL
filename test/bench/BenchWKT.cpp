#include <fstream>

#include <sys/time.h>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include "../test_config.h"

#include <boost/timer/timer.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>

using namespace boost::unit_test ;
using namespace SFCGAL ;

#define N_WKTS 100000

BOOST_AUTO_TEST_SUITE( SFCGAL_BenchWKT )

//
// Test limit case
BOOST_AUTO_TEST_CASE( testReadPoints )
{
	boost::timer::cpu_timer timer;
	timer.start();
	for ( int i = 0; i < N_WKTS; i++ ){
		io::readWkt( "POINT(50000 50000)" ) ;
	}
	timer.stop();
	std::cout << N_WKTS << " WKT points : " << timer.format() << std::endl;
}


//
// Test limit case
BOOST_AUTO_TEST_CASE( testReadLineString )
{
	boost::timer::cpu_timer timer;
	timer.start();
	for ( int i = 0; i < N_WKTS; i++ ){
		io::readWkt( "LINESTRING(0 0,0 1000,1000 1000)" ) ;
	}
	timer.stop();
	std::cout << N_WKTS << " WKT LineStrings : " << timer.format() << std::endl;
}

//
// Test limit case
BOOST_AUTO_TEST_CASE( testReadPolygon )
{
	boost::timer::cpu_timer timer;
	timer.start();
	for ( int i = 0; i < N_WKTS; i++ ){
		io::readWkt( "POLYGON((0 0,0 1000,1000 1000,1000 0,0 0))" ) ;
	}
	timer.stop();
	std::cout << N_WKTS << " WKT Polygons : " << timer.format() << std::endl;
}

//
// Test limit case
BOOST_AUTO_TEST_CASE( testReadTriangle )
{
	boost::timer::cpu_timer timer;
	timer.start();
	for ( int i = 0; i < N_WKTS; i++ ){
		io::readWkt( "TRIANGLE((0 0,0 1000,1000 1000,1000 0,0 0))" ) ;
	}
	timer.stop();
	std::cout << N_WKTS << " WKT Triangles : " << timer.format() << std::endl;
}

//
// Test limit case
BOOST_AUTO_TEST_CASE( testReadSolid )
{
	boost::timer::cpu_timer timer;
	timer.start();
	for ( int i = 0; i < N_WKTS; i++ ){
		io::readWkt( "SOLID((((0.0 0.0 0.0,0.0 1.0 0.0,1.0 1.0 0.0,1.0 0.0 0.0,0.0 0.0 0.0),(0.2 0.2 0.0,0.8 0.2 0.0,0.8 0.8 0.0,0.2 0.8 0.0,0.2 0.2 0.0)),((0.0 0.0 1.0,1.0 0.0 1.0,1.0 1.0 1.0,0.0 1.0 1.0,0.0 0.0 1.0),(0.2 0.2 1.0,0.2 0.8 1.0,0.8 0.8 1.0,0.8 0.2 1.0,0.2 0.2 1.0)),((0.0 0.0 0.0,0.0 0.0 1.0,0.0 1.0 1.0,0.0 1.0 0.0,0.0 0.0 0.0)),((0.0 1.0 0.0,0.0 1.0 1.0,1.0 1.0 1.0,1.0 1.0 0.0,0.0 1.0 0.0)),((1.0 1.0 0.0,1.0 1.0 1.0,1.0 0.0 1.0,1.0 0.0 0.0,1.0 1.0 0.0)),((1.0 0.0 0.0,1.0 0.0 1.0,0.0 0.0 1.0,0.0 0.0 0.0,1.0 0.0 0.0)),((0.2 0.2 0.0,0.2 0.2 1.0,0.8 0.2 1.0,0.8 0.2 0.0,0.2 0.2 0.0)),((0.8 0.2 0.0,0.8 0.2 1.0,0.8 0.8 1.0,0.8 0.8 0.0,0.8 0.2 0.0)),((0.8 0.8 0.0,0.8 0.8 1.0,0.2 0.8 1.0,0.2 0.8 0.0,0.8 0.8 0.0)),((0.2 0.8 0.0,0.2 0.8 1.0,0.2 0.2 1.0,0.2 0.2 0.0,0.2 0.8 0.0))))" ) ;
	}
	timer.stop();
	std::cout << N_WKTS << " WKT Solids : " << timer.format() << std::endl;
}




BOOST_AUTO_TEST_SUITE_END()




