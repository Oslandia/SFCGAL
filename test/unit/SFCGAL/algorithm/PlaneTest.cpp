#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/plane.h>

using namespace SFCGAL ;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_PlaneTest )

BOOST_AUTO_TEST_CASE( testPlane1 )
{
	std::auto_ptr<Geometry> gA( io::readWkt("POLYGON((0 0,1 0,1 1,0 1,0 0))") );

	CGAL::Plane_3<Kernel> plane = algorithm::plane3D<Kernel>( gA->as<Polygon>() );
	BOOST_CHECK_EQUAL( plane.a(), 0.0 );
	BOOST_CHECK_EQUAL( plane.b(), 0.0 );
	BOOST_CHECK_EQUAL( plane.c(), 2.0 );
}




BOOST_AUTO_TEST_SUITE_END()

