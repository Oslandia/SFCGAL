#ifndef _SFCGAL_GRAPH_GEOMETRYGRAPHBUILDER_H_
#define _SFCGAL_GRAPH_GEOMETRYGRAPHBUILDER_H_

#include <SFCGAL/graph/GeometryGraph.h>

namespace SFCGAL {
namespace graph {

	/**
	 * Convert Geometries to a GeometryGraph. Identifier in the Graph are returned in order to
	 * allow the user to keep identify the geometry.
	 */
	template < typename V, typename E >
	class GeometryGraphBuilder {
	public:
		typedef GeometryGraph< V, E >                                    graph_t ;
		typedef typename GeometryGraph< V, E >::vertex_descriptor        vertex_descriptor ;
		typedef typename GeometryGraph< V, E >::edge_descriptor          edge_descriptor ;

		/**
		 * allows to match duplicates
		 */
		typedef std::map< Coordinate, vertex_descriptor >  coordinate_list ;

		/**
		 * default constructor
		 */
		GeometryGraphBuilder( graph_t & graph ):
			_graph(graph)
		{

		}

		/**
		 * destructor
		 */
		~GeometryGraphBuilder()
		{

		}


		/**
		 * add a Point to the Graph
		 */
		vertex_descriptor addPoint( const Point & point ) {
			BOOST_ASSERT( ! point.isEmpty() );

			coordinate_list::const_iterator it = _vertices.find( point.coordinate() ) ;
			if ( it != _vertices.end() ){
				return it->second ;
			}else{
				vertex_descriptor vertex = _graph.addVertex( V( point.coordinate() ) );
				_vertices.insert( std::make_pair( point.coordinate(), vertex ) );
				return vertex ;
			}
		}

		/**
		 * add a Point to the Graph
		 * @return the edge inserted into the graph
		 */
		edge_descriptor   addLineSegment( const Point & a, const Point & b ) {
			BOOST_ASSERT( ! a.isEmpty() );
			BOOST_ASSERT( ! b.isEmpty() );

			return _graph.addEdge(
				addPoint(a),
				addPoint(b)
			);
		}

		/**
		 * add a LineString to the graph
		 * @return the list of edges inserted into the graph
		 */
		std::vector< edge_descriptor > addLineString( const LineString & lineString )
		{
			BOOST_ASSERT( ! lineString.isEmpty() );

			std::vector< edge_descriptor > edges ;
			for ( size_t i = 0; i < lineString.numPoints() - 1; i++ ){
				edges.push_back( addLineSegment( lineString.pointN(i), lineString.pointN(i+1) ) );
			}
			return edges ;
		}

		/**
		 * add a Polygon to the graph
		 * @returns the list of rings inserted into the graph
		 */
		std::vector< std::vector< edge_descriptor > >   addPolygon( const Polygon & polygon )
		{
			BOOST_ASSERT( ! polygon.isEmpty() );

			std::vector< std::vector< edge_descriptor > > rings ;
			for ( size_t i = 0; i < rings.numRings(); i++ ){
				edges.push_back( addLineString( polygon.ringN(i) ) );
			}
			return rings ;
		}


	private:
		graph_t &         _graph ;
		coordinate_list   _vertices ;

		/**
		 * no copy constructor
		 */
		GeometryGraphBuilder( const GeometryGraphBuilder & other ) ;
	};


}//topology
}//SFCGAL


#endif
