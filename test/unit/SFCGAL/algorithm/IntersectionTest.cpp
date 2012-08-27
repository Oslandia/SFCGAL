#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/io/GeometryStreams.h>
#include <SFCGAL/all.h>
#include <SFCGAL/transform/AffineTransform3.h>

#include <boost/test/unit_test.hpp>

using namespace SFCGAL;

// always after CGAL
using namespace boost::unit_test ;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

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
	bool found = false;
	for ( size_t j = 0; j < get_points_b.points.size(); ++j ) {
	    const Point& pta = *(get_points_a.points[i]);
	    const Point& ptb = *(get_points_b.points[j]);
	    if ( pta == ptb ) {
		found = true;
		break;
	    }
	}
	if (! found) {
	    return false;
	}
    }
    return true;
}

BOOST_AUTO_TEST_CASE( testIntersectionPoint )
{
    // The same point
    BOOST_CHECK( *(algorithm::intersection( Point(0,0), Point(0,0) )) == Point(0, 0) );
    // A different point
    BOOST_CHECK( algorithm::intersection( Point(1,0), Point(0,0) )->isEmpty() );
}

BOOST_AUTO_TEST_CASE( testIntersectionLineString )
{
    LineString ls(Point(0, 0), Point(1, 0));
    
    // A point on the line string
    Point pt1(Point(0.5, 0));
    // A point outside
    Point pt2(Point(0,1));
    BOOST_CHECK( *(algorithm::intersection( pt1, ls )) == pt1 );
    BOOST_CHECK( algorithm::intersection( pt2, ls )->isEmpty() );
    
    // A linestring crossing
    LineString ls1(Point(0.5, 0), Point(0.5, 1));
    // A linestring outside
    LineString ls2(Point(0, 1), Point(1, 1));
    // A sub linestring
    LineString ls3(Point(0.5, 0), Point(0.7, 0));
    
    BOOST_CHECK( *(algorithm::intersection( ls1, ls)) == pt1 );
    BOOST_CHECK( algorithm::intersection( ls2, ls )->isEmpty() );
    BOOST_CHECK( *(algorithm::intersection( ls3, ls)) == ls3 );
}

BOOST_AUTO_TEST_CASE( testIntersectionTriangle )
{
    Triangle tri( Point(0, 0), Point(1, 1), Point(1, 0));
    
    // A point on an edge
    BOOST_CHECK( *(algorithm::intersection( Point(0.5,0), tri)) == Point(0.5,0) );
    // A point inside
    BOOST_CHECK( *(algorithm::intersection( Point(0.5,0.5), tri)) == Point(0.5,0.5) );
    // A point outside
    BOOST_CHECK( (algorithm::intersection( Point(-1,0), tri))->isEmpty() );
    
    // A linestring crossing
    LineString ls1( Point(0, 0), Point(2, 1));
    BOOST_CHECK( *(algorithm::intersection( ls1, tri)) == LineString(Point(0,0), Point(1, 0.5)) );
    // A linestring crossing only a vertex
    LineString ls2( Point(0, 0), Point(0, 1));
    BOOST_CHECK( *(algorithm::intersection( ls2, tri)) == Point(0,0) );
    // A linestring outside
    LineString ls3( Point(-1, 0), Point(-1, 1));
    BOOST_CHECK( (algorithm::intersection( ls3, tri))->isEmpty() );
    
    // A triangle crossing, resulting in a triangle
    Triangle tri2( Point(0.5, 0), Point(1.5, 1), Point(1.5, 0));
    BOOST_CHECK( *(algorithm::intersection( tri2, tri)) == Triangle(Point(1, 0.5), Point(0.5, 0), Point(1,0)) );
    // A triangle crossing, resulting in a polygon
    Triangle tri3( Point(0, 0.5), Point(1, 0.5), Point(0, -0.5));
    MultiPoint mp;
    mp.addGeometry(Point(0.5, 0));
    mp.addGeometry(Point(1, 0.5));
    mp.addGeometry(Point(0.5, 0.5));
    mp.addGeometry(Point(0, 0));
    BOOST_CHECK( *(algorithm::intersection( tri3, tri)) == mp );
    // A triangle outside
    Triangle tri4( Point(-3, 0), Point(-2, 1), Point(-2, 0));
    BOOST_CHECK( (algorithm::intersection( tri4, tri))->isEmpty() );
    // A triangle crossing on an edge
    Triangle tri5( Point(0, 0), Point(1, -1), Point(1, 0));
    BOOST_CHECK( *(algorithm::intersection( tri5, tri)) == LineString(Point(1,0), Point(0,0)) );
    // A triangle crossing on a vertex
    Triangle tri6( Point(1, 0), Point(2, 1), Point(2, 0));
    BOOST_CHECK( *(algorithm::intersection( tri6, tri)) == Point(1,0) );
}

BOOST_AUTO_TEST_CASE( testIntersectionPolygon )
{
    // very simple intersection
    // TODO: complete
    std::auto_ptr<Geometry> poly(io::readWkt("POLYGON((0 0,2 0,2 2,0 2,0 0),(1.3 1.3,1.3 1.7,1.7 1.7,1.7 1.3,1.3 1.3))"));

    std::auto_ptr<Geometry> poly1(io::readWkt("POLYGON((1 1,3 1,3 3,1 3,1 1))"));
    std::auto_ptr<Geometry> inter_1 = algorithm::intersection( *poly, *poly1 );
    BOOST_CHECK( *inter_1 == *(io::readWkt("POLYGON((2 2,1 2,1 1,2 1),(1.7 1.7,1.7 1.3,1.3 1.3,1.3 1.7))")) );
}

BOOST_AUTO_TEST_CASE( testIntersectionSolid )
{
    // 3D intersection with a solid
    std::string cubestr = "SOLID("
	// exterior shell
	"("
	"((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0))," // front face
	"((1 0 0,1 1 0,1 1 1,1 0 1,1 0 0))," // right face
	"((0 1 0,0 1 1,1 1 1,1 1 0,0 1 0))," // top face
	"((0 0 1,0 1 1,0 1 0,0 0 0,0 0 1))," // left face
	"((1 0 1,1 1 1,0 1 1,0 0 1,1 0 1))," // back face
	"((1 0 0,1 0 1,0 0 1,0 0 0,1 0 0))" // bottom face
	"))";
    std::auto_ptr<Geometry> cube( io::readWkt( cubestr ));

    //
    // Point x cube

    // intersection (2D) on solid is illegal
    bool has_thrown = false;
    try {
	algorithm::intersection( Point(0, 0), *cube );
    }
    catch ( std::exception& e ) {
	has_thrown = true;
    }
    BOOST_CHECK_EQUAL( has_thrown, true );

    // Consider cube with holes
    // TODO

    //
    // LineString x cube

    // A linestring partly inside
    // TODO

    // A linestring completely inside
    // TODO

    // A linestring outside
    // TODO

    // A linestring crossing on a vertex / edge / face
    // TODO

    // triangle x cube
    //

    // A triangle partly inside
    Triangle tri1(Point(-0.5, 0.5, 0), Point(0.5, 0.5, 0.5), Point(-0.5, 0.5, 1));
    BOOST_CHECK( *(algorithm::intersection3D( tri1, *cube )) == *(io::readWkt("POLYGON((0 0.5 0.75,0.5 0.5 0.5,0 0.5 0.25,0 0.5 0.5,0 0.5 0.75))")) );
    // A triangle completely inside
    Triangle tri2(Point(0.2, 0.2, 0.2), Point(0.8, 0.2, 0.2), Point(0.8, 0.2, 0.8));
    BOOST_CHECK( *(algorithm::intersection3D(tri2, *cube)) == tri2 );

    // A triangle completely outside
    Triangle tri3(Point(-0.5, 1.5, 0), Point(0.5, 1.5, 0.5), Point(-0.5, 1.5, 1));
    BOOST_CHECK( algorithm::intersection3D(tri3, *cube)->isEmpty());

    // A triangle partly touching a face
    Triangle tri3b(Point(-0.5, 0.0, 0.0), Point(1.5, 0.0, 0.0), Point(0.0, 1.0, 0.0));
    std::auto_ptr<Geometry> inter1 = algorithm::intersection3D( tri3b, *cube );
    //    BOOST_CHECK( algorithm::intersection3D( tri3b, *cube )->geometryTypeId() == TYPE_TRIANGLE );

    // A triangle touching a face
    Triangle tri4(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
    BOOST_CHECK( algorithm::intersection3D( tri4, *cube )->envelope() == Envelope( Coordinate(0, 0, 0), Coordinate(1, 1, 0)) );

    // A triangle touching an edge
    Triangle tri5(Point(0.0, 0.0, 0.0), Point(0.0, 1.0, 0.0), Point(-1.0, 0.0, 0.0));
    BOOST_CHECK( *algorithm::intersection3D( tri5, *cube ) == LineString(Point(0, 0, 0), Point(0, 1, 0)) );

    // A triangle touching a vertex
    Triangle tri6(Point(0.0, 0.0, 0.0), Point(-1.0, 1.0, 0.0), Point(-1.0, 0.0, 0.0));
    BOOST_CHECK( *algorithm::intersection3D( tri6, *cube ) == Point(0, 0, 0) );

    // Polygon x Solid
    {
	// special case : when the intersection is a multipoint or a multilinestring
	// multi triangles ??
	std::auto_ptr<Geometry> polygon1 = io::readWkt("POLYGON((0.2 0.5 0, 0.2 0.5 -1, 0.8 0.5 -1, 0.8 0.5 0, 0.5 0.5 -0.5, 0.2 0.5 0))");

	std::auto_ptr<Geometry> inter = algorithm::intersection3D( *polygon1, *cube );
	BOOST_CHECK( *inter == *io::readWkt("GEOMETRYCOLLECTION(POINT(0.8 0.5 0),POINT(0.2 0.5 0))"));
    }

    // Solid x Solid
    {
	Solid solidb( static_cast<Solid&>(*cube));

	CGAL::Vector_3<Kernel> tv( CGAL::Point_3<Kernel>( 0.0, 0.0, 0.0 ),
				   CGAL::Point_3<Kernel>( 0.5, 0.0, 0.0 ));
	transform::AffineTransform3<Kernel> t( CGAL::Aff_transformation_3<Kernel>( CGAL::Translation(),
										    tv ));
	t.transform( solidb );

	// Another solid crossing
	std::auto_ptr<Geometry> inter = algorithm::intersection3D( solidb, *cube );
	BOOST_CHECK( inter->envelope() ==  Envelope( Coordinate( 0.5, 0, 0 ), Coordinate( 1.0, 1.0, 1.0)));
    }
}

BOOST_AUTO_TEST_SUITE_END()

