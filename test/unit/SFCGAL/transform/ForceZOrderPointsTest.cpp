#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/transform/ForceZOrderPoints.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_transform_ForceZOrderPointsTest )

BOOST_AUTO_TEST_CASE( simple )
{
	std::auto_ptr<Geometry> g1 = io::readWkt( "POLYGON((0 0,0 1,1 1,1 0,0 0))" );

	const Polygon& p = g1->as<Polygon>();
	BOOST_CHECK( ! p.isCounterClockWiseOriented() );

	transform::ForceZOrderPoints forceZ;
	g1->accept( forceZ );

	BOOST_CHECK( g1->is3D() );
	BOOST_CHECK( g1->as<Polygon>().isCounterClockWiseOriented() );
}



BOOST_AUTO_TEST_SUITE_END()




