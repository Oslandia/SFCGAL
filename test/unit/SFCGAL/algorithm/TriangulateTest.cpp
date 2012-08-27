#include <boost/test/unit_test.hpp>

#include <cmath>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/triangulate.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_TriangulateTest )

BOOST_AUTO_TEST_CASE( testTriangulateTriangle )
{
	std::auto_ptr< Geometry > geometry( io::readWkt("POLYGON((0.0 0.0,1.0 0.0,0.5 0.5,0.0 0.0))") );
	TriangulatedSurface triangulatedSurface ;
	algorithm::triangulate( geometry->as< Polygon >(), triangulatedSurface );
	BOOST_CHECK_EQUAL( triangulatedSurface.numTriangles(), 1U );
}


BOOST_AUTO_TEST_CASE( testTriangulateSquare2d )
{
	std::auto_ptr< Geometry > geometry( io::readWkt("POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0))") );
	TriangulatedSurface triangulatedSurface ;
	algorithm::triangulate( geometry->as< Polygon >(), triangulatedSurface );
	BOOST_CHECK_EQUAL( triangulatedSurface.numTriangles(), 2U );
}

BOOST_AUTO_TEST_CASE( testTriangulateVerticalSquare )
{
	std::string wkt( "POLYGON((0.0 0.0 0.0,0.0 0.0 1.0,0.0 1.0 1.0,0.0 1.0 0.0,0.0 0.0 0.0))" );
	std::auto_ptr< Geometry > geometry( io::readWkt(wkt) );
	BOOST_CHECK( geometry->is3D() );
	TriangulatedSurface triangulatedSurface ;
	algorithm::triangulate( geometry->as< Polygon >(), triangulatedSurface );
	BOOST_CHECK_EQUAL( triangulatedSurface.numTriangles(), 2U );
}

BOOST_AUTO_TEST_CASE( testTriangulateConcavePolygon )
{
	std::string wkt("POLYGON((0.2 0.5 0, 0.2 0.5 -1, 0.8 0.5 -1, 0.8 0.5 0, 0.5 0.5 -0.5, 0.2 0.5 0))");
	std::auto_ptr< Geometry > geometry( io::readWkt(wkt) );
	BOOST_CHECK( geometry->is3D() );
	TriangulatedSurface triangulatedSurface ;
	algorithm::triangulate( geometry->as< Polygon >(), triangulatedSurface );
	BOOST_CHECK_EQUAL( triangulatedSurface.numTriangles(), 3U );
}

BOOST_AUTO_TEST_SUITE_END()

