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

#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Plane_3.h>

#include <cmath>

typedef CGAL::Cartesian< double > Kernel ;
typedef Kernel::Vector_2          Vector_2 ;
typedef Kernel::Vector_3          Vector_3 ;
typedef Kernel::Point_2           Point_2 ;
typedef Kernel::Point_3           Point_3 ;
typedef CGAL::Plane_3< Kernel >   Plane_3 ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( CGAL_Plane3Test )

BOOST_AUTO_TEST_CASE( testTo2D_UnitNormalForPlane )
{
    Plane_3 plane( 1.0,0.0,0.0,0.0 );
    BOOST_CHECK_EQUAL( plane.to_2d( Point_3( 1.0,0.0,0.0 ) ), Point_2( 0.0,0.0 ) );
    BOOST_CHECK_EQUAL( plane.to_2d( Point_3( 1.0,0.0,1.0 ) ), Point_2( 0.0,1.0 ) );
    BOOST_CHECK_EQUAL( plane.to_2d( Point_3( 1.0,1.0,1.0 ) ), Point_2( 1.0,1.0 ) );
    BOOST_CHECK_EQUAL( plane.to_2d( Point_3( 1.0,1.0,0.0 ) ), Point_2( 1.0,0.0 ) );
}


BOOST_AUTO_TEST_SUITE_END()

