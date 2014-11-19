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

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_CoordinateTest )

BOOST_AUTO_TEST_CASE( testRoundPoint )
{
    std::auto_ptr< Geometry > g( io::readWkt( "POINT(1.5 2.6 3.4)" ) );
    g->round();
    BOOST_CHECK_EQUAL( g->asText(), "POINT(2/1 3/1 3/1)" );
}

BOOST_AUTO_TEST_CASE( testRoundLineString )
{
    std::auto_ptr< Geometry > g( io::readWkt( "LINESTRING(0.5 0.5,1.5 1.5)" ) );
    g->round( 10 );
    BOOST_CHECK_EQUAL( g->asText(), "LINESTRING(1/2 1/2,3/2 3/2)" );
}

BOOST_AUTO_TEST_SUITE_END()




