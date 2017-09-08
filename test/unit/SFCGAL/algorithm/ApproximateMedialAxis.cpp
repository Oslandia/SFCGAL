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

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_ApproximateMedialAxisTest )


BOOST_AUTO_TEST_CASE( testTriangle45 )
{
    std::unique_ptr< Geometry > g( io::readWkt( "TRIANGLE((1 1,2 1,2 2,1 1))" ) );

    std::string expectedWKT( "MULTILINESTRING((1.0 1.0,1.7 1.3),(2.0 2.0,1.7 1.3))");
    {
        std::unique_ptr< MultiLineString > result( algorithm::approximateMedialAxis( *g ) ) ;
        BOOST_CHECK_EQUAL( result->asText( 1 ), expectedWKT );
    }
}

BOOST_AUTO_TEST_CASE( testTriangle60 )
{
    std::unique_ptr< Geometry > g( io::readWkt( "TRIANGLE((1 1,3 1,2 3,1 1))" ) );

    std::string expectedWKT( "MULTILINESTRING EMPTY" );
    {
        std::unique_ptr< MultiLineString > result( algorithm::approximateMedialAxis( *g ) ) ;
        BOOST_CHECK_EQUAL( result->numGeometries(), 0U );
        BOOST_CHECK_EQUAL( result->asText( 1 ), expectedWKT );
    }
}

BOOST_AUTO_TEST_CASE( testPolygon )
{
    std::unique_ptr< Geometry > g( io::readWkt( "POLYGON((0 0,20 0,20 10,0 10,0 0))" ) );

    std::string expectedWKT( "MULTILINESTRING((5 5,15 5))" );
    {
        std::unique_ptr< MultiLineString > result( algorithm::approximateMedialAxis( *g ) ) ;
        BOOST_CHECK_EQUAL( result->numGeometries(), 1U );
        BOOST_CHECK_EQUAL( result->asText( 0 ), expectedWKT );
    }
}
BOOST_AUTO_TEST_CASE( testPolygonWithHole )
{
    std::unique_ptr< Geometry > g( io::readWkt( "POLYGON((0 0,10 0,10 10,0 10,0 0),(4 4,4 6,6 6,6 4,4 4))" ) );

    std::string expectedWKT( "MULTILINESTRING((2 2,8 2),(2 2,2 8),(8 2,8 8),(2 8,8 8))" );
    {
        std::unique_ptr< MultiLineString > result( algorithm::approximateMedialAxis( *g ) ) ;
        BOOST_CHECK_EQUAL( result->numGeometries(), 4U );
        BOOST_CHECK_EQUAL( result->asText( 0 ), expectedWKT );
    }
}

BOOST_AUTO_TEST_CASE( testPolygonWithTouchingHoles )
{
    std::unique_ptr< Geometry > g( io::readWkt( "POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,-0.1 0.5,0.1 -0.5,-0.5 -0.5),(0.1 -0.5,0.1 0.5,0.5 0.5,0.5 -0.5,0.1 -0.5))" ) );

    // just for valgrind
    BOOST_CHECK_THROW( std::unique_ptr< MultiLineString > result( algorithm::approximateMedialAxis( *g ) ), NotImplementedException ) ;

}

BOOST_AUTO_TEST_CASE( testMultiPolygon )
{
    std::unique_ptr< Geometry > g( io::readWkt( "MULTIPOLYGON(((3.000000 0.000000,2.875000 0.484123,2.750000 0.661438,2.625000 0.780625,2.500000 0.866025,2.375000 0.927025,2.250000 0.968246,2.125000 0.992157,2.000000 1.000000,1.875000 1.484123,1.750000 1.661438,1.625000 1.780625,1.500000 1.866025,1.375000 1.927025,1.250000 1.968246,1.125000 1.992157,1.000000 2.000000,0.750000 2.661438,0.500000 2.866025,0.250000 2.968246,0.000000 3.000000,-0.250000 2.968246,-0.500000 2.866025,-0.750000 2.661438,-1.000000 2.000000,-1.125000 1.992157,-1.250000 1.968246,-1.375000 1.927025,-1.500000 1.866025,-1.625000 1.780625,-1.750000 1.661438,-1.875000 1.484123,-2.000000 1.000000,-2.125000 0.992157,-2.250000 0.968246,-2.375000 0.927025,-2.500000 0.866025,-2.625000 0.780625,-2.750000 0.661438,-2.875000 0.484123,-3.000000 0.000000,-2.875000 -0.484123,-2.750000 -0.661438,-2.625000 -0.780625,-2.500000 -0.866025,-2.375000 -0.927025,-2.250000 -0.968246,-2.125000 -0.992157,-2.000000 -1.000000,-1.875000 -1.484123,-1.750000 -1.661438,-1.625000 -1.780625,-1.500000 -1.866025,-1.375000 -1.927025,-1.250000 -1.968246,-1.125000 -1.992157,-1.000000 -2.000000,-0.750000 -2.661438,-0.500000 -2.866025,-0.250000 -2.968246,0.000000 -3.000000,0.250000 -2.968246,0.500000 -2.866025,0.750000 -2.661438,1.000000 -2.000000,1.125000 -1.992157,1.250000 -1.968246,1.375000 -1.927025,1.500000 -1.866025,1.625000 -1.780625,1.750000 -1.661438,1.875000 -1.484123,2.000000 -1.000000,2.125000 -0.992157,2.250000 -0.968246,2.375000 -0.927025,2.500000 -0.866025,2.625000 -0.780625,2.750000 -0.661438,2.875000 -0.484123,3.000000 0.000000),(0.000000 1.000000,0.125000 0.515877,0.250000 0.338562,0.375000 0.219375,0.500000 0.133975,0.625000 0.072975,0.750000 0.031754,0.875000 0.007843,1.000000 0.000000,0.875000 -0.007843,0.750000 -0.031754,0.625000 -0.072975,0.500000 -0.133975,0.375000 -0.219375,0.250000 -0.338562,0.125000 -0.515877,0.000000 -1.000000,-0.125000 -0.515877,-0.250000 -0.338562,-0.375000 -0.219375,-0.500000 -0.133975,-0.625000 -0.072975,-0.750000 -0.031754,-0.875000 -0.007843,-1.000000 0.000000,-0.875000 0.007843,-0.750000 0.031754,-0.625000 0.072975,-0.500000 0.133975,-0.375000 0.219375,-0.250000 0.338562,-0.125000 0.515877,0.000000 1.000000)))" ) );
    std::unique_ptr< MultiLineString > result( algorithm::approximateMedialAxis( *g ) ) ;
    BOOST_CHECK_EQUAL( result->numGeometries(), 108U );
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
      std::unique_ptr< Geometry > g( io::readWkt( *it ) );
      std::unique_ptr< MultiLineString > result(
          algorithm::approximateMedialAxis( *g )
      );
      BOOST_CHECK_EQUAL( result->numGeometries(), 0U );
    }
}


BOOST_AUTO_TEST_SUITE_END()

