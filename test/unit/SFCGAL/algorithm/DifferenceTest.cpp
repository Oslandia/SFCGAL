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
#include <SFCGAL/algorithm/difference.h>
#include <SFCGAL/algorithm/volume.h>
#include <SFCGAL/algorithm/covers.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/detail/tools/Registry.h>

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/vtk.h>

#include <boost/test/unit_test.hpp>

using namespace SFCGAL;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_DifferenceTest )

BOOST_AUTO_TEST_CASE( testDifferenceXPoint )
{
    // The same point
    BOOST_CHECK( algorithm::difference( Point( 0,0 ), Point( 0,0 ) )->isEmpty() );
    // A different point
    BOOST_CHECK( *algorithm::difference( Point( 1,0 ), Point( 0,0 ) ) == Point( 1,0 ) );

    // check difference(X, point) == X
    std::vector<std::string> typeNames;

    for ( size_t i = 0; i < typeNames.size(); ++i ) {
        std::unique_ptr<Geometry> newGeo( tools::Registry::instance().newGeometryByTypeName( typeNames[i] ) );
        std::unique_ptr<Geometry> diffGeo( algorithm::difference( *newGeo, Point( 0, 0 ) ) );
        BOOST_CHECK( *newGeo == *diffGeo );
    }
}

BOOST_AUTO_TEST_CASE( testDifferenceXLineString )
{
    // Point x Linestring intersecting
    BOOST_CHECK( algorithm::difference( Point( 0,0 ), *io::readWkt( "LINESTRING(0 0,1 1)" ) )->isEmpty() );
    // Point x linestring not intersecting
    BOOST_CHECK( *algorithm::difference( Point( 0,0 ), *io::readWkt( "LINESTRING(0 1,1 1)" ) ) == Point( 0, 0 ) );

    // two linestrings with two segments overlapping
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "LINESTRING(0 0,1 0)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "LINESTRING(0.5 0,0.7 0)" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "MULTILINESTRING((0 0,0.5 0),(0.7 0,1 0))" ) );
    }
    // two linestrings with two opposite segments overlapping
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "LINESTRING(0 0,1 0)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "LINESTRING(0.7 0,0.5 0)" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "MULTILINESTRING((0 0,0.5 0),(0.7 0,1 0))" ) );
    }
    // two linestrings with two segments crossing
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "LINESTRING(-1 0,1 0)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "LINESTRING(0 -1,0 1)" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *ls1 );
    }
    // two linestrings with two segments partly overlapping
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "LINESTRING(0 0,1 0)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "LINESTRING(-1 0,0.7 0)" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "LINESTRING(0.7 0,1 0)" ) );
    }
    // two linestrings with a segment covering another one
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "LINESTRING(0 0,1 0)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "LINESTRING(-1 0,2 0)" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( diff->isEmpty() );
    }
    // two linestrings that do not intersect
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "LINESTRING(0 0,1 0)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "LINESTRING(0 1,1 1)" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *ls1 );
    }
    // two linestrings with more than one segment
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "LINESTRING(0 0,1 0,1 1)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "LINESTRING(0.3 0,1 0,1 0.4)" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "MULTILINESTRING((0 0,0.3 0),(1 0.4,1 1))" ) );
    }

    // check difference(X, linestring) == X, with dimension(X) > 1
    // TODO: add generators of random geometries to avoid empty geometries here ?
    std::vector<std::string> typeNames;

    for ( size_t i = 0; i < typeNames.size(); ++i ) {
        std::unique_ptr<Geometry> newGeo( tools::Registry::instance().newGeometryByTypeName( typeNames[i] ) );

        if ( newGeo->dimension() > 1 ) {
            std::unique_ptr<Geometry> diffGeo( algorithm::difference( *newGeo, Point( 0, 0 ) ) );
            BOOST_CHECK( *newGeo == *diffGeo );
        }
    }
}

BOOST_AUTO_TEST_CASE( testDifferencePolygonPolygon2D )
{
    // two identical polygons
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1))" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "GEOMETRYCOLLECTION EMPTY" ) );
    }

    // two polygons, one of wich is invalid for CGAL but valid for SFS
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1))" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1),(-0.5 -0.5,-0.5 0.5,0.5 0.5,1 -0.5,-0.5 -0.5))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "POLYGON((-0.5 -0.5,1 -0.5,0.5 0.5,-0.5 0.5,-0.5 -0.5))" ) );
        BOOST_CHECK( algorithm::isValid( *diff ) );
    }

    // two polygons the result has a hole touching the outer boundary
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1))" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "POLYGON((-0.5 -0.5,1 -0.5,0.5 0.5,-0.5 0.5,-0.5 -0.5))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( algorithm::isValid( *diff ) );
        BOOST_CHECK( *diff == *io::readWkt( "POLYGON((-1 -1,1 -1,1 -0.5,1 1,-1 1,-1 -1),(1 -0.5,-0.5 -0.5,-0.5 0.5,0.5 0.5,1 -0.5))" ) );
    }
}

BOOST_AUTO_TEST_CASE( testDifferenceVolumeVolume )
{

    // two cubes
    {
        std::unique_ptr<Geometry> ls1( io::readWkt(
                                          "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                 ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                 ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                 ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                 ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                 ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt(
                                          "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                 ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                 ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                 ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                 ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                 ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "GEOMETRYCOLLECTION EMPTY" ) );
    }
    // two cubes
    {
        std::unique_ptr<Geometry> ls1 = io::readWkt(
                                          "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                 ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                 ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                 ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                 ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                 ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" );
        std::unique_ptr<Geometry> ls2( io::readWkt(
                                          "SOLID((((0 0 0.5, 0 1 0.5, 1 1 0.5, 1 0 0.5, 0 0 0.5)),\
                 ((0 0 0.5, 0 0 1, 0 1 1, 0 1 0.5, 0 0 0.5)),\
                 ((0 0 0.5, 1 0 0.5, 1 0 1, 0 0 1, 0 0 0.5)),\
                 ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                 ((1 1 1, 1 0 1, 1 0 0.5, 1 1 0.5, 1 1 1)),\
                 ((1 1 1, 1 1 0.5, 0 1 0.5, 0 1 1, 1 1 1))))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( algorithm::volume( *diff ) == Kernel::FT( 0.5 ) );
    }


}

BOOST_AUTO_TEST_CASE( testDifferenceLinePolygon )
{

    // segment - polygon in 2D
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "LINESTRING(-10 0,10 0)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1),(-0.5 -0.5,-0.5 0.5,0 0,-0.5 -0.5),(0.5 0.5,0.5 -0.5,0 0,0.5 0.5))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "MULTILINESTRING((-10 0,-1 0),(-0.5 0,0 0,0.5 0),(1 0,10 0))" ) );
    }

    // segment - polygon in 2D, with sement lying on hole border
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "LINESTRING(-10 0,10 0)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "POLYGON((-1 -1,1 -1,1 1,-1 1,-1 -1),(-0.5 -0.5,-0.5 0.5,0 0,-0.5 -0.5),(0.5 0,0.5 -0.5,0 0,0.5 0))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "MULTILINESTRING((-10 0,-1 0),(-0.5 0,0 0),(1 0,10 0))" ) );
    }

}

BOOST_AUTO_TEST_CASE( testDifferencePoinLine )
{
    // point - segment in 3D
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "POINT(0.5 0.5 0.6)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "LINESTRING(0 0 0,1 1 1)" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "POINT(0.5 0.5 0.6)" ) );
    }
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "POINT(0.5 0.5 0.5)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "LINESTRING(0 0 0,1 1 1)" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "GEOMETRYCOLLECTION EMPTY" ) );
    }

}

BOOST_AUTO_TEST_CASE( testDifferencePoinPolygon2D )
{
    // point - triangle in 3D
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "POINT(0.5 0.5 0.6)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "POLYGON((0 0 0,1 1 1,1 0 1,0 0 0))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "POINT(0.5 0.5 0.6)" ) );
    }
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "POINT(0.5 0.5 0.5)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "POLYGON((0 0 0,1 1 1,1 0 1,0 0 0))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "GEOMETRYCOLLECTION EMPTY" ) );
    }
}

BOOST_AUTO_TEST_CASE( testDifferencePoinVolume )
{

    // point - volume
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "POINT(0.5 0.5 0.5)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt(
                                          "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                 ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                 ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                 ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                 ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                 ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "GEOMETRYCOLLECTION EMPTY" ) );
    }
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "POINT(1.001 0.5 0.5)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt(
                                          "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                 ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                 ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                 ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                 ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                 ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "POINT(1.001 0.5 0.5)" ) );
    }

}

BOOST_AUTO_TEST_CASE( testDifferenceTriangleTriangle3D )
{
    // triangle - trangle in 3D don't share the same plane
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "TRIANGLE((0 0 0,0 1 1,1 0 0,0 0 0))" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "TRIANGLE((0 0 0,0 1 1.01,1 0 0,0 0 0))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "TRIANGLE((0 0 0,0 1 1,1 0 0,0 0 0))" ) );
    }
    // triangle - trangle in 3D don't intersect
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "TRIANGLE((0 0 0,0 1 1,1 0 0,0 0 0))" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "TRIANGLE((.6 .6 .6,1.6 1.6 1.6,1.6 .6 .6,.6 .6 .6))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "TRIANGLE((0 0 0,0 1 1,1 0 0,0 0 0))" ) );
    }
    // triangle - triangle in 3D do intersect
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "TRIANGLE((0 0 0,0 1 1,1 0 0,0 0 0))" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt( "TRIANGLE((.1 .1 .1,1.6 1.6 1.6,1.6 .6 .6,.1 .1 .1))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "TIN(((0 1 1,.5 .5 .5,.1 .1 .1,0 1 1)),((0 0 0,0 1 1,.1 .1 .1,0 0 0)),((.7 .3 .3,1 0 0,.1 .1 .1,.7 .3 .3)),((1 0 0,0 0 0,.1 .1 .1,1 0 0)))" ) );
    }

}

BOOST_AUTO_TEST_CASE( testDifferenceTriangleVolume )
{
    // triangle - volume in 3D
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "TRIANGLE((0 0 .5,10 0 .5,0 10 .5,0 0 .5))" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt(
                                          "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                 ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                 ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                 ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                 ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                 ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        std::unique_ptr<Geometry> ref( io::readWkt( "TIN(((0/1 10/1 1/2,9/20 1/1 1/2,1/2 1/1 1/2,0/1 10/1 1/2)),((0/1 10/1 1/2,1/2 1/1 1/2,18/19 1/1 1/2,0/1 10/1 1/2)),((0/1 10/1 1/2,0/1 1/1 1/2,9/20 1/1 1/2,0/1 10/1 1/2)),((1/1 0/1 1/2,10/1 0/1 1/2,1/1 1/2 1/2,1/1 0/1 1/2)),((1/1 1/1 1/2,0/1 10/1 1/2,18/19 1/1 1/2,1/1 1/1 1/2)),((10/1 0/1 1/2,0/1 10/1 1/2,1/1 1/1 1/2,10/1 0/1 1/2)),((1/1 1/2 1/2,10/1 0/1 1/2,1/1 1/1 1/2,1/1 1/2 1/2)))" ) );
        BOOST_CHECK( algorithm::covers( *diff, *ref ) && algorithm::covers( *ref, *diff ) );
    }
}

BOOST_AUTO_TEST_CASE( testDifferenceLineVolume )
{
    // segment - volume in 3D
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "LINESTRING(-3 -3 .5,3 3 .5,1 1.1 .5,1 .1 .5,.1 .1 .5)" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt(
                                          "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                 ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                 ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                 ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                 ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                 ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))" ) );
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
        BOOST_CHECK( *diff == *io::readWkt( "MULTILINESTRING((-3 -3 .5,0 0 .5),(1 1 .5,3 3 .5,1 1.1 .5,1 1 .5))" ) );
    }

}

BOOST_AUTO_TEST_CASE( testDifferencePolygonVolume )
{

    // polygon - volume crashing the algo in garden
    {
        std::unique_ptr<Geometry> ls1( io::readWkt( "POLYGON((1 -1 -1,1 1 -1,1 1 1,1 -1 1,1 -1 -1))" ) );
        std::unique_ptr<Geometry> ls2( io::readWkt(
                                          "SOLID((((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)),\
                        ((0 0 0,0 0 1,0 1 1,0 1 0,0 0 0)),\
                        ((0 0 0,1 0 0,1 0 1,0 0 1,0 0 0)),\
                        ((1 1 1,0 1 1,0 0 1,1 0 1,1 1 1)),\
                        ((1 1 1,1 0 1,1 0 0,1 1 0,1 1 1)),\
                        ((1 1 1,1 1 0,0 1 0,0 1 1,1 1 1))))" ));
        std::unique_ptr<Geometry> diff( algorithm::difference3D( *ls1, *ls2 ) );
    }

}

BOOST_AUTO_TEST_SUITE_END()

