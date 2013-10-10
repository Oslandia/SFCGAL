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

#include <cmath>

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
#include <SFCGAL/detail/algorithm/coversPoints.h>

using namespace SFCGAL ;
using namespace SFCGAL::detail ;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_CoversPointsTest )

BOOST_AUTO_TEST_CASE( testPointPointCoversPoints )
{
    Point pta( 0.0, 1.0, 0.0 );
    Point ptb( 0.0, 1.0, 0.0 );
    Point ptc( 0.0, 0.0, 0.0 );
    BOOST_CHECK_EQUAL( algorithm::coversPoints( pta, ptb ), true );
    BOOST_CHECK_EQUAL( algorithm::coversPoints( pta, ptc ), false );
    BOOST_CHECK_EQUAL( algorithm::coversPoints3D( pta, ptb ), true );
    BOOST_CHECK_EQUAL( algorithm::coversPoints3D( pta, ptc ), false );
}

BOOST_AUTO_TEST_CASE( testPolygonPolygonCoversPoints )
{
    {
        std::auto_ptr<Geometry> p1 = io::readWkt( "POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0))" );
        std::auto_ptr<Geometry> p2 = io::readWkt( "POLYGON((-0.5 -0.5,-0.5 0.5,0.5 0.5,0.5 -0.5,-0.5 -0.5))" );

        BOOST_CHECK_EQUAL( algorithm::coversPoints( *p1, *p2 ), true );
        BOOST_CHECK_EQUAL( algorithm::coversPoints3D( *p1, *p2 ), true );
    }

    {
        // a square with a substracted triangle => concave shape
        std::auto_ptr<Geometry> p1 = io::readWkt( "POLYGON((0.4 0,0 0,0 1,1 1,1 0,0.6 0,0.5 0.4,0.4 0))" );
        // a smaller square
        std::auto_ptr<Geometry> p2 = io::readWkt( "POLYGON((0.2 0.2,0.8 0.2,0.8 0.8,0.2 0.8,0.2 0.2))" );

        // ST_covers would answer false
        BOOST_CHECK_EQUAL( algorithm::coversPoints( *p1, *p2 ), true );
        BOOST_CHECK_EQUAL( algorithm::coversPoints3D( *p1, *p2 ), true );
    }
}

BOOST_AUTO_TEST_CASE( testCollectionCoversPoints )
{
#if 0
    {
        std::auto_ptr<Geometry> p1 = io::readWkt( "GEOMETRYCOLLECTION(LINESTRING(0 0 0,0 0 1),LINESTRING(0 0 0,0 1 0),LINESTRING(0 1 1,0 0 1),LINESTRING(0 1 1,0 1 0))" );
        std::auto_ptr<Geometry> p2 = io::readWkt( "TIN(((0 0.5 0.5,0 0 1,0 0 0,0 0.5 0.5)),((0 0 1,0 0.5 0.5,0 1 1,0 0 1)),((0 0.5 0.5,0 0 0,0 1 0,0 0.5 0.5)),((0 0.5 0.5,0 1 0,0 1 1,0 0.5 0.5)))" );
        std::cout << "p1 covers p2 ? " << algorithm::coversPoints3D( *p1, *p2 ) << std::endl;
        std::cout << "p2 covers p1 ? " << algorithm::coversPoints3D( *p2, *p1 ) << std::endl;
    }

    {
        std::auto_ptr<Geometry> p1 = io::readWkt( "GEOMETRYCOLLECTION(TRIANGLE((1 1,0.5 0.5,0 1,1 1)),TRIANGLE((1 0,0.5 0.5,1 1,1 0)),TRIANGLE((0.5 0.5,0 0,0 1,0.5 0.5)))" );
        std::auto_ptr<Geometry> p2 = io::readWkt( "TRIANGLE((1 0, 0 0,0.5 0.5,1 0))" );
        std::cout << "p1 covers p2 ? " << algorithm::coversPoints3D( *p1, *p2 ) << std::endl;
        std::cout << "p2 covers p1 ? " << algorithm::coversPoints3D( *p2, *p1 ) << std::endl;
    }
#endif
}

BOOST_AUTO_TEST_SUITE_END()

