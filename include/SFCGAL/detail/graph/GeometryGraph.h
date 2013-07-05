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
#ifndef _SFCGAL_GRAPH_GEOMETRYGRAPH_H_
#define _SFCGAL_GRAPH_GEOMETRYGRAPH_H_

#include <boost/graph/adjacency_list.hpp>

#include <SFCGAL/detail/graph/Vertex.h>
#include <SFCGAL/detail/graph/Edge.h>


namespace SFCGAL {
namespace graph {

/**
 *
 */
typedef enum {
    DIRECT  = 0,
    REVERSE = 1
} EdgeDirection ;


/**
 * reverse EdgeDirection (DIRECT=>REVERSE, REVERSE=>DIRECT)
 */
inline EdgeDirection reverse( const EdgeDirection& direction )
{
    return ( EdgeDirection )( 1 - direction ) ;
}


/**
 *
 * @brief [private]Represents the vertices and edges for a list of geometries.
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
     * An edge descriptor, with a direction.
     *
     * From the vertex point of view, out edges are DIRECT, in edges are REVERSE.
     */
    typedef std::pair< edge_descriptor, EdgeDirection>                 directed_edge_descriptor ;

    typedef typename boost::graph_traits< graph_t >::in_edge_iterator  in_edge_iterator ;
    typedef typename boost::graph_traits< graph_t >::out_edge_iterator out_edge_iterator ;


    /**
     * [vertex]returns the number of vertices
     */
    inline size_t     numVertices() const {
        return boost::num_vertices( _graph );
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
    inline std::pair< vertex_iterator, vertex_iterator > vertices() const {
        return boost::vertices( _graph );
    }

    /**
     * [vertex]add a vertex to the graph
     * @return the identifier of the vertex
     */
    vertex_descriptor addVertex( const vertex_properties& properties = vertex_properties() ) {
        return boost::add_vertex( properties, _graph );
    }
    /**
     * [vertex]Remove a vertex (and all its adjacent edges)
     */
    void             removeVertex( const vertex_descriptor& vertex ) {
        boost::clear_vertex( vertex );
        boost::remove_vertex( vertex );
    }

    /**
     * [edge]returns the number of vertices
     */
    inline size_t     numEdges() const {
        return boost::num_edges( _graph );
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
    inline std::pair< edge_iterator, edge_iterator > edges() const {
        return boost::edges( _graph );
    }

    /**
     * [edge]Add an Edge to the Graph
     * @return the identifier of the vertex
     */
    edge_descriptor   addEdge(
        const vertex_descriptor& source,
        const vertex_descriptor& target,
        const EdgeProperties& properties = EdgeProperties()
    ) {
        return boost::add_edge( source, target, properties, _graph ).first;
    }

    /**
     * [edge]get the source vertex for an edge
     */
    vertex_descriptor source( const edge_descriptor& edge ) const {
        return boost::source( edge, _graph ) ;
    }
    /**
     * [edge]get the source vertex for an edge
     */
    vertex_descriptor source( const edge_descriptor& edge, const EdgeDirection& direction ) const {
        return direction == DIRECT ? boost::source( edge, _graph ) : boost::target( edge, _graph ) ;
    }
    /**
     * [edge]get the source vertex for an edge with a direction
     */
    vertex_descriptor source( const directed_edge_descriptor& edge ) const {
        return source( edge.first, edge.second ) ;
    }

    /**
     * [edge]get the target vertex for an edge
     */
    vertex_descriptor target( const edge_descriptor& edge ) const {
        return boost::target( edge, _graph ) ;
    }
    /**
     * [edge]get the target vertex for an edge with a direction
     */
    vertex_descriptor target( const edge_descriptor& edge, const EdgeDirection& direction ) const {
        return direction == DIRECT ? boost::target( edge, _graph ) : boost::source( edge, _graph ) ;
    }
    /**
     * [edge]get the target vertex for an edge with a direction
     */
    vertex_descriptor target( const directed_edge_descriptor& edge ) const {
        return target( edge.first, edge.second );
    }
    /**
     * [edge]Remove an edge
     */
    void  removeEdge( const edge_descriptor& edge ) {
        boost::remove_edge( edge, _graph );
    }

    /**
     * [edge]Get edges from a to b and from b to a
     */
    std::vector< directed_edge_descriptor > edges( const vertex_descriptor& a, const vertex_descriptor& b ) const {
        std::vector< directed_edge_descriptor > result ;

        //out_edges from a targeting b
        {
            out_edge_iterator it, end ;

            for ( boost::tie( it,end ) = boost::out_edges( a,_graph ); it != end; ++it ) {
                if ( target( *it ) != b ) {
                    continue ;
                }

                result.push_back( std::make_pair( *it, DIRECT ) );
            }
        }
        //out_edges from b targeting a
        {
            out_edge_iterator it, end ;

            for ( boost::tie( it,end ) = boost::out_edges( b,_graph ); it != end; ++it ) {
                if ( target( *it ) != a ) {
                    continue ;
                }

                result.push_back( std::make_pair( *it, REVERSE ) );
            }
        }
        return result ;
    }

    /**
     * returns the degree of a vertex
     */
    inline size_t degree( const vertex_descriptor& vertex ) const {
        return boost::degree( vertex,_graph );
    }


    /**
     * [adjacency]get in edges
     */
    std::vector< edge_descriptor > inEdges( const vertex_descriptor& vertex ) {
        std::vector< edge_descriptor > edges ;

        in_edge_iterator it, end ;

        for ( boost::tie( it, end ) = boost::in_edges( vertex, _graph ); it != end; ++it ) {
            edges.push_back( *it );
        }

        return edges ;
    }
    /**
     * [adjacency]get out edges
     */
    std::vector< edge_descriptor > outEdges( const vertex_descriptor& vertex ) const {
        std::vector< edge_descriptor > edges ;

        out_edge_iterator it, end ;

        for ( boost::tie( it, end ) = boost::out_edges( vertex, _graph ); it != end; ++it ) {
            edges.push_back( *it );
        }

        return edges ;
    }
    /**
     * [adjacency]get in/out edges
     */
    std::vector< directed_edge_descriptor > inOutEdges( const vertex_descriptor& vertex ) const {
        std::vector< directed_edge_descriptor > edges ;
        {
            in_edge_iterator it, end ;

            for ( boost::tie( it, end ) = boost::in_edges( vertex, _graph ); it != end; ++it ) {
                edges.push_back( std::make_pair( *it, REVERSE ) );
            }
        }

        //out edges
        {
            out_edge_iterator it, end ;

            for ( boost::tie( it, end ) = boost::out_edges( vertex, _graph ); it != end; ++it ) {
                edges.push_back( std::make_pair( *it, DIRECT ) );
            }
        }
        return edges ;
    }


    /**
     * [adjacency]Returns the list of the adjacent vertices using both DIRECT and REVERSE direction
     * @param vertex input vertex
     * @param withReverseDirection indicates if in_edges are used
     */
    std::set< vertex_descriptor > adjacentVertices( const vertex_descriptor& vertex, bool withReverseDirection = true ) {
        std::set< vertex_descriptor > vertices ;
        // out edges
        {
            out_edge_iterator it, end ;

            for ( boost::tie( it, end ) = boost::out_edges( vertex, _graph ); it != end; ++it ) {
                vertex_descriptor reached = target( *it ) ;

                if ( reached != vertex ) {
                    vertices.insert( reached );
                }
            }
        }

        // in edges
        if ( withReverseDirection ) {
            in_edge_iterator it, end ;

            for ( boost::tie( it, end ) = boost::in_edges( vertex, _graph ); it != end; ++it ) {
                vertex_descriptor reached = source( *it ) ;

                if ( reached != vertex ) {
                    vertices.insert( reached );
                }
            }
        }

        return vertices ;
    }


    /**
     * [helper]indicates if edges are opposite
     */
    inline bool areOpposite( const edge_descriptor& a, const edge_descriptor& b ) const {
        return source( a ) == target( b ) && target( a ) == source( b ) ;
    }
    /**
     * [helper]indicates if edges are opposite
     */
    inline bool areParallel( const edge_descriptor& a, const edge_descriptor& b ) const {
        return source( a ) == source( b ) && target( a ) == target( b ) ;
    }


    /**
     * [EdgeString]revert the order of a list of edges. Old edges are removed from the graph, new
     * ones are created.
     *
     * @warning properties are kept but oriented one (left face, right face, etc.) are lost.
     */
    void reverse( std::vector< edge_descriptor > & edges ) {
        std::vector< edge_descriptor > result ;

        for ( typename std::vector< edge_descriptor >::reverse_iterator it = edges.rbegin();
                it != edges.rend(); ++it ) {
            edge_descriptor newEdge = addEdge( target( *it ), source( *it ), ( *this )[*it] );
            result.push_back( newEdge );
            removeEdge( *it );
        }

        edges = result ;
    }

    /**
     * returns the VertexProperties attached to a Vertex
     */
    inline const vertex_properties& operator [] ( const vertex_descriptor& vertex ) const {
        return _graph[ vertex ];
    }
    /**
     * returns the VertexProperties attached to a Vertex
     */
    inline vertex_properties& operator [] ( const vertex_descriptor& vertex ) {
        return _graph[ vertex ];
    }


    /**
     * returns the VertexProperties attached to a Vertex
     */
    inline const edge_properties& operator [] ( const edge_descriptor& edge ) const {
        return _graph[ edge ];
    }
    /**
     * returns the VertexProperties attached to a Vertex
     */
    inline edge_properties& operator [] ( const edge_descriptor& edge ) {
        return _graph[ edge ];
    }

    /**
     * returns the wrapped boost::graph
     */
    inline graph_t&        graph() {
        return _graph ;
    }
    /**
     * returns the wrapped boost::graph
     */
    inline const graph_t& graph() const {
        return _graph ;
    }


    /**
     * implicit cast to the wrapped boost graph in order to keep boost graph interface
     */
    operator graph_t& ( void ) {
        return _graph;
    }
    /**
     * implicit cast to the wrapped boost graph in order to keep boost graph interface
     */
    operator const graph_t& ( void ) const {
        return _graph;
    }
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
