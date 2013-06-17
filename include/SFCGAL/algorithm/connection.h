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
#ifndef _SFCGAL_GRAPH_ISCONNECTED_H_
#define _SFCGAL_GRAPH_ISCONNECTED_H_

#include <SFCGAL/all.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <map>
#include <limits>

namespace SFCGAL {
namespace algorithm {

/**
 * Represents a polyhedral surface as a graph where faces are nodes and egde are graph edges
 * @pre the polygons are valid
 */

class PolyHedralSurfaceGraph {
public:
    typedef size_t VertexIndex;
    typedef size_t FaceIndex;
    typedef std::map< Coordinate, VertexIndex >  CoordinateMap ;
    static const size_t INVALID_INDEX = size_t(-1) ; // would use std::numeric_limits< size_t >::max() if it were constant, or SIZE_MAX if it were easier to find.
    // an edge is inserted with vtx ordered by the first polygon we treat, 
    // we search the edge with reverse ordered vtx indexes.
    // as a result, an inconsistent orientation between polygons can be spotted by
    // finding the edge in the same order
    // note that this situation may be caused if a face is duplicated
    typedef std::map< std::pair < VertexIndex, VertexIndex > , std::pair< FaceIndex, FaceIndex > >  EdgeMap ;
    typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS > FaceGraph;
    /*
     * Construct from PolyHedralSurfaceGraph
     * @throw Exception if surface is not connected
     */
    PolyHedralSurfaceGraph( const PolyhedralSurface & s );

    /*
     * Construct from TriangulatedSurface
     * @throw Exception if surface is not connected
     */
    PolyHedralSurfaceGraph( const TriangulatedSurface & tin ) ;

    const EdgeMap & edgeMap() const { return _edgeMap ; }
    const FaceGraph & faceGraph() const { return _graph ; }
    const CoordinateMap & coordMap() const {return _coordinateMap ; }


private:
    CoordinateMap _coordinateMap ;
    EdgeMap _edgeMap ;
    FaceGraph _graph ;

};

inline
PolyHedralSurfaceGraph::PolyHedralSurfaceGraph( const PolyhedralSurface & surf )
{
    VertexIndex numVertices = 0 ;
    const size_t numPolygons = surf.numPolygons() ;
    for ( size_t p = 0; p != numPolygons; ++p ) { // for each polygon
        const Polygon & polygon = surf.polygonN(p) ;
        const size_t numRings = polygon.numRings() ;
        for ( size_t r = 0; r != numRings; ++r ){ // for each ring
            const LineString & ring = polygon.ringN(r) ;
            const size_t numSegments = ring.numSegments() ;
            for ( size_t s = 0; s != numSegments; ++s ){ // for each segment
                const Coordinate startCoord = ring.pointN( s ).coordinate() ;
                const Coordinate endCoord = ring.pointN( ( s + 1 ) % numSegments ).coordinate() ; // possible optimization: store the index of ring start point instead of finding it
                const CoordinateMap::const_iterator startFound = _coordinateMap.find( startCoord ) ;
                const CoordinateMap::const_iterator endFound = _coordinateMap.find( endCoord ) ;
                BOOST_ASSERT_MSG( s + 1 != numSegments || endFound != _coordinateMap.end(), "ring not closed" );
                if ( startFound != _coordinateMap.end() && endFound != _coordinateMap.end() ) {
                    // found both end, we look for the edge
                    const VertexIndex startIndex = startFound->second ;
                    const VertexIndex endIndex = endFound->second ;
                    const std::pair< VertexIndex, VertexIndex > edge( startIndex, endIndex );
                    const EdgeMap::const_iterator foundEdgeWithBadOrientation = _edgeMap.find( edge ); 
                    if ( foundEdgeWithBadOrientation != _edgeMap.end() ) {
                        BOOST_THROW_EXCEPTION( Exception (
                            ( boost::format( "inconsistant orientation of PolyhedralSurface detected at edge %d of ring %d of polygon %d") % s % r % p ).str() 
                        ));
                    }
                    const std::pair< VertexIndex, VertexIndex > reversedEdge( endIndex, startIndex );
                    const EdgeMap::iterator foundEdge = _edgeMap.find( reversedEdge ); 
                    if ( foundEdge != _edgeMap.end() ) {
                        // edit edge
                        foundEdge->second.second = p;
                        // we have two faces connected, this is an edge of the graph
                        boost::add_edge( foundEdge->second.first, foundEdge->second.second, _graph );
                    }
                    else {
                        // create edge
                        _edgeMap.insert( std::make_pair( edge, std::make_pair( p, INVALID_INDEX ) ) );
                    }
                }
                else {
                    // one end at least is missing, create the edge
                    VertexIndex startIndex ;
                    if ( startFound == _coordinateMap.end() ) {
                        _coordinateMap.insert( std::make_pair( startCoord, numVertices) ) ;
                        startIndex = numVertices;
                        ++numVertices ;
                    }
                    else startIndex = startFound->second;

                    VertexIndex endIndex ;
                    if ( endFound == _coordinateMap.end() ) {
                        _coordinateMap.insert( std::make_pair( endCoord, numVertices) ) ;
                        endIndex = numVertices;
                        ++numVertices ;
                    }
                    else endIndex = endFound-> second;

                    const std::pair< VertexIndex, VertexIndex > edge( startIndex, endIndex );
                    _edgeMap.insert( std::make_pair( edge, std::make_pair( p, INVALID_INDEX ) ) );
                }
            }
        }
    }
}

PolyHedralSurfaceGraph::PolyHedralSurfaceGraph( const TriangulatedSurface & tin )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
}

/**
 * test if a surface is connected, the graph should be build beforehand
 */
inline
bool isConnected( const PolyHedralSurfaceGraph graph )
{
    std::vector< PolyHedralSurfaceGraph::FaceIndex > component( graph.coordMap().size() );
    const size_t numComponents = boost::connected_components( graph.faceGraph(), &component[0] );
    return 1 == numComponents;
}

/**
 * test if a surface is closed, the graph should be build beforehand
 * @note the surface may not be connected, eg. two spheres will yield a true result
 */
inline
bool isClosed( const PolyHedralSurfaceGraph graph )
{
    const PolyHedralSurfaceGraph::EdgeMap::const_iterator end = graph.edgeMap().end() ;
    for ( PolyHedralSurfaceGraph::EdgeMap::const_iterator e= graph.edgeMap().begin(); e != end; ++e ) {
        if ( e->second.second == PolyHedralSurfaceGraph::INVALID_INDEX ) return false;
    }
    return true;
}

}
}
#endif
