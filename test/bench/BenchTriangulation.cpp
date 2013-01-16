#include <fstream>

#include <sys/time.h>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include "../test_config.h"

#include <boost/timer/timer.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>

#include <CGAL/point_generators_2.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

#define N_POINTS 100000

typedef CGAL::Creator_uniform_2< double, Kernel::Point_2 > Creator ;


BOOST_AUTO_TEST_SUITE( SFCGAL_BenchTriangulation )

BOOST_AUTO_TEST_CASE( testMultiPointTriangulation )
{
	CGAL::Random_points_in_disc_2< Kernel::Point_2, Creator > g( 150.0 );

	MultiPoint multiPoint ;
	for ( int i = 0; i < N_POINTS; i++ ){
		Kernel::Point_2 p = *(++g) ;
		multiPoint.addGeometry( new Point( p ) ) ;
	}

	boost::timer::cpu_timer timer;
	timer.start();
	TriangulatedSurface triangulatedSurface ;
	SFCGAL::algorithm::triangulate( multiPoint, triangulatedSurface ) ;
	timer.stop();
	std::cout << "triangulate "<< N_POINTS << " points : " << timer.format() << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()




