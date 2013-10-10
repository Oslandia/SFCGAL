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


#include <SFCGAL/detail/graph/GeometryGraph.h>
#include <SFCGAL/detail/graph/GeometryGraphBuilder.h>

using namespace boost::unit_test ;

using namespace SFCGAL ;
using namespace SFCGAL::graph ;


BOOST_AUTO_TEST_SUITE( SFCGAL_GeometryGraphBuilderTest )

BOOST_AUTO_TEST_CASE( addPoint )
{
    typedef GeometryGraph::vertex_descriptor vertex_descriptor ;
    //typedef GeometryGraph::edge_descriptor   edge_descriptor ;
    GeometryGraph        graph;
    GeometryGraphBuilder graphBuilder( graph );

    vertex_descriptor a = graphBuilder.addPoint( Point( 0.0,0.0,0.0 ) );
    vertex_descriptor b = graphBuilder.addPoint( Point( 1.0,1.0,1.0 ) );
    vertex_descriptor c = graphBuilder.addPoint( Point( 2.0,2.0,2.0 ) );

    //b duplicate
    vertex_descriptor d = graphBuilder.addPoint( Point( 1.0,1.0,1.0 ) );

    BOOST_CHECK_EQUAL( graph.numVertices(), 3U );
    BOOST_CHECK_EQUAL( graph.numEdges(), 0U );
    BOOST_CHECK_EQUAL( b, d );

    BOOST_CHECK( graph[ a ].coordinate == Coordinate( 0.0,0.0,0.0 ) );
    BOOST_CHECK( graph[ b ].coordinate == Coordinate( 1.0,1.0,1.0 ) );
    BOOST_CHECK( graph[ c ].coordinate == Coordinate( 2.0,2.0,2.0 ) );
}



BOOST_AUTO_TEST_CASE( addLineString )
{
    //typedef GeometryGraph::vertex_descriptor vertex_descriptor ;
    typedef GeometryGraph::edge_descriptor   edge_descriptor ;
    GeometryGraph        graph;
    GeometryGraphBuilder graphBuilder( graph );

    std::vector< Point > points ;
    points.push_back( Point( 0.0,0.0,0.0 ) );
    points.push_back( Point( 1.0,0.0,0.0 ) );
    points.push_back( Point( 1.0,1.0,0.0 ) );
    points.push_back( Point( 0.0,1.0,0.0 ) );
    points.push_back( Point( 0.0,0.0,0.0 ) );

    LineString lineString( points );
    std::vector< edge_descriptor > sharedLineString = graphBuilder.addLineString( lineString );

    BOOST_CHECK_EQUAL( graph.numVertices(), 4U );
    BOOST_CHECK_EQUAL( graph.numEdges(), 4U );

    //check closed
    for ( size_t i = 0; i < sharedLineString.size(); i++ ) {
        BOOST_CHECK( graph.target( sharedLineString[ i ] ) == graph.source( sharedLineString[ ( i+1 ) % 4 ] ) ) ;
    }
}


BOOST_AUTO_TEST_SUITE_END()




