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
#include <boost/test/unit_test.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/straightSkeleton.h>

#include <SFCGAL/detail/tools/Registry.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_StraightSkeletonTest )


BOOST_AUTO_TEST_CASE( testTriangle )
{
    std::auto_ptr< Geometry > g( io::readWkt( "TRIANGLE((0 0,1 0,1 1,0 0))" ) );

    std::string expectedWKT( "MULTILINESTRING((0.0 0.0,0.7 0.3),(1.0 0.0,0.7 0.3),(1.0 1.0,0.7 0.3))" );
    {
        std::auto_ptr< MultiLineString > result( algorithm::straightSkeleton( *g ) ) ;
        BOOST_CHECK_EQUAL( result->numGeometries(), 3U );
        BOOST_CHECK_EQUAL( result->asText( 1 ), expectedWKT );
    }
    // check orientation insensitive
    {
        g->as< Triangle >().reverse() ;
        std::auto_ptr< MultiLineString > result( algorithm::straightSkeleton( *g ) ) ;
        BOOST_CHECK_EQUAL( result->numGeometries(), 3U );
        BOOST_CHECK_EQUAL( result->asText( 1 ), expectedWKT );
    }
}


BOOST_AUTO_TEST_CASE( testPolygon )
{
    std::auto_ptr< Geometry > g( io::readWkt( "POLYGON((1 1,11 1,11 11,1 11,1 1))" ) );

    std::string expectedWKT( "MULTILINESTRING((1 1,6 6),(11 1,6 6),(11 11,6 6),(1 11,6 6))" );
    {
        std::auto_ptr< MultiLineString > result( algorithm::straightSkeleton( *g ) ) ;
        BOOST_CHECK_EQUAL( result->numGeometries(), 4U );
        BOOST_CHECK_EQUAL( result->asText( 0 ), expectedWKT );
    }
    // check orientation insensitive
    {
        g->as< Polygon >().exteriorRing().reverse() ;
        std::auto_ptr< MultiLineString > result( algorithm::straightSkeleton( *g ) ) ;
        BOOST_CHECK_EQUAL( result->numGeometries(), 4U );
        BOOST_CHECK_EQUAL( result->asText( 0 ), expectedWKT );
    }
}

BOOST_AUTO_TEST_CASE( testPolygonWithHole )
{
    std::auto_ptr< Geometry > g( io::readWkt( "POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,0.5 0.5,-0.5 -0.5))" ) );
    std::string expectedWKT( "MULTILINESTRING((-1.00 -1.00,-0.75 -0.75),(1.00 -1.00,0.41 -0.41),(1.00 1.00,0.75 0.75),(-1.00 1.00,-0.75 0.75),(-0.50 -0.50,-0.65 -0.85),(-0.50 0.50,-0.75 0.75),(0.50 0.50,0.85 0.65),(-0.65 -0.85,0.41 -0.41),(-0.65 -0.85,-0.75 -0.75),(0.85 0.65,0.75 0.75),(0.85 0.65,0.41 -0.41),(-0.75 -0.75,-0.75 0.75),(0.75 0.75,-0.75 0.75))" );
    {
        std::auto_ptr< MultiLineString > result( algorithm::straightSkeleton( *g ) ) ;
        //BOOST_CHECK_EQUAL( result->numGeometries(), 4U );
        BOOST_CHECK_EQUAL( result->asText( 2 ), expectedWKT );
    }
}

BOOST_AUTO_TEST_CASE( testPolygonWithHoleTouchingShell )
{
    std::auto_ptr< Geometry > g( io::readWkt( "POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,0.5 0.5,1.0 -0.5,-0.5 -0.5))" ) );
    BOOST_CHECK_THROW( algorithm::straightSkeleton( *g ), NotImplementedException );
}

BOOST_AUTO_TEST_CASE( testPolygonWithTouchingHoles )
{
    std::auto_ptr< Geometry > g( io::readWkt( "POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,-0.1 0.5,0.1 -0.5,-0.5 -0.5),(0.1 -0.5,0.1 0.5,0.5 0.5,0.5 -0.5,0.1 -0.5))" ) );

    BOOST_CHECK_THROW( algorithm::straightSkeleton( *g ), NotImplementedException );

}

BOOST_AUTO_TEST_CASE( testMultiPolygon )
{
    std::auto_ptr< Geometry > g( io::readWkt( "MULTIPOLYGON(((3.000000 0.000000,2.875000 0.484123,2.750000 0.661438,2.625000 0.780625,2.500000 0.866025,2.375000 0.927025,2.250000 0.968246,2.125000 0.992157,2.000000 1.000000,1.875000 1.484123,1.750000 1.661438,1.625000 1.780625,1.500000 1.866025,1.375000 1.927025,1.250000 1.968246,1.125000 1.992157,1.000000 2.000000,0.750000 2.661438,0.500000 2.866025,0.250000 2.968246,0.000000 3.000000,-0.250000 2.968246,-0.500000 2.866025,-0.750000 2.661438,-1.000000 2.000000,-1.125000 1.992157,-1.250000 1.968246,-1.375000 1.927025,-1.500000 1.866025,-1.625000 1.780625,-1.750000 1.661438,-1.875000 1.484123,-2.000000 1.000000,-2.125000 0.992157,-2.250000 0.968246,-2.375000 0.927025,-2.500000 0.866025,-2.625000 0.780625,-2.750000 0.661438,-2.875000 0.484123,-3.000000 0.000000,-2.875000 -0.484123,-2.750000 -0.661438,-2.625000 -0.780625,-2.500000 -0.866025,-2.375000 -0.927025,-2.250000 -0.968246,-2.125000 -0.992157,-2.000000 -1.000000,-1.875000 -1.484123,-1.750000 -1.661438,-1.625000 -1.780625,-1.500000 -1.866025,-1.375000 -1.927025,-1.250000 -1.968246,-1.125000 -1.992157,-1.000000 -2.000000,-0.750000 -2.661438,-0.500000 -2.866025,-0.250000 -2.968246,0.000000 -3.000000,0.250000 -2.968246,0.500000 -2.866025,0.750000 -2.661438,1.000000 -2.000000,1.125000 -1.992157,1.250000 -1.968246,1.375000 -1.927025,1.500000 -1.866025,1.625000 -1.780625,1.750000 -1.661438,1.875000 -1.484123,2.000000 -1.000000,2.125000 -0.992157,2.250000 -0.968246,2.375000 -0.927025,2.500000 -0.866025,2.625000 -0.780625,2.750000 -0.661438,2.875000 -0.484123,3.000000 0.000000),(0.000000 1.000000,0.125000 0.515877,0.250000 0.338562,0.375000 0.219375,0.500000 0.133975,0.625000 0.072975,0.750000 0.031754,0.875000 0.007843,1.000000 0.000000,0.875000 -0.007843,0.750000 -0.031754,0.625000 -0.072975,0.500000 -0.133975,0.375000 -0.219375,0.250000 -0.338562,0.125000 -0.515877,0.000000 -1.000000,-0.125000 -0.515877,-0.250000 -0.338562,-0.375000 -0.219375,-0.500000 -0.133975,-0.625000 -0.072975,-0.750000 -0.031754,-0.875000 -0.007843,-1.000000 0.000000,-0.875000 0.007843,-0.750000 0.031754,-0.625000 0.072975,-0.500000 0.133975,-0.375000 0.219375,-0.250000 0.338562,-0.125000 0.515877,0.000000 1.000000)))" ) );
    std::auto_ptr< MultiLineString > result( algorithm::straightSkeleton( *g ) ) ;
    BOOST_CHECK_EQUAL( result->numGeometries(), 220U );
}


BOOST_AUTO_TEST_CASE( testInvalidTypes )
{
    std::vector< std::string > wkt;
    wkt.push_back( "POINT(1 2)" );
    wkt.push_back( "LINESTRING(0 0,1 1)" );

    for ( std::vector< std::string >::const_iterator it=wkt.begin(),
                                                     itE=wkt.end();
          it != itE; ++it )
    {
      std::auto_ptr< Geometry > g( io::readWkt( *it ) );
      std::auto_ptr< MultiLineString > result(
          algorithm::straightSkeleton( *g )
      );
      BOOST_CHECK_EQUAL( result->numGeometries(), 0U );
    }
}

// See https://github.com/Oslandia/SFCGAL/issues/75
BOOST_AUTO_TEST_CASE( testPostgisIssue3107 )
{
    std::auto_ptr< Geometry > g( io::readWkt( "POLYGON((1347259.25 7184745.94,1347273.17 7184758.16,1347280.39 7184749.95,1347278.04 7184747.88,1347281.66 7184743.76,1347284.01 7184745.83,1347293.5 7184735.05,1347279.61 7184722.85,1347269.29 7184734.6,1347270.35 7184735.51,1347267.31 7184738.96,1347266.22 7184738.01,1347259.25 7184745.94),(1347267.31 7184738.96,1347269.31 7184736.7,1347272.57 7184739.55,1347270.56 7184741.83,1347267.31 7184738.96))" ) );
    BOOST_CHECK_THROW( algorithm::straightSkeleton( *g ), NotImplementedException );
}

// See https://github.com/Oslandia/SFCGAL/issues/91
BOOST_AUTO_TEST_CASE( testMultiPolygonWithTouchingHoles )
{
    std::auto_ptr< Geometry > g( io::readWkt( "MULTIPOLYGON(((1347259.25 7184745.94,1347273.17 7184758.16,1347280.39 7184749.95,1347278.04 7184747.88,1347281.66 7184743.76,1347284.01 7184745.83,1347293.5 7184735.05,1347279.61 7184722.85,1347269.29 7184734.6,1347270.35 7184735.51,1347267.31 7184738.96,1347266.22 7184738.01,1347259.25 7184745.94),(1347267.31 7184738.96,1347269.31 7184736.7,1347272.57 7184739.55,1347270.56 7184741.83,1347267.31 7184738.96)))" ) );
    BOOST_CHECK_THROW( algorithm::straightSkeleton( *g ), NotImplementedException );
}

BOOST_AUTO_TEST_SUITE_END()

