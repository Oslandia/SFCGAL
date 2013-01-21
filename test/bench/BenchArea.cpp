#include <fstream>

#include <sys/time.h>

#include <SFCGAL/all.h>

#include "../test_config.h"

#include <boost/timer/timer.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>

#include <SFCGAL/generator/sierpinski.h>

#include <SFCGAL/algorithm/area.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_BenchArea )


BOOST_AUTO_TEST_CASE( testAreaSierpinski )
{
	std::auto_ptr< MultiPolygon > fractal( generator::sierpinski(9) ) ;

	boost::timer::cpu_timer timer ;

	timer.start();
	for ( int i = 0; i < 10; i++ ){
		algorithm::area2D( *fractal ) ;
	}
	timer.stop();

	std::cout << fractal->numGeometries() << std::endl;
	std::cout << "area sierpinski : " << timer.format() << std::endl ;
}

BOOST_AUTO_TEST_CASE( testAreaSierpinski3D )
{
	std::auto_ptr< MultiPolygon > fractal( generator::sierpinski(9) ) ;

	boost::timer::cpu_timer timer ;

	timer.start();
	for ( int i = 0; i < 10; i++ ){
		algorithm::area3D( *fractal ) ;
	}
	timer.stop();

	std::cout << fractal->numGeometries() << std::endl;
	std::cout << "area sierpinski 3D : " << timer.format() << std::endl ;
}


BOOST_AUTO_TEST_SUITE_END()




