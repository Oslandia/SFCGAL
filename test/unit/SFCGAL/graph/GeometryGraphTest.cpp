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
}



BOOST_AUTO_TEST_SUITE_END()




