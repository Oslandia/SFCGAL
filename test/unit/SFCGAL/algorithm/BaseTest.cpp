#include <boost/test/unit_test.hpp>

#include <CGAL/Cartesian.h>

typedef CGAL::Cartesian< double > Kernel ;

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/base.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_BaseTest )

BOOST_AUTO_TEST_CASE( testBase3d_1 )
{
	typedef CGAL::Vector_3< Kernel > Vector_3 ;

	Vector_3 a( 0.0, 0.0, 0.0 );
	Vector_3 b( 1.0, 0.0, 0.0 );
	Vector_3 c( 1.0, 1.0, 0.0 );

	Vector_3 ux, uy, uz ;
	algorithm::base3D( a, b, c, ux, uy, uz );
	BOOST_CHECK_EQUAL( ux, Vector_3(0.0,1.0,0.0) );
	BOOST_CHECK_EQUAL( uy, Vector_3(-1.0,0.0,0.0) );
	BOOST_CHECK_EQUAL( uz, Vector_3(0.0,0.0,1.0) );
}

BOOST_AUTO_TEST_CASE( testBase3d_2 )
{
	typedef CGAL::Vector_3< Kernel > Vector_3 ;

	Vector_3 a( 1.0, 0.0, 0.0 );
	Vector_3 b( 0.0, 0.0, 0.0 );
	Vector_3 c( 0.0, 1.0, 0.0 );

	Vector_3 ux, uy, uz ;
	algorithm::base3D( a, b, c, ux, uy, uz );
	BOOST_CHECK_EQUAL( ux, Vector_3(0.0,1.0,0.0) );
	BOOST_CHECK_EQUAL( uy, Vector_3(1.0,0.0,0.0) );
	BOOST_CHECK_EQUAL( uz, Vector_3(0.0,0.0,-1.0) );
}



BOOST_AUTO_TEST_SUITE_END()

