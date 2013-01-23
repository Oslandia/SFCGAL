#include <fstream>

#include <sys/time.h>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/generator/hoch.h>
#include <SFCGAL/generator/disc.h>

#include "../test_config.h"
#include "Bench.h"

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

	bench().start( boost::format("triangulate %s points") % N_POINTS );
	TriangulatedSurface triangulatedSurface ;
	SFCGAL::algorithm::triangulate( multiPoint, triangulatedSurface ) ;
	bench().stop();
}

BOOST_AUTO_TEST_CASE( testPolygonTriangulationHoch )
{
	const int N = 7 ;
	std::auto_ptr< Polygon > fractal( generator::hoch(N) );
	BOOST_CHECK_EQUAL( fractal->exteriorRing().numPoints(), 49153U );

	bench().start( boost::format("triangulate hoch(%s)") % N );
	TriangulatedSurface triangulatedSurface ;
	SFCGAL::algorithm::triangulate( *fractal, triangulatedSurface ) ;
	bench().stop() ;
}

BOOST_AUTO_TEST_CASE( testPolygonTriangulationDisc )
{
	const int N = 20000 ;

	std::auto_ptr< Polygon > disc( generator::disc( Point(0.0,0.0), 1.0, 8U ) );
//	std::cout << fractal->asText(5) << std::endl ;

	bench().start( boost::format("triangulate disc x %s") % N );
	for ( int i = 0; i < N; i++ ){
		TriangulatedSurface triangulatedSurface ;
		SFCGAL::algorithm::triangulate( *disc, triangulatedSurface ) ;
	}
	bench().stop();
}




BOOST_AUTO_TEST_CASE( testMultiPointTriangulation2D )
{
	CGAL::Random_points_in_disc_2< Kernel::Point_2, Creator > g( 150.0 );

	MultiPoint multiPoint ;
	for ( int i = 0; i < N_POINTS; i++ ){
		Kernel::Point_2 p = *(++g) ;
		multiPoint.addGeometry( new Point( p ) ) ;
	}

	bench().start( boost::format("triangulate2D %s points") % N_POINTS );
	TriangulatedSurface triangulatedSurface ;
	SFCGAL::algorithm::triangulate2D( multiPoint, triangulatedSurface ) ;
	bench().stop();
}

BOOST_AUTO_TEST_CASE( testPolygonTriangulationHoch2D )
{
	const int N = 7 ;
	std::auto_ptr< Polygon > fractal( generator::hoch(N) );
//	std::cout << fractal->asText(5) << std::endl ;

	bench().start( boost::format("triangulate2D hoch(%s)") % N );
	TriangulatedSurface triangulatedSurface ;
	SFCGAL::algorithm::triangulate2D( *fractal, triangulatedSurface ) ;
	bench().stop();
}

BOOST_AUTO_TEST_CASE( testPolygonTriangulationDisc2D )
{
	const int N = 20000 ;

	std::auto_ptr< Polygon > disc( generator::disc( Point(0.0,0.0), 1.0, 8U ) ) ;
//	std::cout << fractal->asText(5) << std::endl ;

	bench().start( boost::format("triangulate2D disc x %s") % N ) ;
	for ( int i = 0; i < N; i++ ){
		TriangulatedSurface triangulatedSurface ;
		SFCGAL::algorithm::triangulate2D( *disc, triangulatedSurface ) ;
	}
	bench().stop() ;
}



BOOST_AUTO_TEST_SUITE_END()




