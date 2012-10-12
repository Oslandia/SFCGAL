#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/orientation.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_OrientationTest )

//-- hasConsistentOrientation

BOOST_AUTO_TEST_CASE( hasConsistentOrientation3D_basicTriangles )
{
	Triangle triangle( Point(0.0,0.0,0.0), Point(0.0,0.0,1.0), Point(0.0,1.0, 0.0) );
	TriangulatedSurface triangulatedSurface;
	triangulatedSurface.addTriangle(
		Triangle(
			Point(0.0,0.0,0.0),
			Point(1.0,0.0,0.0),
			Point(0.0,1.0,0.0)
		)
	);
	BOOST_CHECK( algorithm::hasConsistentOrientation3D( triangulatedSurface ) );

	triangulatedSurface.addTriangle(
		Triangle(
			Point( 0.0,0.0,0.0),
			Point( 0.0,1.0,0.0),
			Point(-1.0,0.0,0.0)
		)
	);
	BOOST_CHECK( algorithm::hasConsistentOrientation3D( triangulatedSurface ) );

	triangulatedSurface.addTriangle(
		Triangle(
			Point(0.0, 0.0, 0.0 ),
			Point(1.0, 0.0, 0.0 ),
			Point(0.0,-1.0, 0.0 )
		)
	);
	BOOST_CHECK( ! algorithm::hasConsistentOrientation3D( triangulatedSurface ) );
}




BOOST_AUTO_TEST_SUITE_END()

