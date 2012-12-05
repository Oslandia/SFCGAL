#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/normal.h>

using namespace SFCGAL ;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_NormalTest )

BOOST_AUTO_TEST_CASE( testNormal1 )
{
	typedef CGAL::Vector_3< Kernel > Vector_3 ;
	typedef CGAL::Point_3< Kernel > Point_3 ;

	Point_3 a( 0.0, 0.0, 0.0 );
	Point_3 b( 1.0, 0.0, 0.0 );
	Point_3 c( 1.0, 1.0, 0.0 );

	Vector_3 normal = algorithm::normal3D( a, b, c );
	BOOST_CHECK_EQUAL( normal.x(), 0.0 );
	BOOST_CHECK_EQUAL( normal.y(), 0.0 );
	BOOST_CHECK_EQUAL( normal.z(), 1.0 );
}

BOOST_AUTO_TEST_CASE( testNormal2 )
{
	// a square ccw
	std::auto_ptr<Geometry> gA( io::readWkt("POLYGON((0 0,1 0,1 1,0 1,0 0))") );
	// a square cw oriented
	std::auto_ptr<Geometry> gB( io::readWkt("POLYGON((0 0,0 1,1 1,1 0,0 0))") );

	// a pseudo-square ccw oriented, with a concave part
	std::auto_ptr<Geometry> gC( io::readWkt("POLYGON((0 0,0.5 0.5,1 0,1 1,0 1,0 0))") );

	{
		CGAL::Vector_3<Kernel> normal = algorithm::normal3D<Kernel>( gA->as<Polygon>() );
		BOOST_CHECK_EQUAL( normal.x(), 0.0 );
		BOOST_CHECK_EQUAL( normal.y(), 0.0 );
		BOOST_CHECK_EQUAL( normal.z(), 2.0 );
	}

	{
		CGAL::Vector_3<Kernel> normal = algorithm::normal3D<Kernel>( gB->as<Polygon>() );
		BOOST_CHECK_EQUAL( normal.x(), 0.0 );
		BOOST_CHECK_EQUAL( normal.y(), 0.0 );
		BOOST_CHECK_EQUAL( normal.z(), -2.0 );
	}

	{
		CGAL::Vector_3<Kernel> normal = algorithm::normal3D<Kernel>( gC->as<Polygon>() );
		BOOST_CHECK_EQUAL( normal.x(), 0.0 );
		BOOST_CHECK_EQUAL( normal.y(), 0.0 );
		// ok, the normal is pointing up (z > 0)
		BOOST_CHECK_EQUAL( normal.z(), 1.5 );
	}
}

BOOST_AUTO_TEST_SUITE_END()

