#include <boost/test/unit_test.hpp>

#include <CGAL/Cartesian.h>

typedef CGAL::Cartesian< double > Kernel ;

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/ConsistentOrientationBuilder.h>
#include <SFCGAL/algorithm/orientation.h>


using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_ConsistentOrientationBuilderTest )

BOOST_AUTO_TEST_CASE( testOppositeTriangle )
{
	algorithm::ConsistentOrientationBuilder builder ;
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point( 1.0, 0.0, 0.0 ),
			Point( 0.0, 1.0, 0.0 )
		)
	);
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point(-1.0, 0.0, 0.0 ),
			Point( 0.0, 1.0, 0.0 )
		)
	);
	TriangulatedSurface triangulatedSurface = builder.buildTriangulatedSurface();
	BOOST_CHECK_EQUAL( triangulatedSurface.numGeometries(), 2U );
	BOOST_CHECK( algorithm::hasConsistentOrientation3D( triangulatedSurface ) );
}

BOOST_AUTO_TEST_CASE( testFourTriangle )
{
	algorithm::ConsistentOrientationBuilder builder ;
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point( 1.0, 0.0, 0.0 ),
			Point( 0.0, 1.0, 0.0 )
		)
	);
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point(-1.0, 0.0, 0.0 ),
			Point( 0.0, 1.0, 0.0 )
		)
	);
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point( 1.0, 0.0, 0.0 ),
			Point( 0.0,-1.0, 0.0 )
		)
	);
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point(-1.0, 0.0, 0.0 ),
			Point( 0.0,-1.0, 0.0 )
		)
	);
	TriangulatedSurface triangulatedSurface = builder.buildTriangulatedSurface();
	BOOST_CHECK_EQUAL( triangulatedSurface.numGeometries(), 4U );
	BOOST_CHECK( algorithm::hasConsistentOrientation3D( triangulatedSurface ) );
}


BOOST_AUTO_TEST_SUITE_END()

