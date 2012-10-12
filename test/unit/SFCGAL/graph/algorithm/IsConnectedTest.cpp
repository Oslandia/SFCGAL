#include <boost/test/unit_test.hpp>

#include <SFCGAL/graph/GeometryGraph.h>
#include <SFCGAL/graph/algorithm/isConnected.h>

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




