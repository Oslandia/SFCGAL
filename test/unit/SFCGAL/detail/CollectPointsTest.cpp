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
#include <SFCGAL/detail/collectPoints.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_detail_CollectPointsTest )

BOOST_AUTO_TEST_CASE( testPoint )
{
    Geometry<Epeck> g = Point<Epeck>(1.0, 2.0, 3.0) ;
    
    std::vector< Point<Epeck> > points ;
    detail::collectPoints(g,points);
    
    BOOST_CHECK_EQUAL( points.size(), 1U );
}

BOOST_AUTO_TEST_CASE( testLineString )
{
    LineString<Epeck> g{
        Point<Epeck>(0.0,0.0,0.0),
        Point<Epeck>(1.0,0.0,0.0),
        Point<Epeck>(1.0,1.0,0.0),
        Point<Epeck>(2.0,1.0,0.0)
    } ;
    
    std::vector< Point<Epeck> > points ;
    detail::collectPoints(g,points);
    
    BOOST_CHECK_EQUAL( points.size(), 4U );
    for ( size_t i = 0; i < points.size(); i++ ){
        BOOST_CHECK_EQUAL( points[i], g[i] );
    }
}

BOOST_AUTO_TEST_CASE( testPolygonWithHole )
{
    Geometry<Epeck> g = io::readWkt<Epeck>( "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0),(0.2 0.2,0.2 0.8,0.8 0.8,0.8 0.2,0.2 0.2))" ) ;
    std::vector< Point<Epeck> > points ;
    detail::collectPoints(g,points);
    BOOST_CHECK_EQUAL( points.size(), 10U );
}


BOOST_AUTO_TEST_CASE( testMultiPoint )
{
    Geometry<Epeck> g = io::readWkt<Epeck>( "MULTIPOINT((1.0 2.0 3.0),(2.0 3.0 6.0),(8.0 6.0 7.0),(2.0 1.0 6.0))" ) ;
    std::vector< Point<Epeck> > points ;
    detail::collectPoints(g,points);
    BOOST_CHECK_EQUAL( points.size(), 4U );
}


//MultiSolid


BOOST_AUTO_TEST_SUITE_END()




