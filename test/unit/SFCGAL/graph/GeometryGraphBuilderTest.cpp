#include <boost/test/unit_test.hpp>


#include <SFCGAL/graph/GeometryGraph.h>
#include <SFCGAL/graph/GeometryGraphBuilder.h>

using namespace boost::unit_test ;

using namespace SFCGAL ;
using namespace SFCGAL::graph ;


BOOST_AUTO_TEST_SUITE( SFCGAL_GeometryGraphBuilderTest )

BOOST_AUTO_TEST_CASE( addPoint )
{
	typedef GeometryGraph::vertex_descriptor vertex_descriptor ;
	typedef GeometryGraph::edge_descriptor   edge_descriptor ;
	GeometryGraph        graph;
	GeometryGraphBuilder graphBuilder( graph );

	vertex_descriptor a = graphBuilder.addPoint( Point(0.0,0.0,0.0) );
	vertex_descriptor b = graphBuilder.addPoint( Point(1.0,1.0,1.0) );
	vertex_descriptor c = graphBuilder.addPoint( Point(2.0,2.0,2.0) );

	//b duplicate
	vertex_descriptor d = graphBuilder.addPoint( Point(1.0,1.0,1.0) );

	BOOST_CHECK_EQUAL( graph.numVertices(), 3U );
	BOOST_CHECK_EQUAL( graph.numEdges(), 0U );
	BOOST_CHECK_EQUAL( b, d );

	BOOST_CHECK( graph[ a ].coordinate == Coordinate(0.0,0.0,0.0) );
	BOOST_CHECK( graph[ b ].coordinate == Coordinate(1.0,1.0,1.0) );
	BOOST_CHECK( graph[ c ].coordinate == Coordinate(2.0,2.0,2.0) );
}



BOOST_AUTO_TEST_CASE( addLineString )
{
	typedef GeometryGraph::vertex_descriptor vertex_descriptor ;
	typedef GeometryGraph::edge_descriptor   edge_descriptor ;
	GeometryGraph        graph;
	GeometryGraphBuilder graphBuilder( graph );

	std::vector< Point > points ;
	points.push_back( Point(0.0,0.0,0.0) );
	points.push_back( Point(1.0,0.0,0.0) );
	points.push_back( Point(1.0,1.0,0.0) );
	points.push_back( Point(0.0,1.0,0.0) );
	points.push_back( Point(0.0,0.0,0.0) );

	LineString lineString( points );
	std::vector< edge_descriptor > sharedLineString = graphBuilder.addLineString( lineString );

	BOOST_CHECK_EQUAL( graph.numVertices(), 4U );
	BOOST_CHECK_EQUAL( graph.numEdges(), 4U );

	//check closed
	for ( size_t i = 0; i < sharedLineString.size(); i++ ){
		BOOST_CHECK( graph.target( sharedLineString[ i ] ) == graph.source( sharedLineString[ (i+1) % 4 ] ) ) ;
	}
}


BOOST_AUTO_TEST_SUITE_END()




