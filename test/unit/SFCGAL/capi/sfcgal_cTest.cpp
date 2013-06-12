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
#include <SFCGAL/capi/sfcgal_c.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_sfcgal_cTest )

/// Coordinate() ;
BOOST_AUTO_TEST_CASE( testErrorOnBadGeometryType )
{
    std::auto_ptr<Geometry> l( io::readWkt("LINESTRING(0 0, 0 1)") );
    std::auto_ptr<Geometry> p( io::readWkt("POINT(0 2)") );
    sfcgal_geometry_t* gl = l.get();
    
    BOOST_CHECK_EQUAL( 2, sfcgal_linestring_num_points( gl ) ); // should succeed
    BOOST_CHECK( sfcgal_triangle_vertex( gl, 0 ) == 0 ); // should fail
    std::cerr << "... wrong geometry type is expected\n";
    sfcgal_geometry_t* gp = p.release();
    sfcgal_linestring_add_point( gl, gp ); // should succeed
    sfcgal_linestring_add_point( gl, gl ); // should fail
    std::cerr << "... wrong geometry type is expected\n";
    
}

BOOST_AUTO_TEST_SUITE_END()




