#include <boost/test/unit_test.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>


using namespace SFCGAL ;

using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_BoundaryTest )

//-- Point

BOOST_AUTO_TEST_CASE( emptyPoint )
{
	Point g;
	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->is< GeometryCollection >() );
	BOOST_CHECK( boundary->isEmpty() );
}
BOOST_AUTO_TEST_CASE( pointXY )
{
	Point g(2.0,3.0);
	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->is< GeometryCollection >() );
	BOOST_CHECK( boundary->isEmpty() );
}

//-- LineString

BOOST_AUTO_TEST_CASE( emptyLineString )
{
	LineString g;
	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->is< GeometryCollection >() );
	BOOST_CHECK( boundary->isEmpty() );
}

BOOST_AUTO_TEST_CASE( lineStringSegment )
{
	LineString g;
	g.addPoint( Point(0.0,0.0) );
	g.addPoint( Point(1.0,1.0) );
	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->is< MultiPoint >() );
	BOOST_CHECK_EQUAL( boundary->as< MultiPoint >().numGeometries(), 2U );
}

BOOST_AUTO_TEST_CASE( lineStringWithThreePoints )
{
	LineString g;
	g.addPoint( Point(0.0,0.0) );
	g.addPoint( Point(1.0,1.0) );
	g.addPoint( Point(2.0,2.0) );
	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->is< MultiPoint >() );
	BOOST_CHECK_EQUAL( boundary->as< MultiPoint >().numGeometries(), 2U );
}

BOOST_AUTO_TEST_CASE( lineStringClosed )
{
	LineString g;
	g.addPoint( Point(0.0,0.0) );
	g.addPoint( Point(1.0,1.0) );
	g.addPoint( Point(2.0,2.0) );
	g.addPoint( Point(0.0,0.0) );

	std::auto_ptr< Geometry > boundary( g.boundary() );

	BOOST_CHECK( boundary->isEmpty() );
	BOOST_CHECK( boundary->is< GeometryCollection >() );
}



BOOST_AUTO_TEST_SUITE_END()

