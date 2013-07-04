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

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/orientation.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_OrientationTest )

//TODO COMPLETE TEST

//void makeValidOrientation( CGAL::Polygon_2< Kernel > & polygon ) ;
//void makeValidOrientation( CGAL::Polygon_with_holes_2< Kernel > & polygon ) ;
//void makeValidOrientation( Polygon & polygon ) ;


//bool hasConsistentOrientation3D( const TriangulatedSurface & g ) ;
BOOST_AUTO_TEST_CASE( testHasConsistentOrientation3D_basicTriangles )
{
    TriangulatedSurface triangulatedSurface;
    BOOST_CHECK( algorithm::hasConsistentOrientation3D( triangulatedSurface ) );
    triangulatedSurface.addTriangle(
        Triangle(
            Point( 0.0,0.0,0.0 ),
            Point( 1.0,0.0,0.0 ),
            Point( 0.0,1.0,0.0 )
        )
    );
    BOOST_CHECK( algorithm::hasConsistentOrientation3D( triangulatedSurface ) );

    triangulatedSurface.addTriangle(
        Triangle(
            Point( 0.0,0.0,0.0 ),
            Point( 0.0,1.0,0.0 ),
            Point( -1.0,0.0,0.0 )
        )
    );
    BOOST_CHECK( algorithm::hasConsistentOrientation3D( triangulatedSurface ) );

    triangulatedSurface.addTriangle(
        Triangle(
            Point( 0.0, 0.0, 0.0 ),
            Point( 1.0, 0.0, 0.0 ),
            Point( 0.0,-1.0, 0.0 )
        )
    );
    BOOST_CHECK( ! algorithm::hasConsistentOrientation3D( triangulatedSurface ) );
}



//bool hasConsistentOrientation3D( const PolyhedralSurface & g ) ;

BOOST_AUTO_TEST_CASE( testHasConsistentOrientation3D_basicPolygons )
{
    PolyhedralSurface polyhedralSurface ;
    BOOST_CHECK( algorithm::hasConsistentOrientation3D( polyhedralSurface ) );

    //base polygon
    {
        LineString ring ;
        ring.addPoint( Point( 0.0,0.0,0.0 ) );
        ring.addPoint( Point( 1.0,0.0,0.0 ) );
        ring.addPoint( Point( 1.0,1.0,0.0 ) );
        ring.addPoint( Point( 0.0,1.0,0.0 ) );
        ring.addPoint( ring.startPoint() ) ;

        polyhedralSurface.addPolygon( Polygon( ring ) );
    }
    BOOST_CHECK( algorithm::hasConsistentOrientation3D( polyhedralSurface ) );

    //right polygon
    {
        LineString ring ;
        ring.addPoint( Point( 1.0,0.0,0.0 ) );
        ring.addPoint( Point( 1.0,0.0,1.0 ) );
        ring.addPoint( Point( 1.0,1.0,1.0 ) );
        ring.addPoint( Point( 1.0,1.0,0.0 ) );
        ring.addPoint( ring.startPoint() ) ;

        polyhedralSurface.addPolygon( Polygon( ring ) );
    }
    BOOST_CHECK( algorithm::hasConsistentOrientation3D( polyhedralSurface ) );

    //left polygon (bad orientation)
    {
        LineString ring ;
        ring.addPoint( Point( 0.0,0.0,0.0 ) );
        ring.addPoint( Point( 0.0,0.0,1.0 ) );
        ring.addPoint( Point( 0.0,1.0,1.0 ) );
        ring.addPoint( Point( 0.0,1.0,0.0 ) );
        ring.addPoint( ring.startPoint() ) ;

        polyhedralSurface.addPolygon( Polygon( ring ) );
    }
    BOOST_CHECK( ! algorithm::hasConsistentOrientation3D( polyhedralSurface ) );
}



//void makeConsistentOrientation3D( TriangulatedSurface & g ) ;

//bool isCounterClockWiseOriented( const Polygon& );
BOOST_AUTO_TEST_CASE( testIsCounterClockWiseOriented_Polygon )
{
    LineString ring ;
    ring.addPoint( Point( 0.0,0.0 ) );
    ring.addPoint( Point( 1.0,0.0 ) );
    ring.addPoint( Point( 1.0,1.0 ) );
    ring.addPoint( Point( 0.0,1.0 ) );
    ring.addPoint( ring.startPoint() ) ;

    Polygon polygon( ring ) ;
    BOOST_CHECK( algorithm::isCounterClockWiseOriented( polygon ) );
    polygon.exteriorRing().reverse();
    BOOST_CHECK( ! algorithm::isCounterClockWiseOriented( polygon ) );
}

//bool isCounterClockWiseOriented( const Triangle& );
BOOST_AUTO_TEST_CASE( testIsCounterClockWiseOriented_Triangle )
{
    Triangle triangle(
        Point( 0.0,0.0 ),
        Point( 1.0,0.0 ),
        Point( 1.0,1.0 )
    );
    BOOST_CHECK( algorithm::isCounterClockWiseOriented( triangle ) );
    triangle.reverse();
    BOOST_CHECK( ! algorithm::isCounterClockWiseOriented( triangle ) );
}


//bool isCounterClockWiseOriented( const LineString& );
BOOST_AUTO_TEST_CASE( testIsCounterClockWiseOriented_LineString )
{
    LineString ring ;
    ring.addPoint( Point( 0.0,0.0 ) ) ;
    ring.addPoint( Point( 1.0,0.0 ) ) ;
    ring.addPoint( Point( 1.0,1.0 ) ) ;
    ring.addPoint( Point( 0.0,1.0 ) ) ;
    ring.addPoint( ring.startPoint() ) ;

    BOOST_CHECK( algorithm::isCounterClockWiseOriented( ring ) );
    ring.reverse();
    BOOST_CHECK( ! algorithm::isCounterClockWiseOriented( ring ) );
}




BOOST_AUTO_TEST_SUITE_END()

