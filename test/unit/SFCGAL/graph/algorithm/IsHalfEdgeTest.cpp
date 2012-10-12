#include <boost/test/unit_test.hpp>

#include <SFCGAL/graph/GeometryGraph.h>
#include <SFCGAL/graph/algorithm/isHalfEdge.h>

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

	vertex_descriptor a = graph.addVertex( Coordinate(0.0,0.0) ) ;
	vertex_descriptor b = graph.addVertex( Coordinate(1.0,0.0) ) ;
	vertex_descriptor c = graph.addVertex( Coordinate(1.0,1.0) ) ;
	vertex_descriptor d = graph.addVertex( Coordinate(0.0,1.0) ) ;

	vertex_descriptor e = graph.addVertex( Coordinate(2.0,0.0) ) ;
	vertex_descriptor f = graph.addVertex( Coordinate(2.0,1.0) ) ;

	vertex_descriptor g = graph.addVertex( Coordinate(3.0,0.0) ) ;
	vertex_descriptor h = graph.addVertex( Coordinate(3.0,1.0) ) ;


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




