#include <boost/test/unit_test.hpp>

#include <cmath>

#include <SFCGAL/Kernel.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

/*
 * base checks (mainly for compilation issues)
 */
BOOST_AUTO_TEST_SUITE( SFCGAL_KernelTest )

BOOST_AUTO_TEST_CASE( testVector2 )
{
	Vector_2 a(2.0,3.0);
	Vector_2 b(5.0,7.0);

	Vector_2 ab = b - a ;
	BOOST_CHECK_EQUAL( ab.x(), 3.0 );
	BOOST_CHECK_EQUAL( ab.y(), 4.0 );
}


BOOST_AUTO_TEST_SUITE_END()

