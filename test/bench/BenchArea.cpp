#include <fstream>

#include <sys/time.h>

#include <SFCGAL/all.h>

#include "../test_config.h"
#include "Bench.h"

#include <boost/test/unit_test.hpp>

#include <SFCGAL/generator/sierpinski.h>

#include <SFCGAL/algorithm/area.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_BenchArea )


BOOST_AUTO_TEST_CASE( testAreaSierpinski )
{
	std::auto_ptr< MultiPolygon > fractal( generator::sierpinski(9) ) ;

	bench().start( "area sierpinski" ) ;
	for ( int i = 0; i < 10; i++ ){
		algorithm::area( *fractal ) ;
	}
	bench().stop();
}

BOOST_AUTO_TEST_CASE( testAreaSierpinski3D )
{
	std::auto_ptr< MultiPolygon > fractal( generator::sierpinski(9) ) ;

	bench().start( "area sierpinski" ) ;
	for ( int i = 0; i < 10; i++ ){
		algorithm::area3D( *fractal ) ;
	}
	bench().stop() ;
}


BOOST_AUTO_TEST_SUITE_END()




