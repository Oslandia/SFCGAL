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

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/vtk.h>

#include <boost/test/unit_test.hpp>

#define DEBUG_OUT if (1) std::cerr << __FILE__ << ":" << __LINE__ << " debug: "

using namespace SFCGAL;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_UnionTest )

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
        std::auto_ptr<Geometry> a = io::readWkt( "POINT(0 0 1)" );
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(0 1 1)" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "MULTIPOINT(0 0 1,0 1 1)" ) );
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

BOOST_AUTO_TEST_CASE( TrianglePoint )
{
    {
        std::auto_ptr<Geometry> a = io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POINT(.1 .1)" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" ) );
    }
}


BOOST_AUTO_TEST_CASE( TriangleTriangle )
{
    try {
        std::auto_ptr<Geometry> a = io::readWkt( "TRIANGLE((0 0,1 0,0 1,0 0))" );
        std::auto_ptr<Geometry> b = io::readWkt( "TRIANGLE((0 0,1 0,0 1,0 0))" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( *u == *io::readWkt( "TRIANGLE((0 0,0 1,1 0,0 0))" ) );
    }
    catch ( NotImplementedException e ) {
        std::cerr << e.what() << "\n";
    }

}

BOOST_AUTO_TEST_CASE( PolygonPolygon )
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

BOOST_AUTO_TEST_CASE( GardenFailures )
{
    // crash (valgrind err) with invalidated iterator after push_back
    {
        std::auto_ptr<Geometry> a = io::readWkt( "LINESTRING(-1 -1,1 1)" );
        std::auto_ptr<Geometry> b = io::readWkt( "MULTILINESTRING((1/1 -1/1 -1/1,1/1 1/1 1/1),(1/1 1/1 1/1,1/1 1/1 -1/1))" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
    }

    // cgal precondition violation
    {
        std::auto_ptr<Geometry> a = io::readWkt( "POLYGON((0 0,10 0,10 0,10 10,0 10,0 0),(2 2,2 5,5 5,5 2,2 2))" );
        std::auto_ptr<Geometry> b = io::readWkt( "TRIANGLE((-1 -1,1 -1,-1 1,-1 -1))" );
        io::vtk( *a, "a.vtk" );
        io::vtk( *b, "b.vtk" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        BOOST_CHECK( algorithm::area(*a) + algorithm::area(*b) == algorithm::area(*u) );
    }

    // infinite loop
    try {
        std::auto_ptr<Geometry> a = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POLYGON((0 0,10 0,10 0,10 10,0 10,0 0))" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
    }
    catch ( NotImplementedException e ) {
        std::cerr << e.what() << "\n";
    }

    // infinite loop
    {
        std::auto_ptr<Geometry> a = io::readWkt( "MULTIPOLYGON(((-3 -1,1 -1,-1 1,-3 1,-3 -1)),((1 -1,3 -1,3 1,1 1,1 -1)))" );
        std::auto_ptr<Geometry> b = io::readWkt( "MULTIPOLYGON(((-3 -1,-1 -1,-1 1,-3 1,-3 -1)),((1 -1,3 -1,3 1,1 1,1 -1)))" );
        std::auto_ptr<Geometry> u = algorithm::union_( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
    }

    // segfault
    try {
        std::auto_ptr<Geometry> a = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1))" );
        std::auto_ptr<Geometry> b = io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1),(-0.5 -0.5,-0.5 0.5,0.5 0.5,0.5 -0.5,-0.5 -0.5))" );
        io::vtk( *a, "a.vtk" );
        io::vtk( *b, "b.vtk" );
        std::auto_ptr<Geometry> u = algorithm::union3D( *a, *b );
        DEBUG_OUT << u->asText() <<"\n";
        io::vtk( *u, "u.vtk" );
    }
    catch ( NotImplementedException e ) {
        std::cerr << e.what() << "\n";
    }

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

