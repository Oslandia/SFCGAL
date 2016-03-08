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

#include <SFCGAL/Geometry.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/normal.h>

using namespace SFCGAL ;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_NormalTest )

BOOST_AUTO_TEST_CASE( testNormal1 )
{
    typedef CGAL::Vector_3< Epeck > Vector_3 ;
    typedef CGAL::Point_3< Epeck > Point_3 ;

    Point_3 a( 0.0, 0.0, 0.0 );
    Point_3 b( 1.0, 0.0, 0.0 );
    Point_3 c( 1.0, 1.0, 0.0 );

    Vector_3 normal = algorithm::normal3D( a, b, c );
    BOOST_CHECK_EQUAL( normal.x(), 0.0 );
    BOOST_CHECK_EQUAL( normal.y(), 0.0 );
    BOOST_CHECK_EQUAL( normal.z(), 1.0 );
}

BOOST_AUTO_TEST_CASE( testNormal2 )
{
    // a square ccw
    std::unique_ptr< Geometry<Epeck> > gA( io::readWkt<Epeck>( "POLYGON((0 0,1 0,1 1,0 1,0 0))" ) );
    // a square cw oriented
    std::unique_ptr< Geometry<Epeck> > gB( io::readWkt<Epeck>( "POLYGON((0 0,0 1,1 1,1 0,0 0))" ) );

    // a pseudo-square ccw oriented, with a concave part
    std::unique_ptr< Geometry<Epeck> > gC( io::readWkt<Epeck>( "POLYGON((0 0,0.5 0.5,1 0,1 1,0 1,0 0))" ) );

    {
        CGAL::Vector_3<Epeck> normal = algorithm::normal3D<Epeck>( gA->as< Polygon<Epeck> >() );
        BOOST_CHECK_EQUAL( normal.x(), 0.0 );
        BOOST_CHECK_EQUAL( normal.y(), 0.0 );
        BOOST_CHECK_EQUAL( normal.z(), 2.0 );
    }

    {
        CGAL::Vector_3<Epeck> normal = algorithm::normal3D<Epeck>( gB->as< Polygon<Epeck> >() );
        BOOST_CHECK_EQUAL( normal.x(), 0.0 );
        BOOST_CHECK_EQUAL( normal.y(), 0.0 );
        BOOST_CHECK_EQUAL( normal.z(), -2.0 );
    }

    {
        CGAL::Vector_3<Epeck> normal = algorithm::normal3D<Epeck>( gC->as< Polygon<Epeck> >() );
        BOOST_CHECK_EQUAL( normal.x(), 0.0 );
        BOOST_CHECK_EQUAL( normal.y(), 0.0 );
        // ok, the normal is pointing up (z > 0)
        BOOST_CHECK_EQUAL( normal.z(), 1.5 );
    }
}


BOOST_AUTO_TEST_CASE( testNormal3 )
{
    std::unique_ptr< Geometry<Epeck> > gA( io::readWkt<Epeck>( "POLYGON((0 1 0,0 1 1,1 1 1,1 1 0,0 1 0))" ) );
    // exact
    {
        CGAL::Vector_3<Epeck> normal = algorithm::normal3D<Epeck>( gA->as<Polygon<Epeck>>(), true );
        //std::cout << CGAL::exact(normal) << std::endl;
        CGAL::Plane_3<Epeck> plane( gA->as<Polygon<Epeck>>().exteriorRing().startPoint().toPoint_3(), normal );
        //std::cout << CGAL::exact(plane) << std::endl;
        BOOST_CHECK( ! plane.is_degenerate() );
    }
    // round
    {
        CGAL::Vector_3<Epeck> normal = algorithm::normal3D<Epeck>( gA->as<Polygon<Epeck>>(), false );
        //std::cout << CGAL::exact(normal) << std::endl;
        CGAL::Plane_3<Epeck> plane( gA->as<Polygon<Epeck>>().exteriorRing().startPoint().toPoint_3(), normal );
        //std::cout << CGAL::exact(plane) << std::endl;
        BOOST_CHECK( ! plane.is_degenerate() );
    }

}



BOOST_AUTO_TEST_SUITE_END()

