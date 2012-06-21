#include <boost/test/unit_test.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/convexHull.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_ConvexHullTest )

BOOST_AUTO_TEST_CASE( testPoint )
{
	Point p(1.0,2.0,3.0);
	std::auto_ptr< Geometry > hull( algorithm::convexHull3D(p) );
	BOOST_CHECK( hull->is< Point >() );
	BOOST_CHECK_EQUAL( hull->as< Point >().x(), 1.0 ) ;
	BOOST_CHECK_EQUAL( hull->as< Point >().y(), 2.0 ) ;
	BOOST_CHECK_EQUAL( hull->as< Point >().z(), 3.0 ) ;
}

/*
 * @todo Test if points are collinear
 */
BOOST_AUTO_TEST_CASE( testLineStringCollinear )
{
	std::vector< Point > points ;
	points.push_back( Point(0.0,0.0,0.0)  );
	points.push_back( Point(1.0,1.0,1.0)  );
	points.push_back( Point(2.0,2.0,2.0)  );
	points.push_back( Point(3.0,3.0,3.0)  );

	LineString lineString( points ) ;
	std::auto_ptr< Geometry > hull( algorithm::convexHull3D( lineString ) );
	BOOST_CHECK( hull->is< LineString >() );
}

BOOST_AUTO_TEST_CASE( testLineStringCoplanar )
{
	std::vector< Point > points ;
	points.push_back( Point(0.0,0.0,1.0)  );
	points.push_back( Point(1.0,0.0,1.0)  );
	points.push_back( Point(1.0,1.0,1.0)  );
	points.push_back( Point(0.0,1.0,1.0)  );

	LineString lineString( points ) ;
	std::auto_ptr< Geometry > hull( algorithm::convexHull3D( lineString ) );
	BOOST_CHECK( hull->is< PolyhedralSurface >() );
	BOOST_CHECK_EQUAL( hull->as< PolyhedralSurface >().numPolygons(), 1U );
}




BOOST_AUTO_TEST_CASE( testTetrahedron )
{
	std::vector< Point > points ;
	points.push_back( Point(0.0,0.0,0.0) );
	points.push_back( Point(1.0,0.0,0.0) );
	points.push_back( Point(0.0,1.0,0.0) );
	points.push_back( Point(0.0,0.0,1.0) );

	LineString lineString( points ) ;
	std::auto_ptr< Geometry > hull( algorithm::convexHull3D( lineString ) );
	BOOST_CHECK( hull->is< PolyhedralSurface >() );
	BOOST_CHECK_EQUAL( hull->as< PolyhedralSurface >().numPolygons(), 4U );

}


BOOST_AUTO_TEST_SUITE_END()

