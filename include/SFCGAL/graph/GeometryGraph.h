#ifndef _SFCGAL_GRAPH_GEOMETRYGRAPH_H_
#define _SFCGAL_GRAPH_GEOMETRYGRAPH_H_

#include <boost/graph/adjacency_list.hpp>

#include <SFCGAL/graph/Vertex.h>
#include <SFCGAL/graph/Edge.h>


namespace SFCGAL {
namespace graph {



	/**
	 *
	 * Represents the vertices and edges for a list of geometries.
	 *
	 * A boost::adjancency_list is wrapped in order to be able to annex some informations
	 * and to provide basic functionalities.
	 *
	 * @warning duplicate matching is performed in GeometryGraphBuilder (allows to modify position once it's done)
	 *
	 */
	template < typename VertexProperties, typename EdgeProperties >
	class GeometryGraphT {
	public:
		typedef VertexProperties                                         vertex_properties ;
		typedef EdgeProperties                                           edge_properties ;

		/**
		 * the wrapped graphEdgeProperties
		 */
		typedef boost::adjacency_list<
			boost::listS, /* stable identifiers */
			boost::listS, /* parallel edges allowed + stable identifiers */
			boost::bidirectionalS,
			vertex_properties,
			edge_properties
		> graph_t ;

		typedef typename boost::graph_traits< graph_t >::vertex_descriptor vertex_descriptor ;
		typedef typename boost::graph_traits< graph_t >::edge_descriptor   edge_descriptor ;

		typedef typename boost::graph_traits< graph_t >::vertex_iterator   vertex_iterator ;
		typedef typename boost::graph_traits< graph_t >::edge_iterator     edge_iterator ;

		/**
		 * add a vertex to the graph
		 * @return the identifier of the vertex
		 */
		vertex_descriptor addVertex( const vertex_properties & properties = vertex_properties() )
		{
			return boost::add_vertex( properties, _graph );
		}
		/**
		 * Remove a vertex (and all its adjacent edges)
		 */
		void             removeVertex( const vertex_descriptor & vertex )
		{
			boost::clear_vertex( vertex );
			boost::remove_vertex( vertex );
		}

		/**
		 * Add an Edge to the Graph
		 * @return the identifier of the vertex
		 */
		edge_descriptor   addEdge(
			const vertex_descriptor & source,
			const vertex_descriptor & target,
			const EdgeProperties & properties = EdgeProperties()
		)
		{
			return boost::add_edge( source, target, properties, _graph ).first;
		}

		/**
		 * get the source vertex for an edge
		 */
		vertex_descriptor source( const edge_descriptor & edge ) {
			return boost::source( edge, _graph ) ;
		}
		/**
		 * get the target vertex for an edge
		 */
		vertex_descriptor target( const edge_descriptor & edge ) {
			return boost::target( edge, _graph ) ;
		}

		/**
		 * Remove an edge
		 */
		void  removeEdge( const edge_descriptor& edge ){
			boost::remove_edge( edge );
		}


		/**
		 * [iterator]return vertex iterator
		 *
		 * @code
		 * typename GeometryGraph<V,E>::vertex_iterator it, end ;
		 * for ( boost::tie( it, end ) = g.vertices(); it != end; ++it ){
		 * 		typename GeometryGraph<V,E>::vertex_descriptor vertex = *it ;
		 * 		std::cout << g[ vertex ].coordinate << std::endl;
		 * }
		 * @endcode
		 */
		inline std::pair< vertex_iterator, vertex_iterator > vertices() const
		{
			return boost::vertices( _graph );
		}
		/**
		 * [iterator]return edge iterator
		 *
		 * @code
		 * typename GeometryGraph<V,E>::edge_iterator it, end ;
		 * for ( boost::tie( it, end ) = g.edges(); it != end; ++it ){
		 * 		typename GeometryGraph<V,E>::edge_descriptor edge = *it ;
		 * 		std::cout << g.source(edge) << "," << g.target(edge) << std::endl;
		 * }
		 * @endcode
		 */
		inline std::pair< edge_iterator, edge_iterator > edges() const
		{
			return boost::edges( _graph );
		}

		/**
		 * returns the VertexProperties attached to a Vertex
		 */
		inline const vertex_properties & operator [] ( const vertex_descriptor & vertex ) const
		{
			return _graph[ vertex ];
		}
		/**
		 * returns the VertexProperties attached to a Vertex
		 */
		inline vertex_properties & operator [] ( const vertex_descriptor & vertex )
		{
			return _graph[ vertex ];
		}


		/**
		 * returns the VertexProperties attached to a Vertex
		 */
		inline const edge_properties & operator [] ( const edge_descriptor & edge ) const
		{
			return _graph[ edge ];
		}
		/**
		 * returns the VertexProperties attached to a Vertex
		 */
		inline edge_properties & operator [] ( const edge_descriptor & edge )
		{
			return _graph[ edge ];
		}

		/**
		 * returns the wrapped boost::graph
		 */
		inline graph_t &       graph() { return _graph ; }
		/**
		 * returns the wrapped boost::graph
		 */
		inline const graph_t & graph() const { return _graph ; }
	private:
		/**
		 * a wrapped boost::graph
		 */
		graph_t _graph ;
	};


	/**
	 * Default GeometryGraph with predefined Vertex and Edge properties for general usage
	 */
	typedef GeometryGraphT< Vertex, Edge > GeometryGraph ;


}//graph
}//SFCGAL


#endif
