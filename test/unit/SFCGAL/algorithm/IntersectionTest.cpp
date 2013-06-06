/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
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
#include <SFCGAL/detail/io/WktWriter.h>
#include <SFCGAL/io/GeometryStreams.h>
#include <SFCGAL/all.h>
#include <SFCGAL/detail/transform/AffineTransform3.h>

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

    // Consider cube with holes
    // TODO

    //
    // LineString x cube
    {
	// A linestring partly inside
	LineString ls1;
	ls1.addPoint( Point(-0.5, 0.5, 0.5 ));
	ls1.addPoint( Point(0.5, 0.5, 0.5 ));
	BOOST_CHECK( *algorithm::intersection3D( ls1, *cube ) == LineString(Point(0, 0.5, 0.5), Point(0.5, 0.5, 0.5)) );

	// A linestring completely inside
	LineString ls2;
	ls2.addPoint( Point(0.2, 0.5, 0.5 ));
	ls2.addPoint( Point(0.8, 0.5, 0.5 ));
	BOOST_CHECK( *algorithm::intersection3D( ls2, *cube ) == ls2 );

	// A linestring with more than one segment
	ls1.addPoint( Point(QT(8,10), QT(5,10), QT(5,10)));
	ls1.addPoint( Point(QT(15,10), QT(5,10), QT(5,10)));
	BOOST_CHECK( *algorithm::intersection3D( ls1, *cube ) == *io::readWkt("LINESTRING(-0 0.5 0.5,0.5 0.5 0.5,0.8 0.5 0.5,1 0.5 0.5)") );

	// A linestring crossing on two vertices
	LineString ls3;
	ls3.addPoint( Point(0.0, 0.0, 0.0));
	ls3.addPoint( Point(0.5, 0.0, -0.5));
	ls3.addPoint( Point(1.0, 0.0, 0.0));
	GeometryCollection coll1;
	coll1.addGeometry( Point( 0, 0, 0 ));
	coll1.addGeometry( Point( 1, 0, 0 ));
	BOOST_CHECK( *algorithm::intersection3D( ls3, *cube ) == coll1 );

	// A linestring crossing on an edge
	LineString ls4;
	ls4.addPoint( Point(0.2, 0.0, 0.0));
	ls4.addPoint( Point(0.8, 0.0, 0.0));
	ls4.addPoint( Point(0.8, 0.5, 0.5));
	BOOST_CHECK( *algorithm::intersection3D( ls4, *cube ) == ls4 );
	// A linestring crossing on a vertex
	LineString ls5;
	ls5.addPoint( Point(-0.5, 0.0, 0.5));
	ls5.addPoint( Point(0.5, 0.0, -0.5));
	ls5.addPoint( Point(1.0, 0.5, -0.5));
	BOOST_CHECK( *algorithm::intersection3D( ls5, *cube ) == Point(0, 0, 0));

	// A linestring outside
	LineString ls6;
	ls6.addPoint( Point( -0.5, 0.0, 0.5 ));
	ls6.addPoint( Point( 0.2, 0.0, -0.5 ));
	BOOST_CHECK( algorithm::intersection3D( ls6, *cube )->isEmpty() );
    }

    // triangle x cube
    //
    {
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
	// polygon with a doubled point (10 0)
	std::auto_ptr<Geometry> gA = io::readWkt("LINESTRING(0 0,0 10,10 10,10 0)");
	std::auto_ptr<Geometry> gB = io::readWkt("LINESTRING(10 10 4,10 0 5,0 0 5)");

	std::auto_ptr<Geometry> gI = algorithm::intersection( *gA, *gB );
	BOOST_CHECK( gI->geometryTypeId() == SFCGAL::TYPE_GEOMETRYCOLLECTION );
	BOOST_CHECK( gI->as<GeometryCollection>().numGeometries() == 2 );
}

BOOST_AUTO_TEST_CASE( testIntersectionEmpty )
{
	std::auto_ptr<Geometry> gA = io::readWkt("POLYGON EMPTY");
	std::auto_ptr<Geometry> gB = io::readWkt("LINESTRING(10 10 4,10 0 5,0 0 5)");

	std::auto_ptr<Geometry> gI = algorithm::intersection( *gA, *gB );
}




BOOST_AUTO_TEST_CASE( testIntersection_invalidOrientation )
{
	std::auto_ptr<Geometry> gA = io::readWkt("POLYGON((8669379559606881/281474976710656 7916317869408501/140737488355328,270918111237715/8796093022208 7916317869408501/140737488355328,2134169211442127/70368744177664 7970605343251471/140737488355328,4160631594687399/140737488355328 4029364555888085/70368744177664,6116394148693517/281474976710656 8960720992054877/140737488355328,6073978156073765/281474976710656 2254267966426095/35184372088832,8325777969759013/281474976710656 3943117826690031/35184372088832,8376427659504235/281474976710656 7906080332407857/70368744177664,1330304241385973/35184372088832 4146843334168311/35184372088832,1251660450585549/35184372088832 5276613261500369/35184372088832,2507841106470861/70368744177664 2642817822536509/17592186044416,1261061637519677/35184372088832 661449400940061/4398046511104,936076078434355/17592186044416 2768015707630051/17592186044416,3799727381694151/70368744177664 693389503606431/4398046511104,3831242048199649/70368744177664 2772104912814475/17592186044416,1272370488382681/17592186044416 2481824777220057/17592186044416,5095624474239005/70368744177664 619958788416741/4398046511104,2861797619274101/35184372088832 4369759637199047/35184372088832,128528983275669/1099511627776 6991505962428365/70368744177664,1029663620939819/8796093022208 871984914298081/8796093022208,8236834388205243/70368744177664 1739127554548669/17592186044416,8224628789629133/70368744177664 6941463275776369/70368744177664,5268115402867955/70368744177664 5084688203973297/70368744177664,5236783134358927/70368744177664 4920193794300901/70368744177664,2616143591630197/35184372088832 2454590737576305/35184372088832,470093902813667/8796093022208 5394165542284667/140737488355328,3740209471292431/70368744177664 5368039702932137/140737488355328,7433474547321699/140737488355328 5380948514166291/140737488355328,3681854470988973/70368744177664 1359507365998249/35184372088832,7251842294101963/140737488355328 5529556721346073/140737488355328,8669379559606881/281474976710656 7916317869408501/140737488355328))");
	std::auto_ptr<Geometry> gB = io::readWkt("POLYGON((1867875554198039/35184372088832 2713713388426203/70368744177664,461607983221751/8796093022208 687200737642883/17592186044416,22/1 64/1,36/1 150/1,1877727914546985/35184372088832 2761046010532207/17592186044416,3810878897050701/70368744177664 345823539665985/2199023255552,316819925148861/4398046511104 1238154090866731/8796093022208,1026082437762715/8796093022208 3483445459036695/35184372088832,1867875554198039/35184372088832 2713713388426203/70368744177664))");

	std::auto_ptr<Geometry> gI = algorithm::intersection( *gA, *gB );
}

BOOST_AUTO_TEST_SUITE_END()

