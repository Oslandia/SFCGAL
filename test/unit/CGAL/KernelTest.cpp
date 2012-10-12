#include <iostream>

#include <boost/test/unit_test.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Plane_3.h>

#include <cmath>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel ;
typedef Kernel::Vector_2          Vector_2 ;
typedef Kernel::Vector_3          Vector_3 ;
typedef Kernel::Point_2           Point_2 ;
typedef Kernel::Point_3           Point_3 ;
typedef Kernel::Segment_2         Segment_2 ;
typedef Kernel::Segment_3         Segment_3 ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( CGAL_KernelTest )

BOOST_AUTO_TEST_CASE( testIntersectsRobutness )
{
	Segment_2 ab( Point_2(0.0,0.0) , Point_2(1.0,3.0) );
	Segment_2 cd( Point_2(0.0,1.0) , Point_2(1.0,1.0) );
	Segment_2 ef( Point_2(-1.0,3.0), Point_2(1.0,0.0) );

	CGAL::Object abIcd_ = CGAL::intersection( ab, cd );
	const Point_2 * abIcd = CGAL::object_cast<Point_2>(&abIcd_) ;
	BOOST_REQUIRE( abIcd != NULL );

	CGAL::Object abIef_ = CGAL::intersection( ab, ef );
	const Point_2 * abIef = CGAL::object_cast<Point_2>(&abIef_) ;
	BOOST_REQUIRE( abIef != NULL );

//	std::cout << (*abIcd) << std::endl;
//	std::cout << (*abIef) << std::endl;
	BOOST_CHECK( (*abIcd) == (*abIef) ) ;
}



BOOST_AUTO_TEST_SUITE_END()

