#include <iostream>

#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Coordinate.h>
#include <SFCGAL/LineString.h>

using namespace SFCGAL ;

typedef Kernel::Vector_2          Vector_2 ;
typedef Kernel::Vector_3          Vector_3 ;
typedef Kernel::Point_2           Point_2 ;
typedef Kernel::Point_3           Point_3 ;
typedef Kernel::Segment_2         Segment_2 ;
typedef Kernel::Segment_3         Segment_3 ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_KernelTest )


/**
 * 3 lines intersecting on POINT(1/3 1)
 */
BOOST_AUTO_TEST_CASE( testIntersectsRobutness )
{
	LineString ab( Point(0.0,0.0),  Point(1.0,3.0) );
	LineString cd( Point(0.0,1.0) , Point(1.0,1.0) );
	LineString ef( Point(-1.0,3.0), Point(1.0,0.0) );

	//ab, cd
	CGAL::Object abIcd_ = CGAL::intersection(
		Segment_2( ab.startPoint().toPoint_2(), ab.endPoint().toPoint_2() ),
		Segment_2( cd.startPoint().toPoint_2(), cd.endPoint().toPoint_2() )
	);
	const Point_2 * abIcd = CGAL::object_cast<Point_2>(&abIcd_) ;
	BOOST_REQUIRE( abIcd != NULL );

	//would break robustness if construction history is lost
	Point intersectionA( *abIcd );

	CGAL::Object abIef_ = CGAL::intersection(
		intersectionA.toPoint_2(),
		Segment_2( ef.startPoint().toPoint_2(), ef.endPoint().toPoint_2() )
	);
	const Point_2 * abIef = CGAL::object_cast<Point_2>(&abIef_) ;
	BOOST_REQUIRE( abIef != NULL );

	Point intersectionB( *abIef );

	BOOST_CHECK_EQUAL( intersectionA.toPoint_2(), intersectionB.toPoint_2() );
}



BOOST_AUTO_TEST_SUITE_END()

