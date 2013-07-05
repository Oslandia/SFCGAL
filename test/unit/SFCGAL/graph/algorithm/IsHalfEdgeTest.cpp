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

#include <SFCGAL/detail/graph/GeometryGraph.h>
#include <SFCGAL/detail/graph/algorithm/isHalfEdge.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;
using namespace SFCGAL::graph ;


BOOST_AUTO_TEST_SUITE( SFCGAL_graph_algorithm_IsHalfEdgeTest )


BOOST_AUTO_TEST_CASE( basicTestCase )
{
    GeometryGraph graph;
    typedef GeometryGraph::vertex_descriptor vertex_descriptor ;

    /*
        d   c   f   h

        a   b   e   g
    */

    vertex_descriptor a = graph.addVertex( Coordinate( 0.0,0.0 ) ) ;
    vertex_descriptor b = graph.addVertex( Coordinate( 1.0,0.0 ) ) ;
    vertex_descriptor c = graph.addVertex( Coordinate( 1.0,1.0 ) ) ;
    vertex_descriptor d = graph.addVertex( Coordinate( 0.0,1.0 ) ) ;

    vertex_descriptor e = graph.addVertex( Coordinate( 2.0,0.0 ) ) ;
    vertex_descriptor f = graph.addVertex( Coordinate( 2.0,1.0 ) ) ;

    vertex_descriptor g = graph.addVertex( Coordinate( 3.0,0.0 ) ) ;
    vertex_descriptor h = graph.addVertex( Coordinate( 3.0,1.0 ) ) ;


    graph.addEdge( a, b );
    graph.addEdge( b, c );
    graph.addEdge( c, d );
    graph.addEdge( d, a );
    BOOST_CHECK( algorithm::isHalfEdge( graph ) );
    graph.addEdge( c, b );
    graph.addEdge( c, f );
    graph.addEdge( f, e );
    graph.addEdge( e, b );
    BOOST_CHECK( algorithm::isHalfEdge( graph ) );

    graph.addEdge( e, g );
    graph.addEdge( g, h );
    graph.addEdge( h, f );
    BOOST_CHECK( algorithm::isHalfEdge( graph ) );
    graph.addEdge( f, e );
    BOOST_CHECK( ! algorithm::isHalfEdge( graph ) );
}


BOOST_AUTO_TEST_SUITE_END()




