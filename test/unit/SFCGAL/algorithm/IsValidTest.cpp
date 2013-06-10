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
#include <boost/test/unit_test.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/isValid.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_IsValid )

BOOST_AUTO_TEST_CASE( pointIsValid )
{
    {
        BOOST_CHECK( algorithm::isValid( Point() ) );
    }
    {
        std::auto_ptr< Geometry > g( io::readWkt("POINT(1 2)") );
        BOOST_CHECK( algorithm::isValid( *g ) );
    }
    {
        std::auto_ptr< Geometry > g( io::readWkt("POINT(1 2 3)") );
        BOOST_CHECK( algorithm::isValid( *g ) );
    }
}


BOOST_AUTO_TEST_CASE( lineStringIsValid )
{
    {
        BOOST_CHECK( algorithm::isValid( LineString() ) );
    }
    {
        std::auto_ptr< Geometry > g( io::readWkt("LINESTRING(0 0,0 1,1 1,1 0,0 0)") );
        BOOST_CHECK( algorithm::isValid( *g ) );
    }
    {
        std::auto_ptr< Geometry > g( io::readWkt("LINESTRING(-1.0 -1.0,-1.0 -1.0)") );
        BOOST_CHECK( !algorithm::isValid( *g ) );
    }
}


BOOST_AUTO_TEST_SUITE_END()

/*
# Point2D 
# valid
POINT(-1.0 -1.0)
# limit
# invalid
###
# Point3D 
# valid
POINT(-1.0 -1.0 -1.0)
# limit
# invalid
###
# Linestring2D 
# valid
LINESTRING(-1.0 -1.0,1.0 1.0)
# limit
# invalid
LINESTRING(-1.0 -1.0,-1.0 -1.0)
###
# Linestring3D 
# valid
LINESTRING(-1.0 -1.0 -1.0,1.0 1.0 1.0)
# limit
# invalid
LINESTRING(-1.0 -1.0 -1.0,-1.0 -1.0 -1.0)
###
# Polygon2D
# valid
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0))
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.5 -0.5,0.5 0.5,-0.5 0.5,-0.5 -0.5))
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,1.0 -0.5,0.5 0.5,-0.5 0.5,-0.5 -0.5)) # one contact point between interior ans exterior
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.1 -0.5,-0.1 0.5,-0.5 0.5,-0.5 -0.5),(0.1 -0.5,0.5 -0.5,0.5 0.5,0.1 0.5,0.1 -0.5))
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.1 -0.5,-0.1 0.5,-0.5 0.5,-0.5 -0.5),(0.1 -0.5,0.5 -0.5,0.5 0.5,0.1 0.5,0.1 -0.5)) # one contact point between 2 interior rings
# limit
# invalid
POLYGON((-1.0 -1.0,-1.0 1.0,-1.0 -1.0)) # only 3 points
POLYGON((-1.0 -1.0,-1.0 1.0,1.0 1.0,-1.0 -1.0,-1.0 1.0)) # not closed
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 -1.0,-1.0 -1.0,-1.0 -1.0)) # zero surface
POLYGON((-1.0 -1.0,2.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0)) # ring adjacency (spyke)
POLYGON((-1.0 -1.0,1.0 1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0)) # ring intersection
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,-0.5 -0.5)) # interior ring only 3 points
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,-0.5 0.5,0.5 0.5,-0.5 -0.5,-0.5 0.5)) # interior ring not closed
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.5 -0.5,0.5 -0.5,-0.5 -0.5,-0.5 -0.5)) # interior ring no surface
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.7 -0.5,0.5 -0.5,0.5 0.5,-0.5 0.5,-0.5 -0.5)) # interior ring adjacency (spyke)
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.5 0.5,0.5 -0.5,-0.5 0.5,-0.5 -0.5)) # interior ring intersection
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,1.0 -0.5,1.0 0.5,-0.5 0.5,-0.5 -0.5)) # interior ring adjacent to exterior
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,2.0 -0.5,2.0 0.5,-0.5 0.5,-0.5 -0.5)) # interior ring intersection with exterior
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(1.5 -0.5,2.0 -0.5,2.0 0.5,1.5 0.5,1.5 -0.5)) # interior ring is ouside exterior
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.0 -0.5,0.0 0.5,-0.5 0.5,-0.5 -0.5),(0.0 -0.5,0.5 -0.5,0.5 0.5,0.0 0.5,0.0 -0.5)) # adjacent interior rings
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.2 -0.4,-0.1 0.5,-0.5 0.5,-0.5 -0.5),(0.1 -0.5,0.5 -0.5,0.5 0.5,0.1 0.5,0.1 -0.5)) # intersection between interior rings
POLYGON((-1.0 -1.0,1.0 -1.0,1.0 1.0,-1.0 1.0,-1.0 -1.0),(-0.5 -0.5,0.5 -0.5,0.5 0.5,-0.5 0.5,-0.5 -0.5),(-0.2 -0.2,0.2 -0.2,0.2 0.2,-0.2 0.2,-0.2 -0.2)) # one inetrior ring is inside the other
###
# Polygon3D
# valid
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0))
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5))
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 1.0 -0.5,1.0 0.5 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5)) # one contact point between interior ans exterior
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 -0.1 -0.5,1.0 -0.1 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5),(1.0 0.1 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 0.1 0.5,1.0 0.1 -0.5))
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.1 -0.5,1.0 -0.1 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5),(1.0 0.1 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 0.1 0.5,1.0 0.1 -0.5)) # one contact point between 2 interior rings
# limit
# invalid
POLYGON((1.0 -1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)) # only 3 points
POLYGON((1.0 -1.0 -1.0,1.0 -1.0 1.0,1.0 1.0 1.0,1.0 -1.0 -1.0,1.0 -1.0 1.0)) # not closed
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 -1.0,1.0 -1.0 -1.0)) # zero surface
POLYGON((0.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)) # non plane
POLYGON((1.0 -1.0 -1.0,1.0 2.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)) # ring adjacency (1.0 spyke)
POLYGON((1.0 -1.0 -1.0,1.0 1.0 1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)) # ring intersection
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5)) # interior ring only 3 points
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 -0.5 0.5,1.0 0.5 0.5,1.0 -0.5 -0.5,1.0 -0.5 0.5)) # interior ring not closed
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.5 -0.5,1.0 0.5 -0.5,1.0 -0.5 -0.5,1.0 -0.5 -0.5)) # interior ring no surface
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.7 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5)) # interior ring adjacency (1.0 spyke)
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.5 0.5,1.0 0.5 -0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5)) # interior ring intersection
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 1.0 -0.5,1.0 1.0 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5)) # interior ring adjacent to exterior
POLYGON((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 2.0 -0.5,1.0 2.0 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5)) # interior ring intersection with exterior
POLYGON(1.0 (1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 1.5 -0.5,1.0 2.0 -0.5,1.0 2.0 0.5,1.0 1.5 0.5,1.0 1.5 -0.5)) # interior ring is ouside exterior
POLYGON(1.0 (1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.0 -0.5,1.0 0.0 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5),(1.0 0.0 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 0.0 0.5,1.0 0.0 -0.5)) # adjacent interior rings
POLYGON(1.0 (1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.2 -0.4,1.0 -0.1 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5),(1.0 0.1 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 0.1 0.5,1.0 0.1 -0.5)) # intersection between interior rings
POLYGON(1.0 (1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0),(1.0 -0.5 -0.5,1.0 0.5 -0.5,1.0 0.5 0.5,1.0 -0.5 0.5,1.0 -0.5 -0.5),(1.0 -0.2 -0.2,1.0 0.2 -0.2,1.0 0.2 0.2,1.0 -0.2 0.2,1.0 -0.2 -0.2)) # one inetrior ring is inside the other
###
# Multipoint2D
# valid
MULTIPOINT((-1.0 -1.0),(1.0 1.0))
# limit
# invalid
###
# Multipoint3D
# valid
MULTIPOINT((-1.0 -1.0 -1),(1.0 1.0 1.0))
# limit
# invalid
###
# MultiLinestring2D 
# valid
MULTILINESTRING((-1.0 -1.0,1.0 1.0),(1.0 1.0,1.0 -1.0))
# limit
# invalid
MULTILINESTRING((-1.0 -1.0,1.0 1.0),(1.0 1.0,1.0 1.0))
###
# MultiLinestring3D 
# valid
MULTILINESTRING((1.0 -1.0 -1.0,1.0 1.0 1.0),(1.0 1.0 1.0,1.0 1.0 -1.0))
# limit
# invalid
MULTILINESTRING((1.0 -1.0 -1.0,1.0 1.0 1.0),(1.0 1.0 1.0,1.0 1.0 1.0))
###
# MultiPolygon2D
# valid
MULTIPOLYGON(((-3.0 -1.0,-1.0 -1.0,-1.0 1.0,-3.0 1.0,-3.0 -1.0)),((1.0 -1.0,3.0 -1.0,3.0 1.0,1.0 1.0,1.0 -1.0)))
MULTIPOLYGON(((-3.0 -1.0,1.0 -1.0,-1.0 1.0,-3.0 1.0,-3.0 -1.0)),((1.0 -1.0,3.0 -1.0,3.0 1.0,1.0 1.0,1.0 -1.0))) # one contact point
# limit
# invalid
MULTIPOLYGON(((-3.0 -1.0,-1.0 -1.0,-1.0 1.0,-3.0 1.0,-3.0 -1.0)),((1.0 -1.0,3.0 -1.0,3.0 1.0,1.0 1.0))) # second polygon is invalid
MULTIPOLYGON(((-2.0 -1.0,0.0 -1.0,0.0 1.0,-2.0 1.0,-2.0 -1.0)),((0.0 -1.0,2.0 -1.0,2.0 1.0,0.0 1.0,0.0 -1.0))) # adjacent
MULTIPOLYGON(((-3.0 -1.0,1.3 0.0,-1.0 1.0,-3.0 1.0,-3.0 -1.0)),((1.0 -1.0,3.0 -1.0,3.0 1.0,1.0 1.0,1.0 -1.0))) # intersection
###
# MultiPolygon3D
# valid
MULTIPOLYGON(((1.0 -3.0 -1.0,1.0 -1.0 -1.0,1.0 -1.0 1.0,1.0 -3.0 1.0,1.0 -3.0 -1.0)),((1.0 1.0 -1.0,1.0 3.0 -1.0,1.0 3.0 1.0,1.0 1.0 1.0,1.0 1.0 -1.0)))
MULTIPOLYGON(((1.0 -3.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -3.0 1.0,1.0 -3.0 -1.0)),((1.0 1.0 -1.0,1.0 3.0 -1.0,1.0 3.0 1.0,1.0 1.0 1.0,1.0 1.0 -1.0))) # one contact point
# limit
# invalid
MULTIPOLYGON(((1.0 -3.0 -1.0,1.0 -1.0 -1.0,1.0 -1.0 1.0,1.0 -3.0 1.0,1.0 -3.0 -1.0)),((1.0 1.0 -1.0,1.0 3.0 -1.0,1.0 3.0 1.0,1.0 1.0 1.0))) # second polygon is invalid
MULTIPOLYGON(((1.0 -2.0 -1.0,1.0 0.0 -1.0,1.0 0.0 1.0,1.0 -2.0 1.0,1.0 -2.0 -1.0)),((1.0 0.0 -1.0,1.0 2.0 -1.0,1.0 2.0 1.0,1.0 0.0 1.0,1.0 0.0 -1.0))) # adjacent
MULTIPOLYGON(((1.0 -3.0 -1.0,1.0 1.3 0.0,1.0 -1.0 1.0,1.0 -3.0 1.0,1.0 -3.0 -1.0)),((1.0 1.0 -1.0,1.0 3.0 -1.0,1.0 3.0 1.0,1.0 1.0 1.0,1.0 1.0 -1.0))) # intersection
###
# Triangle2D
# valid
TRIANGLE((-1.0 -1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0))
# limit
# invalid
TRIANGLE((-1.0 -1.0,1.0 -1.0,-1.0 -1.0)) # only 3 points
TRIANGLE((-1.0 -1.0,1.0 -1.0,-1.0 -1.0,-1.0 -1.0)) # zero surface
# Triangle3D
# valid
TRIANGLE((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0))
# limit
# invalid
TRIANGLE((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 -1.0)) # only 3 points
TRIANGLE((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 -1.0,1.0 -1.0 -1.0)) # zero surface
###
# TIN2D
# valid
TIN(((-1.0 -1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0)),((-1.0 1.0,1.0 -1.0,1.0 1.0,-1.0 1.0)))
# limit
# invalid
TIN(((-1.0 -1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0)),((-1.0 1.0,1.0 -1.0,1.0 1.0))) # second triangle is invalid
TIN(((-1.0 -1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0)),((1.0 -1.0,-1.0 1.0,1.0 1.0,1.0 -1.0))) # inconsitent orientation
TIN(((-1.0 -1.0,1.0 -1.0,-1.0 1.0,-1.0 -1.0)),((0.0 1.0,2.0 -1.0,2.0 1.0,0.0 1.0))) # not connected
###
# TIN3D
# valid
TIN(((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)),((1.0 -1.0 1.0,1.0 1.0 -1.0,1.0 1.0 1.0,1.0 -1.0 1.0)))
# limit
# invalid
TIN(((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)),((1.0 -1.0 1.0,1.0 1.0 -1.0,1.0 1.0 1.0))) # second triangle is invalid
TIN(((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)),((1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 1.0 1.0,1.0 1.0 -1.0))) # inconsitent orientation
TIN(((1.0 -1.0 -1.0,1.0 1.0 -1.0,1.0 -1.0 1.0,1.0 -1.0 -1.0)),((1.0 0.0 1.0,1.0 2.0 -1.0,1.0 2.0 1.0,1.0 0.0 1.0))) # not connected
TIN(((0.0 0.0 0.0,1.0 0.0 0.0,0.0 1.0 0.0,0.0 0.0 0.0)),((0.0 0.0 0.0,0.0 1.0 0.0,0.0 0.0 1.0,0.0 0.0 0.0)),((0.0 0.0 0.0,0.0 0.0 1.0,0.5 0.1 -0.1,0.0 0.0 0.0)))# self intersect
###

*/
