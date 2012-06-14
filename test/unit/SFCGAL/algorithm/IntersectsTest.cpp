#include <boost/test/unit_test.hpp>

#include <cmath>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/algorithm/intersects.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_IntersectsTest )

BOOST_AUTO_TEST_CASE( testPointPointIntersects )
{
    Point pta( 0.0, 1.0, 0.0 );
    Point ptb( 0.0, 1.0, 0.0 );
    Point ptc( 0.0, 0.0, 0.0 );
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, ptb ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, ptc ), false );
}

BOOST_AUTO_TEST_CASE( testPointLineStringIntersects )
{
    Point pta( 0.0, 0.0, 0.0 );
    Point ptb( 0.5, 0.0, 0.0 );
    Point ptc( 0.0, 0.0, 1.0 );
    std::auto_ptr<Geometry> ls = io::readWkt( "LINESTRING(0 0 0, 1 0 0, 1 1 0)" );
    // point on an extremity
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, *ls ), true );
    // point on a line
    BOOST_CHECK_EQUAL( algorithm::intersects( ptb, *ls ), true );
    // point not on a line
    BOOST_CHECK_EQUAL( algorithm::intersects( ptc, *ls ), false );
    // symmetric calls
}

BOOST_AUTO_TEST_CASE( testPointTriangleIntersects )
{
    Point pta( 0.0, 0.0, 0.0 );
    Point ptb( 0.5, 0.0, 0.0 );
    Point ptc( 0.0, 0.0, 1.0 );
    Point ptd( 0.5, 0.5, 0.0 );
    Triangle tri( Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0) );
    // point on a vertex
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, tri ), true );
    // point on a boundary
    BOOST_CHECK_EQUAL( algorithm::intersects( ptb, tri ), true );
    // point not inside
    BOOST_CHECK_EQUAL( algorithm::intersects( ptc, tri ), false );
    // point inside the triangle
    BOOST_CHECK_EQUAL( algorithm::intersects( ptd, tri ), true );
}

BOOST_AUTO_TEST_CASE( testPointPolygonIntersects )
{
    Point pta( 0.0, 0.0 );
    Point ptb( 0.5, 0.0 );
    Point ptc( 0.0, 1.5 );
    Point ptd( 0.5, 0.5 );

    // a square
    std::vector<Point> ring;
    ring.push_back( Point(0.0, 0.0) );
    ring.push_back( Point(1.0, 0.0) );
    ring.push_back( Point(1.0, 1.0) );
    ring.push_back( Point(0.0, 1.0) );
    ring.push_back( Point(0.0, 0.0) );
    LineString ext(ring);
    Polygon poly( ext );

    // point on a vertex
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, poly ), true );
    // point on a boundary
    BOOST_CHECK_EQUAL( algorithm::intersects( ptb, poly ), true );
    // point outside
    BOOST_CHECK_EQUAL( algorithm::intersects( ptc, poly ), false );
    // point inside
    BOOST_CHECK_EQUAL( algorithm::intersects( ptd, poly ), true );
}

BOOST_AUTO_TEST_CASE( testPointPolygon3DIntersects )
{
    Point pta( 0.0, 0.0 );
    Point ptb( 0.5, 0.0 );
    Point ptc( 0.0, 1.5 );
    Point ptd( 0.5, 0.5 );

    // a square
    std::vector<Point> ring;
    ring.push_back( Point(0.0, 0.0, 0.0) );
    ring.push_back( Point(1.0, 0.0, 0.0) );
    ring.push_back( Point(1.0, 1.0, 0.0) );
    ring.push_back( Point(0.0, 1.0, 0.0) );
    ring.push_back( Point(0.0, 0.0, 0.0) );
    LineString ext(ring);
    Polygon poly( ext );

    // point on a vertex
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, poly ), true );
    // point on a boundary
    BOOST_CHECK_EQUAL( algorithm::intersects( ptb, poly ), true );
    // point outside
    BOOST_CHECK_EQUAL( algorithm::intersects( ptc, poly ), false );
    // point inside
    BOOST_CHECK_EQUAL( algorithm::intersects( ptd, poly ), true );
}

BOOST_AUTO_TEST_CASE( testLineStringLineStringIntersects )
{
    // 2D
    {
	std::auto_ptr<Geometry> ls = io::readWkt( "LINESTRING(0 0, 1 0, 1 1)" );
	std::auto_ptr<Geometry> lsb = io::readWkt( "LINESTRING(0 0, 1 0, 1 4)" );
	std::auto_ptr<Geometry> lsc = io::readWkt( "LINESTRING(10 0, 11 0, 11 1)" );
	
	BOOST_CHECK_EQUAL( algorithm::intersects( *ls, *lsb ), true );
	BOOST_CHECK_EQUAL( algorithm::intersects( *lsc, *ls ), false );
    }
    // 3D
    {
	std::auto_ptr<Geometry> ls = io::readWkt( "LINESTRING(0 0 0, 1 0 0, 1 1 0)" );
	std::auto_ptr<Geometry> lsb = io::readWkt( "LINESTRING(0 0 0, 1 0 1, 1 4 0)" );
	std::auto_ptr<Geometry> lsc = io::readWkt( "LINESTRING(10 0 0, 11 0 0, 11 1 0)" );
	
	BOOST_CHECK_EQUAL( algorithm::intersects( *ls, *lsb ), true );
	BOOST_CHECK_EQUAL( algorithm::intersects( *lsc, *ls ), false );
    }
}

BOOST_AUTO_TEST_CASE( testLineStringTriangleIntersects )
{
    Triangle tri( Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(1.0, 1.0, 0.0) );
    std::auto_ptr<Geometry> ls = io::readWkt( "LINESTRING(0 0 0, 1 0 0, 1 1 0)" );
    std::auto_ptr<Geometry> lsb = io::readWkt( "LINESTRING(0 0 0, 1 0 1, 1 4 0)" );
    std::auto_ptr<Geometry> lsc = io::readWkt( "LINESTRING(10 0 0, 11 0 0, 11 1 0)" );
    
    BOOST_CHECK_EQUAL( algorithm::intersects( *ls, tri ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects( *lsb, tri ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects( *lsc, tri ), false );
}
BOOST_AUTO_TEST_SUITE_END()

