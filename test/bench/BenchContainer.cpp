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

BOOST_AUTO_TEST_SUITE( SFCGAL_BenchContainer )

//
// Test limit case
BOOST_AUTO_TEST_CASE( testVectorOfPoint2 )
{
	const int N = 10000 ;

	boost::timer::cpu_timer timer;

	// create points
	timer.start();
	std::vector< Kernel::Point_2 > points ;
	points.reserve( N ) ;
	for ( int i = 0; i < N; i++ ){
		points.push_back( Kernel::Point_2(i,i)  ) ;
	}
	timer.stop();
	std::cout << "std::vector< Kernel::Point_2 > create " << timer.format() << std::endl ;

	// centroid
	timer.start();
	Kernel::FT x = 0 , y = 0 ;
	for ( std::vector< Kernel::Point_2 >::const_iterator it = points.begin(); it != points.end(); ++it ){
		x += it->x() ;
		y += it->y() ;
	}
	Kernel::Point_2 centroid( x / Kernel::FT(points.size()), y / Kernel::FT(points.size()) );
	timer.stop();
	std::cout << "std::vector< Kernel::Point_2 > access " << timer.format() << std::endl ;

	// reverse
	timer.start();
	std::reverse( points.begin(), points.end() ) ;
	timer.stop();
	std::cout << "std::vector< Kernel::Point_2 > reverse " << timer.format() << std::endl ;

	// clear
	timer.start();
	points.clear() ;
	timer.stop();
	std::cout << "std::vector< Kernel::Point_2 > clear " << timer.format() << std::endl ;
}

//
// Test limit case
BOOST_AUTO_TEST_CASE( testVectorOfPoint )
{
	const int N = 10000 ;

	boost::timer::cpu_timer timer;

	// create points
	timer.start();
	std::vector< Point > points ;
	points.reserve( N ) ;
	for ( int i = 0; i < N; i++ ){
		points.push_back( Point(i,i)  ) ;
	}
	timer.stop();
	std::cout << "std::vector< Point > create " << timer.format() << std::endl ;

	// centroid
	timer.start();
	Kernel::FT x = 0 , y = 0 ;
	for ( std::vector< Point >::const_iterator it = points.begin(); it != points.end(); ++it ){
		x += it->x() ;
		y += it->y() ;
	}
	Point centroid( x / Kernel::FT(points.size()), y / Kernel::FT(points.size()) );
	timer.stop();
	std::cout << "std::vector< Point > access " << timer.format() << std::endl ;


	// reverse
	timer.start();
	std::reverse( points.begin(), points.end() ) ;
	timer.stop();
	std::cout << "std::vector< Point > reverse " << timer.format() << std::endl ;

	// clear
	timer.start();
	points.clear() ;
	timer.stop();
	std::cout << "std::vector< Point > clear " << timer.format() << std::endl ;
}


//
// Test limit case
BOOST_AUTO_TEST_CASE( testPtrVectorOfPoint )
{
	const int N = 10000 ;

	boost::timer::cpu_timer timer;

	// create points
	timer.start();
	boost::ptr_vector< Point > points ;
	points.reserve( N ) ;
	for ( int i = 0; i < N; i++ ){
		points.push_back( new Point(i,i)  ) ;
	}
	timer.stop();
	std::cout << "boost::ptr_vector< Point > create " << timer.format() << std::endl ;

	// centroid
	timer.start();
	Kernel::FT x = 0 , y = 0 ;
	for ( boost::ptr_vector< Point >::const_iterator it = points.begin(); it != points.end(); ++it ){
		x += it->x() ;
		y += it->y() ;
	}
	Point centroid( x / Kernel::FT(points.size()), y / Kernel::FT(points.size()) );
	timer.stop();
	std::cout << "boost::ptr_vector< Point > access " << timer.format() << std::endl ;


	// reverse
	timer.start();
	std::reverse( points.begin(), points.end() ) ;
	timer.stop();
	std::cout << "boost::ptr_vector< Point > reverse " << timer.format() << std::endl ;

	// clear
	timer.start();
	points.clear() ;
	timer.stop();
	std::cout << "boost::ptr_vector< Point > clear " << timer.format() << std::endl ;
}



BOOST_AUTO_TEST_SUITE_END()




