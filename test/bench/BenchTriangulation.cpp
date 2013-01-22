#include <fstream>

#include <sys/time.h>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/generator/hoch.h>
#include <SFCGAL/generator/disc.h>

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

BOOST_AUTO_TEST_CASE( testPolygonTriangulationHoch )
{
	const int N = 7 ;
	std::auto_ptr< Polygon > fractal( generator::hoch(N) );
	BOOST_CHECK_EQUAL( fractal->exteriorRing().numPoints(), 49153U );

	boost::timer::cpu_timer timer;
	timer.start();
	TriangulatedSurface triangulatedSurface ;
	SFCGAL::algorithm::triangulate( *fractal, triangulatedSurface ) ;
	timer.stop();
	std::cout << "hoch(" << N << ") "<< triangulatedSurface.numTriangles() << " triangles : " << timer.format() << std::endl;
}

BOOST_AUTO_TEST_CASE( testPolygonTriangulationDisc )
{
	const int N = 20000 ;

	std::auto_ptr< Polygon > disc( generator::disc( Point(0.0,0.0), 1.0, 8U ) );
//	std::cout << fractal->asText(5) << std::endl ;

	boost::timer::cpu_timer timer;
	timer.start();
	for ( int i = 0; i < N; i++ ){
		TriangulatedSurface triangulatedSurface ;
		SFCGAL::algorithm::triangulate( *disc, triangulatedSurface ) ;
	}
	timer.stop();
	std::cout << N << " disc triangulation" << timer.format() << std::endl;
}




BOOST_AUTO_TEST_CASE( testMultiPointTriangulation2D )
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
	SFCGAL::algorithm::triangulate2D( multiPoint, triangulatedSurface ) ;
	timer.stop();
	std::cout << "triangulate2D "<< N_POINTS << " points : " << timer.format() << std::endl;
}

BOOST_AUTO_TEST_CASE( testPolygonTriangulationHoch2D )
{
	const int N = 7 ;
	std::auto_ptr< Polygon > fractal( generator::hoch(N) );
//	std::cout << fractal->asText(5) << std::endl ;

	boost::timer::cpu_timer timer;
	timer.start();
	TriangulatedSurface triangulatedSurface ;
	SFCGAL::algorithm::triangulate2D( *fractal, triangulatedSurface ) ;
	timer.stop();
	std::cout << "hoch2D(" << N << ") "<< triangulatedSurface.numTriangles() << " triangles : " << timer.format() << std::endl;
}

BOOST_AUTO_TEST_CASE( testPolygonTriangulationDisc2D )
{
	const int N = 20000 ;

	std::auto_ptr< Polygon > disc( generator::disc( Point(0.0,0.0), 1.0, 8U ) );
//	std::cout << fractal->asText(5) << std::endl ;

	boost::timer::cpu_timer timer;
	timer.start();
	for ( int i = 0; i < N; i++ ){
		TriangulatedSurface triangulatedSurface ;
		SFCGAL::algorithm::triangulate2D( *disc, triangulatedSurface ) ;
	}
	timer.stop();
	std::cout << N << " disc triangulation 2D" << timer.format() << std::endl;
}



BOOST_AUTO_TEST_SUITE_END()




