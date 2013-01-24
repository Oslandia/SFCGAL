#include <SFCGAL/Kernel.h>
#include <SFCGAL/Point.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_PointTest )

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
	Point g;
	BOOST_CHECK( g.isEmpty() ) ;
	BOOST_CHECK( ! g.is3D() );

	// no more access to double
/*
	BOOST_CHECK( isNaN( g.x() ) );
	BOOST_CHECK( isNaN( g.y() ) );
	BOOST_CHECK( isNaN( g.z() ) );
*/
	BOOST_CHECK_EQUAL( g.x(), 0 );
	BOOST_CHECK_EQUAL( g.y(), 0 );
	BOOST_CHECK_EQUAL( g.z(), 0 );
}

BOOST_AUTO_TEST_CASE( testGeometryTypeId ){
	Point g;
	BOOST_CHECK_EQUAL( g.geometryTypeId(), TYPE_POINT );
}


BOOST_AUTO_TEST_CASE( xyConstructor )
{
	Point g(2.0,3.0);
	BOOST_CHECK( ! g.isEmpty() ) ;
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.x(), 2.0 );
	BOOST_CHECK_EQUAL( g.y(), 3.0 );
}
BOOST_AUTO_TEST_CASE( xyzConstructor )
{
	Point g(2.0,3.0,4.0);
	BOOST_CHECK( ! g.isEmpty() ) ;
	BOOST_CHECK( g.is3D() );
	BOOST_CHECK_EQUAL( g.x(), 2.0 );
	BOOST_CHECK_EQUAL( g.y(), 3.0 );
	BOOST_CHECK_EQUAL( g.z(), 4.0 );
}


//-- toVector_2

BOOST_AUTO_TEST_CASE( emptyToVector_2 )
{
	Point g ;
	CGAL::Vector_2< Kernel > p = g.toVector_2();
	BOOST_CHECK_EQUAL( CGAL::to_double( p.x() ), 0.0 );
	BOOST_CHECK_EQUAL( CGAL::to_double( p.y() ), 0.0 );
}

BOOST_AUTO_TEST_CASE( xyToVector_2 )
{
	Point g( 3.0, 4.0 );
	CGAL::Vector_2< Kernel > p = g.toVector_2();
	BOOST_CHECK_EQUAL( CGAL::to_double( p.x() ), 3.0 );
	BOOST_CHECK_EQUAL( CGAL::to_double( p.y() ), 4.0 );
}

BOOST_AUTO_TEST_CASE( xyToVector_3 )
{
	Point g( 3.0, 4.0 );
	CGAL::Vector_3< Kernel > p = g.toVector_3();

	BOOST_CHECK_EQUAL( CGAL::to_double( p.x() ), 3.0 );
	BOOST_CHECK_EQUAL( CGAL::to_double( p.y() ), 4.0 );
	BOOST_CHECK_EQUAL( CGAL::to_double( p.z() ), 0.0 );
}


//-- asText

BOOST_AUTO_TEST_CASE( asTextEmpty )
{
	Point g;
	BOOST_CHECK_EQUAL( g.asText(1), "POINT EMPTY" );
}

BOOST_AUTO_TEST_CASE( asText2d )
{
	Point g(2.0,3.0);
	BOOST_CHECK_EQUAL( g.asText(3), "POINT(2.000 3.000)" );
}
BOOST_AUTO_TEST_CASE( asText3d )
{
	Point g(2.0,3.0,4.0);
	BOOST_CHECK_EQUAL( g.asText(3), "POINT(2.000 3.000 4.000)" );
}

//-- isPoint

BOOST_AUTO_TEST_CASE( isPoint )
{
	Point g;
	BOOST_CHECK( g.is< Point >() );
}

BOOST_AUTO_TEST_SUITE_END()




