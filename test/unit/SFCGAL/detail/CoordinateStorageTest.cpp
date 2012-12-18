#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/numeric.h>
#include <SFCGAL/detail/CoordinateStorage.h>


using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;


BOOST_AUTO_TEST_SUITE( SFCGAL_detail_CoordinateStorageTest )


BOOST_AUTO_TEST_CASE( testEmpty )
{
	detail::CoordinateStorage empty ;
	BOOST_CHECK_EQUAL( empty.which(), 0 );
	BOOST_CHECK_EQUAL( detail::x( empty ), 0 );
	BOOST_CHECK_EQUAL( detail::y( empty ), 0 );
	BOOST_CHECK_EQUAL( detail::z( empty ), 0 );
}

BOOST_AUTO_TEST_CASE( test2D )
{
	detail::CoordinateStorage empty( Kernel::Point_2(3.0,4.0) ) ;
	BOOST_CHECK_EQUAL( empty.which(), 1 );
	BOOST_CHECK_EQUAL( detail::x( empty ), 3 );
	BOOST_CHECK_EQUAL( detail::y( empty ), 4 );
	BOOST_CHECK_EQUAL( detail::z( empty ), 0 );
}

BOOST_AUTO_TEST_CASE( test3D )
{
	detail::CoordinateStorage empty( Kernel::Point_3(3.0,4.0,5.0) ) ;
	BOOST_CHECK_EQUAL( empty.which(), 2 );
	BOOST_CHECK_EQUAL( detail::x( empty ), 3 );
	BOOST_CHECK_EQUAL( detail::y( empty ), 4 );
	BOOST_CHECK_EQUAL( detail::z( empty ), 5 );
}


BOOST_AUTO_TEST_SUITE_END()

