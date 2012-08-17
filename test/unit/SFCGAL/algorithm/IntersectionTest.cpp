#include <boost/test/unit_test.hpp>

#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/all.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_IntersectionTest )

///
/// Function used to compare geometries
/// Since we do not have (yet) a real "equals" operator, we only compare points coordinates
bool operator == ( const Geometry& ga, const Geometry& gb )
{
    BOOST_ASSERT( &ga != 0 );
    BOOST_ASSERT( &gb != 0 );
    detail::GetPointsVisitor get_points_a, get_points_b;
    ga.accept( get_points_a );
    gb.accept( get_points_b );

    if ( get_points_a.points.size() != get_points_b.points.size() )
	return false;

    for ( size_t i = 0; i < get_points_a.points.size(); ++i ) {
	const Point& pta = *(get_points_a.points[i]);
	const Point& ptb = *(get_points_b.points[i]);
	if ( pta != ptb )
	    return false;
    }
    return true;
}

BOOST_AUTO_TEST_CASE( testIntersectionPoint )
{
    // Point x Point
    {
	// The same point
	BOOST_CHECK_EQUAL( *(algorithm::intersection( Point(0,0), Point(0,0) )) == Point(0, 0), true);
	// A different point
	BOOST_CHECK_EQUAL( algorithm::intersection( Point(1,0), Point(0,0) )->isEmpty(), true);
    }

    // Linestring
    {
	LineString ls(Point(0, 0), Point(1, 0));

	// A point on the line string
	Point pt1(Point(0.5, 0));
	// A point outside
	Point pt2(Point(0,1));
	BOOST_CHECK_EQUAL( *(algorithm::intersection( pt1, ls )) == pt1, true);
	BOOST_CHECK_EQUAL( algorithm::intersection( pt2, ls )->isEmpty(), true);

	// A linestring crossing
	LineString ls1(Point(0.5, 0), Point(0.5, 1));
	// A linestring outside
	LineString ls2(Point(0, 1), Point(1, 1));
	// A sub linestring
	LineString ls3(Point(0.5, 0), Point(0.7, 0));

	BOOST_CHECK_EQUAL( *(algorithm::intersection( ls1, ls)) == pt1, true);
	BOOST_CHECK_EQUAL( algorithm::intersection( ls2, ls )->isEmpty(), true );
	BOOST_CHECK_EQUAL( *(algorithm::intersection( ls3, ls)) == ls3, true);
    }

    // Triangle
    {
	Triangle tri( Point(0, 0), Point(1, 1), Point(1, 0));

	// A point on an edge
	BOOST_CHECK_EQUAL( *(algorithm::intersection( Point(0.5,0), tri)) == Point(0.5,0), true);
	// A point inside
	BOOST_CHECK_EQUAL( *(algorithm::intersection( Point(0.5,0.5), tri)) == Point(0.5,0.5), true);
	// A point outside
	BOOST_CHECK_EQUAL( (algorithm::intersection( Point(-1,0), tri))->isEmpty(), true);

	// A linestring crossing
	LineString ls1( Point(0, 0), Point(2, 1));
	BOOST_CHECK_EQUAL( *(algorithm::intersection( ls1, tri)) == LineString(Point(0,0), Point(1, 0.5)), true);
	// A linestring crossing only a vertex
	LineString ls2( Point(0, 0), Point(0, 1));
	BOOST_CHECK_EQUAL( *(algorithm::intersection( ls2, tri)) == Point(0,0), true);	
	// A linestring outside
	LineString ls3( Point(-1, 0), Point(-1, 1));
	BOOST_CHECK_EQUAL( (algorithm::intersection( ls3, tri))->isEmpty(), true );

	// A triangle crossing, resulting in a triangle
	Triangle tri2( Point(0.5, 0), Point(1.5, 1), Point(1.5, 0));
	BOOST_CHECK_EQUAL( *(algorithm::intersection( tri2, tri)) == Triangle(Point(1, 0.5), Point(0.5, 0), Point(1,0)), true );
	// A triangle crossing, resulting in a polygon
	Triangle tri3( Point(0, 0.5), Point(1, 0.5), Point(0, -0.5));
	MultiPoint mp;
	mp.addGeometry(Point(0.5, 0));
	mp.addGeometry(Point(1, 0.5));
	mp.addGeometry(Point(0.5, 0.5));
	mp.addGeometry(Point(0, 0));
	BOOST_CHECK_EQUAL( *(algorithm::intersection( tri3, tri)) == mp, true );
	// A triangle outside
	Triangle tri4( Point(-3, 0), Point(-2, 1), Point(-2, 0));
	BOOST_CHECK_EQUAL( (algorithm::intersection( tri4, tri))->isEmpty(), true );
	// A triangle crossing on an edge
	Triangle tri5( Point(0, 0), Point(1, -1), Point(1, 0));
	BOOST_CHECK_EQUAL( *(algorithm::intersection( tri5, tri)) == LineString(Point(1,0), Point(0,0)), true );
	// A triangle crossing on a vertex
	Triangle tri6( Point(1, 0), Point(2, 1), Point(2, 0));
	BOOST_CHECK_EQUAL( *(algorithm::intersection( tri6, tri)) == Point(1,0), true );
    }
}

BOOST_AUTO_TEST_SUITE_END()

