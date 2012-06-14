#include <boost/test/unit_test.hpp>

#include <cmath>

#include <CGAL/Cartesian.h>


typedef CGAL::Cartesian< double > Kernel ;
typedef Kernel::Vector_2          Vector_2 ;
typedef Kernel::Vector_3          Vector_3 ;
typedef Kernel::Point_2           Point_2 ;
typedef Kernel::Point_3           Point_3 ;
typedef CGAL::Segment_2< Kernel > Segment_2 ;
typedef CGAL::Segment_3< Kernel > Segment_3 ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( CGAL_WorkWithPoint3AsPoint2 )

BOOST_AUTO_TEST_CASE( testIntersect2dSegmentsIn2d )
{
	Point_2 a( 0.0, 0.0 );
	Point_2 b( 1.0, 1.0 );
	Point_2 c( 1.0, 0.0 );
	Point_2 d( 0.0, 1.0 );

	Segment_2 ab( a, b );
	Segment_2 cd( c, d );

	CGAL::Object obj = CGAL::intersection( ab, cd );
	const Point_2 *p = CGAL::object_cast< Point_2 >(&obj) ;
	BOOST_REQUIRE( p != NULL );
	BOOST_CHECK_EQUAL( p->x(), 0.5 );
	BOOST_CHECK_EQUAL( p->y(), 0.5 );
}

BOOST_AUTO_TEST_CASE( testIntersect3dSegmentsIn3d )
{
	Point_3 a( 0.0, 0.0, 0.0 );
	Point_3 b( 1.0, 1.0, 1.0 );
	Point_3 c( 1.0, 0.0, 0.0 );
	Point_3 d( 0.0, 1.0, 1.0 );

	Segment_3 ab( a, b );
	Segment_3 cd( c, d );

	CGAL::Object obj = CGAL::intersection( ab, cd );
	const Point_3 *p = CGAL::object_cast< Point_3 >(&obj) ;
	BOOST_REQUIRE( p != NULL );
	BOOST_CHECK_EQUAL( p->x(), 0.5 );
	BOOST_CHECK_EQUAL( p->y(), 0.5 );
	BOOST_CHECK_EQUAL( p->z(), 0.5 );
}


/**
 *
 */
BOOST_AUTO_TEST_CASE( testIntersect3dSegmentsIn2d )
{
	Point_3 a( 0.0, 0.0, 0.0 );
	Point_3 b( 1.0, 1.0, 1.0 );
	Point_3 c( 1.0, 0.0, 0.0 );
	Point_3 d( 0.0, 1.0, 1.0 );

	Segment_3 ab( a, b );
	Segment_3 cd( c, d );

	//TODO : Find the best way to do 2D intersection on 3D segments

	/*
	CGAL::Object obj = CGAL::intersection(

	);
	const Point_2 *p = CGAL::object_cast< Point_2 >(&obj) ;
	BOOST_REQUIRE( p != NULL );
	BOOST_CHECK_EQUAL( p->x(), 0.5 );
	BOOST_CHECK_EQUAL( p->y(), 0.5 );
	*/
}



BOOST_AUTO_TEST_SUITE_END()

