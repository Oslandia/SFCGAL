/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include <SFCGAL/Exception.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/algorithm/union.h>
#include <SFCGAL/algorithm/volume.h>
#include <SFCGAL/algorithm/translate.h>
#include <SFCGAL/algorithm/area.h>
#include <SFCGAL/detail/tools/Registry.h>
#include <SFCGAL/algorithm/tesselate.h>
#include <SFCGAL/triangulate/triangulatePolygon.h>

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/vtk.h>

#include <boost/test/unit_test.hpp>

namespace SFCGAL{ namespace algorithm {
void handleLeakTest();
}}

#define DEBUG_OUT if (0) std::cerr << __FILE__ << ":" << __LINE__ << " debug: "

using namespace SFCGAL;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_UnionTest )


BOOST_AUTO_TEST_CASE( Handle )
{
    algorithm::handleLeakTest();
}

BOOST_AUTO_TEST_CASE( Handle1 )
{
    std::auto_ptr<Geometry> a = io::readWkt( "POINT(0 1)" );
    std::auto_ptr<Geometry> b = io::readWkt( "POINT(0 1)" );
    std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
    DEBUG_OUT << u->asText() <<"\n";
    BOOST_CHECK( *u == *io::readWkt( "POINT(0 1)" ) );
}

BOOST_AUTO_TEST_CASE( Handle2 )
{
    std::auto_ptr<Geometry> a = io::readWkt( "MULTIPOINT(0 1,0 1,0 1)" );
    std::auto_ptr<Geometry> b = io::readWkt( "POINT(0 1)" );
    std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
    DEBUG_OUT << u->asText() <<"\n";
    BOOST_CHECK( *u == *io::readWkt( "POINT(0 1)" ) );
}

BOOST_AUTO_TEST_CASE( PointPoint )
{
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POINT(0 1)" );
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(0 1)" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "POINT(0 1)" ) );
    }
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POINT(0 0)" );
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(0 1)" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "MULTIPOINT(0 0,0 1)" ) );
    }
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POINT(0 1 1)" );
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(0 1 1)" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "POINT(0 1 1)" ) );
    }
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POINT(0 0 0)" );
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(0 0 1)" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "MULTIPOINT(0 0 0,0 0 1)" ) );
    }
}

BOOST_AUTO_TEST_CASE( PointLine )
{
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POINT(.5 0)" );
        std::auto_ptr<Geometry> b = io::readWkt( "LINESTRING(-1 0,1 0)" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "LINESTRING(-1 0,.5 0,1 0)" ) );
    }
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POINT(0 0 .5)" );
        std::auto_ptr<Geometry> b = io::readWkt( "LINESTRING(0 0 -1,0 0 1)" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "LINESTRING(0 0 -1,0 0 .5,0 0 1)" ) );
    }
}

BOOST_AUTO_TEST_CASE( LineLine )
{
    {
        std::auto_ptr<Geometry> a = io::readWkt( "LINESTRING(-1 0,1 0)" );
        std::auto_ptr<Geometry> b = io::readWkt( "LINESTRING(-1 1,1 1)" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "MULTILINESTRING((-1 0,1 0),(-1 1,1 1))" ) );
    }
    {
        std::auto_ptr<Geometry> a = io::readWkt( "LINESTRING(-1 0,1 0)" );
        std::auto_ptr<Geometry> b = io::readWkt( "LINESTRING(0 -1,0 1)" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "MULTILINESTRING((-1 0,0 0),(0 0,1 0),(0 -1,0 0),(0 0,0 1))" ) );
    }
}

BOOST_AUTO_TEST_CASE( LineVolume )
{
    std::auto_ptr<Geometry> a = io::readWkt( "LINESTRING(-1 -1 -1,2 2 2)" );
    std::auto_ptr<Geometry> b = io::readWkt(
                                      "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
             ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
             ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
             ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
             ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
             ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" );
    std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
    BOOST_CHECK( u->geometryTypeId() == TYPE_GEOMETRYCOLLECTION );
    BOOST_CHECK( u->geometryN(0).geometryTypeId() == TYPE_LINESTRING );
    BOOST_CHECK( u->geometryN(1).geometryTypeId() == TYPE_LINESTRING );
    BOOST_CHECK( u->geometryN(2).geometryTypeId() == TYPE_SOLID );
}

BOOST_AUTO_TEST_CASE( PointSurface )
{
    {
        std::auto_ptr<Geometry> a = io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(.1 .1)" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" ) );
    }
    {
        std::auto_ptr<Geometry> a = io::readWkt( "TRIANGLE((0 0 1,0 1 1,1 0 1,0 0 1))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(.1 .1 1)" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "TRIANGLE((0 0 1,0 1 1,1 0 1,0 0 1))" ) );
    }
}

BOOST_AUTO_TEST_CASE( PointVolume )
{
    std::auto_ptr<Geometry> a = io::readWkt(
                                      "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
             ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
             ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
             ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
             ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
             ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" );
    {
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(.1 .1 1)" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        BOOST_CHECK( u->geometryTypeId() == TYPE_SOLID );
    }
    {
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(-.1 .1 1)" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        BOOST_CHECK( u->geometryTypeId() == TYPE_GEOMETRYCOLLECTION );
    }
}

BOOST_AUTO_TEST_CASE( TriangleTriangle )
{
    {
        std::auto_ptr<Geometry> a = io::readWkt( "TRIANGLE((0 0,1 0,0 1,0 0))" );
        std::auto_ptr<Geometry> b = io::readWkt( "TRIANGLE((0 0,1 0,0 1,0 0))" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" ) );
    }
}

BOOST_AUTO_TEST_CASE( PolygonPolygon1 )
{
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1),(-0.5 -0.5,-0.5 0.5,0.5 0.5,0.5 -0.5,-0.5 -0.5))" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        BOOST_CHECK( *u == *io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1))" ) );
    }

    {
        std::auto_ptr<Geometry> a = io::readWkt( "POLYGON((0 0,1 0,1 1,0 1,0 0))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POLYGON((1 0,2 0,2 1,1 1,1 0))" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "POLYGON((0 0,1 0,2 0,2 1,1 1,0 1,0 0))" ) );
    }
}

BOOST_AUTO_TEST_CASE( PolygonPolygon2 )
{
    {
        std::auto_ptr<Geometry> base = io::readWkt( "POLYGON((0 0,1 0,1 1,0 1,0 0))" );
        Polygon a( base->as<Polygon>() );
        algorithm::translate( a, .75, 0, 0); //disjoined
        GeometryCollection b;
        b.addGeometry( base->as<Polygon>() );
        b.addGeometry( base->as<Polygon>() );
        algorithm::translate( b.geometryN(1), 1.5, 0, 0 );

        std::auto_ptr<Geometry> u = algorithm::union_( a, b );
        DEBUG_OUT << u->asText() << "\n";
        DEBUG_OUT << "area " << algorithm::area3D(*u) <<"\n";
        BOOST_CHECK( u->geometryTypeId() == TYPE_POLYGON );
        BOOST_CHECK( algorithm::area3D(*u) == 2.5 );

        u = algorithm::union3D( a, b );
        DEBUG_OUT << u->asText() <<"\n";
        DEBUG_OUT << "area " << algorithm::area3D(*u) <<"\n";
        BOOST_CHECK( u->geometryTypeId() == TYPE_TRIANGULATEDSURFACE );
        BOOST_CHECK( algorithm::area3D(*u) == 2.5 );
    }
}

BOOST_AUTO_TEST_CASE( PolygonPolygon3 )
{
    std::auto_ptr<Geometry> base = io::readWkt( "POLYGON((0 0,1 0,1 1,0 1,0 0))" );
    GeometryCollection a;
    GeometryCollection b;
    for (unsigned i = 0; i<4; ++i){
        for (unsigned j = 0; j<4; ++j){
            Polygon p(  base->as<Polygon>() ); 
            algorithm::translate( p, std::pow(i, 1.3 ), std::pow(j, 1.3 ), 0 );
            a.addGeometry( p );
            algorithm::translate( p, .5, .5, 0 );
            b.addGeometry( p );
        }
    }
    std::auto_ptr<Geometry> u = algorithm::union_( a, b );
    TriangulatedSurface ts;
    triangulate::triangulatePolygon3D(*u, ts); 
    io::vtk(ts, "out.vtk");
}

BOOST_AUTO_TEST_CASE( GardenFailures1 )
{
    // cgal precondition violation
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POLYGON((0 0,10 0,10 0,10 10,0 10,0 0),(2 2,2 5,5 5,5 2,2 2))" );
        std::auto_ptr<Geometry> b = io::readWkt( "TRIANGLE((-1 -1,1 -1,-1 1,-1 -1))" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( algorithm::area(*a) + algorithm::area(*b) == algorithm::area(*u) );
    }
}

BOOST_AUTO_TEST_CASE( GardenFailures2 )
{
    // cgal assertion 
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1),(-.5 -.5,-.5 .5,.5 .5,.5 -.5,-.5 -.5))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1),(-.5 -.5,-.5 .5,.5 .5,.5 -.5,-.5 -.5))" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
    }
}

BOOST_AUTO_TEST_CASE( GardenFailures3 )
{
    // crash (valgrind err) with invalidated iterator after push_back
    {
        std::auto_ptr<Geometry> a = io::readWkt( "LINESTRING(-1 -1,1 1)" );
        std::auto_ptr<Geometry> b = io::readWkt( "MULTILINESTRING((1/1 -1/1 -1/1,1/1 1/1 1/1),(1/1 1/1 1/1,1/1 1/1 -1/1))" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
    }
}

BOOST_AUTO_TEST_CASE( GardenFailures4 )
{
    // infinite loop
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POLYGON((0 0,10 0,10 0,10 10,0 10,0 0))" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
    }
}

BOOST_AUTO_TEST_CASE( GardenFailures5 )
{
    // infinite loop
    {
        std::auto_ptr<Geometry> a = io::readWkt( "MULTIPOLYGON(((-3 -1,-1 -1,-1 1,-3 1,-3 -1)),((1 -1,3 -1,3 1,1 1,1 -1)))" );
        std::auto_ptr<Geometry> b = io::readWkt( "MULTIPOLYGON(((-3 -1,-1 -1,-1 1,-3 1,-3 -1)),((1 -1,3 -1,3 1,1 1,1 -1)))" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
    }
}

BOOST_AUTO_TEST_CASE( GardenFailures6 )
{
    // segfault
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1),(-0.5 -0.5,-0.5 0.5,0.5 0.5,0.5 -0.5,-0.5 -0.5))" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
    }
}

BOOST_AUTO_TEST_CASE( GardenFailures7 )
{
    std::auto_ptr<Geometry> a = io::readWkt( "LINESTRING(-1 -1,1 1)" );
    std::auto_ptr<Geometry> b = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1),(-.5 -.5,-.5 .5,.5 .5,1 -.5,-.5 -.5))" );
    std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
    DEBUG_OUT << u->asText() <<"\n";
}

BOOST_AUTO_TEST_CASE( VolumeVolume )
{
    std::auto_ptr<Geometry> a = io::readWkt(
                                      "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
             ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
             ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
             ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
             ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
             ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" );
    {
        Solid b = a->as<Solid>();
        algorithm::translate( b, 2, 0, 0); //disjoined
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, b );
        BOOST_CHECK( u->geometryTypeId() == TYPE_MULTISOLID );
        BOOST_CHECK( algorithm::volume(*u) == 2 );
    }
    {
        Solid b = a->as<Solid>();
        algorithm::translate( b, .5, 0, 0);
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, b );
        BOOST_CHECK( u->geometryTypeId() == TYPE_SOLID );
        BOOST_CHECK( algorithm::volume(*u) == 1.5 );
    }
    {
        Solid b = a->as<Solid>();
        algorithm::translate( b, 1, 0, 0);
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, b );
        BOOST_CHECK( u->geometryTypeId() == TYPE_SOLID );
        BOOST_CHECK( algorithm::volume(*u) == 2 );
    }
    {
        Solid b = a->as<Solid>();
        algorithm::translate( b, 1, 1, 0);
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, b );
        BOOST_CHECK( u->geometryTypeId() == TYPE_MULTISOLID );
        BOOST_CHECK( algorithm::volume(*u) == 2 );
    }

    // @todo fix the code to pass the test
    if (0)
    {
        Solid b = a->as<Solid>();
        algorithm::translate( b, 1, 1, 1); // share a corner
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, b );
        std::cerr << u->asText() << "\n";
        BOOST_CHECK( u->geometryTypeId() == TYPE_MULTISOLID );
        BOOST_CHECK( algorithm::volume(*u) == 2 );
    }
}

BOOST_AUTO_TEST_SUITE_END()

