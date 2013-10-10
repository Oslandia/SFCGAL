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
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/detail/triangulate/ConstraintDelaunayTriangulation.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;
using namespace SFCGAL::triangulate ;

BOOST_AUTO_TEST_SUITE( SFCGAL_triangulate_ConstraintDelaunayTriangulationTest )

/// Coordinate() ;
BOOST_AUTO_TEST_CASE( testDefaultConstructor )
{
    ConstraintDelaunayTriangulation triangulation ;
    BOOST_CHECK_EQUAL( triangulation.numVertices(), 0U );
    BOOST_CHECK_EQUAL( triangulation.numTriangles(), 0U );
}

BOOST_AUTO_TEST_CASE( testTriangulateSquare )
{
    ConstraintDelaunayTriangulation triangulation ;
    typedef ConstraintDelaunayTriangulation::Vertex_handle         Vertex_handle ;
    typedef ConstraintDelaunayTriangulation::Face_handle           Face_handle ;
    typedef ConstraintDelaunayTriangulation::All_faces_iterator    All_faces_iterator ;
    //typedef ConstraintDelaunayTriangulation::Finite_faces_iterator Finite_faces_iterator ;

    Vertex_handle a = triangulation.addVertex( Coordinate( 0.0,0.0 ) ) ;
    Vertex_handle b = triangulation.addVertex( Coordinate( 1.0,0.0 ) ) ;
    Vertex_handle c = triangulation.addVertex( Coordinate( 1.0,1.0 ) ) ;
    Vertex_handle d = triangulation.addVertex( Coordinate( 0.0,1.0 ) ) ;

    BOOST_CHECK_EQUAL( triangulation.numVertices(), 4U );
    BOOST_CHECK_EQUAL( triangulation.numTriangles(), 2U );

    triangulation.addConstraint( a, b );
    triangulation.addConstraint( b, c );
    triangulation.addConstraint( c, d );
    triangulation.addConstraint( d, a );

    // constraint have no impact
    BOOST_CHECK_EQUAL( triangulation.numVertices(), 4U );
    BOOST_CHECK_EQUAL( triangulation.numTriangles(), 2U );


    /*
     * nesting level
     */
    for ( All_faces_iterator it = triangulation.all_faces_begin(); it != triangulation.all_faces_end(); ++it ) {
        BOOST_CHECK_EQUAL( it->info().nestingLevel, -1 );
    }

    // check mark domains
    triangulation.markDomains();

    for ( All_faces_iterator it = triangulation.all_faces_begin(); it != triangulation.all_faces_end(); ++it ) {
        Face_handle face = it ;

        if ( triangulation.isInfinite( face ) ) {
            BOOST_CHECK_EQUAL( it->info().nestingLevel, 0 );
        }
        else {
            BOOST_CHECK_EQUAL( it->info().nestingLevel, 1 );
        }
    }
}




BOOST_AUTO_TEST_CASE( testProjectionPlane )
{
    ConstraintDelaunayTriangulation triangulation ;
    //typedef ConstraintDelaunayTriangulation::Vertex_handle         Vertex_handle ;
    //typedef ConstraintDelaunayTriangulation::Face_handle           Face_handle ;

    triangulation.setProjectionPlane( Kernel::Plane_3( Kernel::RT( 1 ), Kernel::RT( 0 ), Kernel::RT( 0 ), Kernel::RT( 0 ) ) );

    triangulation.addVertex( Coordinate( 1.0,0.0,0.0 ) ) ;
    triangulation.addVertex( Coordinate( 1.0,1.0,0.0 ) ) ;
    triangulation.addVertex( Coordinate( 1.0,1.0,1.0 ) ) ;
    triangulation.addVertex( Coordinate( 1.0,0.0,1.0 ) ) ;

    BOOST_CHECK_EQUAL( triangulation.numVertices(), 4U );
    BOOST_CHECK_EQUAL( triangulation.numTriangles(), 2U );
}



BOOST_AUTO_TEST_SUITE_END()




