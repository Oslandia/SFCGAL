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
#include <SFCGAL/detail/graph/algorithm/isConnected.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;
using namespace SFCGAL::graph ;


BOOST_AUTO_TEST_SUITE( SFCGAL_graph_algorithm_IsConnectedTest )

BOOST_AUTO_TEST_CASE( trivialConnected )
{
	GeometryGraph graph;
	graph.addVertex( Coordinate(0.0,0.0) );
	BOOST_CHECK( algorithm::isConnected( graph ) );
}

BOOST_AUTO_TEST_CASE( trivialNotConnected )
{
	GeometryGraph graph;
	graph.addVertex( Coordinate(0.0,0.0) );
	graph.addVertex( Coordinate(1.0,1.0) );
	BOOST_CHECK( ! algorithm::isConnected( graph ) );
}


BOOST_AUTO_TEST_CASE( twoVertexAndOneEdge )
{
	GeometryGraph graph;

	graph.addEdge(
		graph.addVertex( Coordinate(0.0,0.0) ),
		graph.addVertex( Coordinate(1.0,1.0) )
	);

	BOOST_CHECK( algorithm::isConnected( graph ) );
}

BOOST_AUTO_TEST_CASE( testConnectParts )
{
	GeometryGraph graph;
	typedef GeometryGraph::vertex_descriptor vertex_descriptor ;

	vertex_descriptor a = graph.addVertex( Coordinate(0.0,0.0) ) ;
	vertex_descriptor b = graph.addVertex( Coordinate(1.0,1.0) ) ;

	vertex_descriptor c = graph.addVertex( Coordinate(0.0,0.0) ) ;
	vertex_descriptor d = graph.addVertex( Coordinate(1.0,1.0) ) ;

	graph.addEdge( a, b );
	graph.addEdge( c, d );
	BOOST_CHECK( ! algorithm::isConnected( graph ) );
	graph.addEdge( c, b );
	BOOST_CHECK( algorithm::isConnected( graph ) );
}


BOOST_AUTO_TEST_SUITE_END()




