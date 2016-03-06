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

#include <SFCGAL/Exception.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/triangulate/triangulatePolygon3D.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;
using namespace SFCGAL::triangulate ;

BOOST_AUTO_TEST_SUITE( SFCGAL_triangulate_TriangulatePolygon3DTest )

BOOST_AUTO_TEST_CASE( testPolygon )
{
    Geometry<Epeck> g = io::readWkt<Epeck>( "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0))" ) ;
    TriangulatedSurface<Epeck> tin;
    triangulatePolygon3D<Epeck>(
        boost::get<Polygon<Epeck>>(g),
        tin
    );
    BOOST_CHECK_EQUAL(tin.size(),2U);
    //TODO check area
}


BOOST_AUTO_TEST_CASE( testPolygonWithHole )
{
    Geometry<Epeck> g = io::readWkt<Epeck>( "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0),(0.2 0.2,0.2 0.8,0.8 0.8,0.8 0.2,0.2 0.2))" ) ;
    TriangulatedSurface<Epeck> tin;
    triangulatePolygon3D<Epeck>(
        boost::get<Polygon<Epeck>>(g),
        tin
    );
    BOOST_CHECK_EQUAL(tin.size(),8U);
    //TODO check area
}

// test behavior with constraint intersections (here invalid polygon)
BOOST_AUTO_TEST_CASE( testPolygonWithHoleTouching )
{
    Geometry<Epeck> g = io::readWkt<Epeck>( 
        "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0),(0.5 0.0,1.0 0.5,0.3 0.7,0.5 0.0))" 
    ) ;
    TriangulatedSurface<Epeck> tin;
    triangulatePolygon3D<Epeck>(
        boost::get<Polygon<Epeck>>(g),
        tin
    );
    BOOST_CHECK_EQUAL(tin.size(),5U);
    //TODO check area
}

BOOST_AUTO_TEST_SUITE_END()




