#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;


#include <SFCGAL/graph/GeometryGraph.h>

using namespace SFCGAL ;
using namespace SFCGAL::graph ;


BOOST_AUTO_TEST_SUITE( SFCGAL_GeometryGraphTest )

BOOST_AUTO_TEST_CASE( addVertexAndEdges )
{
	GeometryGraph graph;
	typedef GeometryGraph::vertex_descriptor vertex_descriptor ;
	typedef GeometryGraph::edge_descriptor   edge_descriptor ;

	vertex_descriptor a = graph.addVertex( Coordinate(0.0,0.0) );
	vertex_descriptor b = graph.addVertex( Coordinate(1.0,1.0) );

	edge_descriptor ab = graph.addEdge( a, b );
	BOOST_CHECK_EQUAL( graph.source(ab), a );
	BOOST_CHECK_EQUAL( graph.target(ab), b );

	BOOST_CHECK_EQUAL( graph.numVertices(), 2U );
	BOOST_CHECK_EQUAL( graph.numEdges(), 1U );
}



BOOST_AUTO_TEST_CASE( adjacentVertices )
{
	GeometryGraph graph;
	typedef GeometryGraph::vertex_descriptor vertex_descriptor ;
	typedef GeometryGraph::edge_descriptor   edge_descriptor ;

	vertex_descriptor a = graph.addVertex( Coordinate(0.0,0.0) );
	vertex_descriptor b = graph.addVertex( Coordinate(1.0,1.0) );
	vertex_descriptor c = graph.addVertex( Coordinate(2.0,2.0) );
	vertex_descriptor d = graph.addVertex( Coordinate(2.0,2.0) );

	graph.addEdge( a, b );
	graph.addEdge( b, c );
	graph.addEdge( c, d );

	std::set< vertex_descriptor > bAjacentVertices = graph.adjacentVertices(b);
	BOOST_CHECK_EQUAL( bAjacentVertices.size(), 2U );
	BOOST_CHECK_EQUAL( bAjacentVertices.count(a), 1U );
	BOOST_CHECK_EQUAL( bAjacentVertices.count(c), 1U );
}



BOOST_AUTO_TEST_CASE( inOutEdges )
{
	GeometryGraph graph;
	typedef GeometryGraph::vertex_descriptor          vertex_descriptor ;
	typedef GeometryGraph::edge_descriptor            edge_descriptor ;
	typedef GeometryGraph::directed_edge_descriptor   directed_edge_descriptor ;

	vertex_descriptor a = graph.addVertex( Coordinate(0.0,0.0) );
	vertex_descriptor b = graph.addVertex( Coordinate(1.0,1.0) );
	vertex_descriptor c = graph.addVertex( Coordinate(2.0,2.0) );
	vertex_descriptor d = graph.addVertex( Coordinate(2.0,2.0) );

	edge_descriptor ab = graph.addEdge( a, b );
	edge_descriptor bc = graph.addEdge( b, c );
	edge_descriptor cd = graph.addEdge( c, d );

	std::vector< edge_descriptor >          inEdges    = graph.inEdges(b);
	std::vector< edge_descriptor >          outEdges   = graph.outEdges(b);
	std::vector< directed_edge_descriptor > inOutEdges = graph.inOutEdges(b);

	BOOST_REQUIRE_EQUAL( inEdges.size(), 1U );
	BOOST_CHECK_EQUAL( inEdges[0], ab );

	BOOST_REQUIRE_EQUAL( outEdges.size(), 1U );
	BOOST_CHECK_EQUAL( outEdges[0], bc );

	BOOST_REQUIRE_EQUAL( inOutEdges.size(), 2U );
	//ab is in inOutEdges
	BOOST_CHECK( ( inOutEdges[0].first == ab && inOutEdges[0].second == REVERSE )
			||  ( inOutEdges[1].first == ab && inOutEdges[1].second == REVERSE ) );
	//bc is in inOutEdges
	BOOST_CHECK( ( inOutEdges[0].first == bc && inOutEdges[0].second == DIRECT )
			||  ( inOutEdges[1].first == bc && inOutEdges[1].second == DIRECT ) );
}




BOOST_AUTO_TEST_SUITE_END()




