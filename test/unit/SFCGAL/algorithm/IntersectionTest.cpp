/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/io/GeometryStreams.h>
#include <SFCGAL/all.h>
#include <SFCGAL/transform/AffineTransform3.h>

#include <boost/test/unit_test.hpp>

using namespace SFCGAL;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_IntersectionTest )

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
    Triangle tri( Point(0, 0), Point(1, 0), Point(1, 1));
    
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
    Triangle tri2( Point(0.5, 0), Point(1.5, 0), Point(1.5, 1));
    BOOST_CHECK( *(algorithm::intersection( tri2, tri)) == Triangle(Point(1, 0.5), Point(0.5, 0), Point(1,0)) );
    // A triangle crossing, resulting in a polygon
    Triangle tri3( Point(0, 0.5), Point(1, 0.5), Point(0, -0.5));

    std::auto_ptr<Geometry> mp = io::readWkt( "POLYGON((0 0,0.5 0,1 0.5,0.5 0.5,0 0))" );
    BOOST_CHECK( *(algorithm::intersection( tri3, tri ) ) == *mp );
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
	// a square with a hole
	std::auto_ptr<Geometry> poly(io::readWkt("POLYGON((0 0,2 0,2 2,0 2,0 0),(1.3 1.3,1.3 1.7,1.7 1.7,1.7 1.3,1.3 1.3))"));
	BOOST_CHECK_EQUAL( poly->asText(1), "POLYGON((0.0 0.0,2.0 0.0,2.0 2.0,0.0 2.0,0.0 0.0),(1.3 1.3,1.3 1.7,1.7 1.7,1.7 1.3,1.3 1.3))" );

	// point x polygon
	{
		// a point inside
		BOOST_CHECK( *(algorithm::intersection( Point(QT(1,10), QT(1,10)), *poly)) == Point(QT(1,10), QT(1,10)));
		// a point on an edge
		BOOST_CHECK( *(algorithm::intersection( Point(QT(1,10), 0), *poly)) == Point(QT(1,10), 0));
		// a point on a vertex
		BOOST_CHECK( *(algorithm::intersection( Point(0, 0), *poly)) == Point(0, 0));
		// a point on an interior edge
		BOOST_CHECK( *(algorithm::intersection( Point(QT(15,10), QT(13,10)), *poly)) == Point(QT(15,10), QT(13,10)));
		// a point in a hole
		BOOST_CHECK( (algorithm::intersection( Point(QT(15,10), QT(15,10)), *poly))->isEmpty() );
		// a point outside
		BOOST_CHECK( (algorithm::intersection( Point(QT(25,10), QT(25,10)), *poly))->isEmpty() );
	}
	// linestring x polygon
	{
		LineString ls1( Point(-0.5, 0.5), Point(2.5, 0.5));
		// a linestring inside
		BOOST_CHECK(  ! (algorithm::intersection( ls1, *poly ))->isEmpty() );

		LineString ls2( Point(0.0, 0.0), Point(1.0, 0.0));
		// a linestring on an edge
		BOOST_CHECK(  ! (algorithm::intersection( ls2, *poly ))->isEmpty() );

		LineString ls3( Point(-0.5, 0.0), Point(1.0, -2.0));
		// a linestring outside
		BOOST_CHECK(  (algorithm::intersection( ls3, *poly ))->isEmpty() );
	}
	// triangle x polygon
	// TODO

	// polygon x polygon
	{
		std::auto_ptr<Geometry> poly1(io::readWkt("POLYGON((1 1,3 1,3 3,1 3,1 1))"));
		std::auto_ptr<Geometry> inter_1 = algorithm::intersection( *poly, *poly1 );
		BOOST_CHECK( *inter_1 == *(io::readWkt("POLYGON((2 2,1 2,1 1,2 1,2 2),(1.7 1.7,1.7 1.3,1.3 1.3,1.3 1.7,1.7 1.7))")) );
	}
}

BOOST_AUTO_TEST_CASE( testIntersection3DPolygon )
{
    std::auto_ptr<Geometry> square(io::readWkt("POLYGON((0 0 0.1,1 0 0.1,1 1 0.1,0 1 0.1,0 0 0.1))"));

    // linestring crossing
    LineString ls( Point(QT(1,2), QT(1,2), -QT(1,2)), Point(QT(1,2), QT(1,2), QT(1,10)));
    BOOST_CHECK( *algorithm::intersection3D( ls, *square ) == Point(QT(1,2), QT(1,2), QT(1,10)) );
    // linestring on a triangulation edge
    LineString ls2( Point(QT(1,10), QT(9,10), QT(1,10)), Point(QT(9,10), QT(1,10), QT(1,10)));
    BOOST_CHECK( *algorithm::intersection3D( ls2, *square ) == ls2 );
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
    {
	// A linestring partly inside
	LineString ls1;
	ls1.addPoint( Point(-0.5, 0.5, 0.5 ));
	ls1.addPoint( Point(0.5, 0.5, 0.5 ));
	std::cout << "ls1, cube: " << algorithm::intersection3D( ls1, *cube )->asText() << std::endl;
	BOOST_CHECK( *algorithm::intersection3D( ls1, *cube ) == LineString(Point(0, 0.5, 0.5), Point(0.5, 0.5, 0.5)) );

	// A linestring completely inside
	LineString ls2;
	ls2.addPoint( Point(0.2, 0.5, 0.5 ));
	ls2.addPoint( Point(0.8, 0.5, 0.5 ));
	std::cout << "ls2, cube: " << algorithm::intersection3D( ls2, *cube )->asText() << std::endl;
	BOOST_CHECK( *algorithm::intersection3D( ls2, *cube ) == ls2 );

	// A linestring with more than one segment
	ls1.addPoint( Point(QT(8,10), QT(5,10), QT(5,10)));
	ls1.addPoint( Point(QT(15,10), QT(5,10), QT(5,10)));
	std::cout << "ls1b, cube: " << algorithm::intersection3D( ls1, *cube )->asText() << std::endl;
	BOOST_CHECK( *algorithm::intersection3D( ls1, *cube ) == *io::readWkt("LINESTRING(-0 0.5 0.5,0.5 0.5 0.5,0.8 0.5 0.5,1 0.5 0.5)") );

	// A linestring crossing on two vertices
	LineString ls3;
	ls3.addPoint( Point(0.0, 0.0, 0.0));
	ls3.addPoint( Point(0.5, 0.0, -0.5));
	ls3.addPoint( Point(1.0, 0.0, 0.0));
	GeometryCollection coll1;
	coll1.addGeometry( Point( 0, 0, 0 ));
	coll1.addGeometry( Point( 1, 0, 0 ));
	std::cout << "ls3, cube: " << algorithm::intersection3D( ls3, *cube )->asText() << std::endl;
	BOOST_CHECK( *algorithm::intersection3D( ls3, *cube ) == coll1 );
	
	// A linestring crossing on an edge
	LineString ls4;
	ls4.addPoint( Point(0.2, 0.0, 0.0));
	ls4.addPoint( Point(0.8, 0.0, 0.0));
	ls4.addPoint( Point(0.8, 0.5, 0.5));
	std::cout << "ls4, cube: " << algorithm::intersection3D( ls4, *cube )->asText() << std::endl;
	BOOST_CHECK( *algorithm::intersection3D( ls4, *cube ) == ls4 );

	// A linestring crossing on a vertex
	LineString ls5;
	ls5.addPoint( Point(-0.5, 0.0, 0.5));
	ls5.addPoint( Point(0.5, 0.0, -0.5));
	ls5.addPoint( Point(1.0, 0.5, -0.5));
	std::cout << "ls5, cube: " << algorithm::intersection3D( ls5, *cube )->asText() << std::endl;
	BOOST_CHECK( *algorithm::intersection3D( ls5, *cube ) == Point(0, 0, 0));

	// A linestring outside
	LineString ls6;
	ls6.addPoint( Point( -0.5, 0.0, 0.5 ));
	ls6.addPoint( Point( 0.2, 0.0, -0.5 ));
	std::cout << "ls6, cube: " << algorithm::intersection3D( ls6, *cube )->asText() << std::endl;
	BOOST_CHECK( algorithm::intersection3D( ls6, *cube )->isEmpty() );
    }

    // triangle x cube
    //
    {
	// A triangle partly inside
	Triangle tri1(Point(-0.5, 0.5, 0), Point(0.5, 0.5, 0.5), Point(-0.5, 0.5, 1));
	    std::cout << "tri, cube: " << algorithm::intersection3D( tri1, *cube )->asText() << std::endl;
	BOOST_CHECK( *(algorithm::intersection3D( tri1, *cube )) == *(io::readWkt("POLYGON((0 0.5 0.75,0.5 0.5 0.5,0 0.5 0.25,0 0.5 0.5,0 0.5 0.75))")) );
	// A triangle completely inside
	Triangle tri2(Point(0.2, 0.2, 0.2), Point(0.8, 0.2, 0.2), Point(0.8, 0.2, 0.8));
	BOOST_CHECK( *(algorithm::intersection3D(tri2, *cube)) == tri2 );
	
	// A triangle completely outside
	Triangle tri3(Point(-0.5, 1.5, 0), Point(0.5, 1.5, 0.5), Point(-0.5, 1.5, 1));
	BOOST_CHECK( algorithm::intersection3D(tri3, *cube)->isEmpty());
	
	// A triangle partly touching a face
	Triangle tri3b(Point(-0.5, 0.0, 0.0), Point(1.5, 0.0, 0.0), Point(0.0, 1.0, 0.0));
	//	std::auto_ptr<Geometry> inter1 = algorithm::intersection3D( tri3b, *cube );
	//	std::cout << inter1->asText() << std::endl;
	BOOST_CHECK( algorithm::intersection3D( tri3b, *cube )->geometryTypeId() == TYPE_POLYGON );
	
	// A triangle touching a face
	Triangle tri4(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
	BOOST_CHECK( algorithm::intersection3D( tri4, *cube )->envelope() == Envelope( Coordinate(0, 0, 0), Coordinate(1, 1, 0)) );
	
	// A triangle touching an edge
	Triangle tri5(Point(0.0, 0.0, 0.0), Point(0.0, 1.0, 0.0), Point(-1.0, 0.0, 0.0));
	BOOST_CHECK( *algorithm::intersection3D( tri5, *cube ) == LineString(Point(0, 0, 0), Point(0, 1, 0)) );
	
	// A triangle touching a vertex
	Triangle tri6(Point(0.0, 0.0, 0.0), Point(-1.0, 1.0, 0.0), Point(-1.0, 0.0, 0.0));
	BOOST_CHECK( *algorithm::intersection3D( tri6, *cube ) == Point(0, 0, 0) );
    }
	
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


BOOST_AUTO_TEST_CASE( testIntersectionDegenerateSegment )
{
	// pôlygon with a doubled point (10 0)
	std::auto_ptr<Geometry> gA = io::readWkt("POLYGON((0 0,10 0,10 0,10 10,0 10,0 0))");
	std::auto_ptr<Geometry> gB = io::readWkt("POLYGON((0 0,5 0,5 5,0 5,0 0))");

	// check it does not assert
	std::auto_ptr<Geometry> gI = algorithm::intersection( *gA, *gB );
}

BOOST_AUTO_TEST_CASE( testIntersectionCleanup )
{
	// pôlygon with a doubled point (10 0)
	std::auto_ptr<Geometry> gA = io::readWkt("LINESTRING(0 0,0 10,10 10,10 0)");
	std::auto_ptr<Geometry> gB = io::readWkt("LINESTRING(10 10 4,10 0 5,0 0 5)");

	std::auto_ptr<Geometry> gI = algorithm::intersection( *gA, *gB );
	BOOST_CHECK( gI->geometryTypeId() == SFCGAL::TYPE_GEOMETRYCOLLECTION );
	BOOST_CHECK( gI->as<GeometryCollection>().numGeometries() == 2 );
}
BOOST_AUTO_TEST_SUITE_END()

