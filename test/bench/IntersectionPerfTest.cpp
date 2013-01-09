#include <fstream>

#include <sys/time.h>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/convexHull.h>

#include "../test_config.h"

#include <boost/timer/timer.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_IntersectionPerfTest )


#define N_POLYGONS 10000
#define N_POINTS 50

//
// Test limit case
BOOST_AUTO_TEST_CASE( testIntersectionPerf )
{

	//
	// generate polygons
	std::vector<Geometry *> polygons;

	for ( size_t i = 0; i < N_POLYGONS; ++i ) {
		MultiPoint mp;
		for ( size_t j = 0; j < N_POINTS; ++j ) {
			double x = (rand() +.0) / RAND_MAX * 10.0;
			double y = (rand() +.0) / RAND_MAX * 10.0;
			mp.addGeometry( Point( x, y ) );
		}
		//		std::cout << i << ": " << mp.asText() << std::endl;
		std::auto_ptr<Geometry> g( algorithm::convexHull( mp ) );
		polygons.push_back( g.release() );
	}

	boost::timer::cpu_timer timer;
	for ( size_t i = 0; i < N_POLYGONS / 2; ++i ) {
		algorithm::intersects( *polygons[2*i], *polygons[2*i+1] );
	}
	std::cout << "elapsed: " << timer.format() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()




