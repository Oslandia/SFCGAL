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
#include <SFCGAL/capi/sfcgal_c.h>
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

#include <boost/test/unit_test.hpp>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_sfcgal_cTest )

bool hasError = false;

int on_error( const char* /*msg*/, ... )
{
    hasError = true;
    return 0;
}

/// Coordinate() ;
BOOST_AUTO_TEST_CASE( testErrorOnBadGeometryType )
{
    sfcgal_set_error_handlers( printf, on_error );

    std::auto_ptr<Geometry> l( io::readWkt( "LINESTRING(0 0, 0 1)" ) );
    std::auto_ptr<Geometry> p( io::readWkt( "POINT(0 2)" ) );
    sfcgal_geometry_t* gl = l.get();

    hasError = false;
    BOOST_CHECK_EQUAL( 2, sfcgal_linestring_num_points( gl ) ); // should succeed
    BOOST_CHECK( hasError == false );

    hasError = false;
    BOOST_CHECK( sfcgal_triangle_vertex( gl, 0 ) == 0 ); // should fail
    BOOST_CHECK( hasError == true );

    sfcgal_geometry_t* gp = p.release();
    hasError = false;
    sfcgal_linestring_add_point( gl, gp ); // should succeed
    BOOST_CHECK( hasError == false );

    hasError = false;
    sfcgal_linestring_add_point( gl, gl ); // should fail
    BOOST_CHECK( hasError == true );
}

BOOST_AUTO_TEST_SUITE_END()




