#include <boost/test/unit_test.hpp>

#include <CGAL/Cartesian.h>

typedef CGAL::Cartesian< double > Kernel ;

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/normal.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_NormalTest )

BOOST_AUTO_TEST_CASE( testNormal1 )
{
	typedef CGAL::Vector_3< Kernel > Vector_3 ;

	Vector_3 a( 0.0, 0.0, 0.0 );
	Vector_3 b( 1.0, 0.0, 0.0 );
	Vector_3 c( 1.0, 1.0, 0.0 );

	Vector_3 normal = algorithm::normal3D( a, b, c );
	BOOST_CHECK_EQUAL( normal.x(), 0.0 );
	BOOST_CHECK_EQUAL( normal.y(), 0.0 );
	BOOST_CHECK_EQUAL( normal.z(), 1.0 );
}




BOOST_AUTO_TEST_SUITE_END()

