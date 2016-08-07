/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SFCGAL_GRAPH_ISCONNECTED_H_
#define _SFCGAL_GRAPH_ISCONNECTED_H_

#include <SFCGAL/Geometry.h>
#include <SFCGAL/Coordinate.h>
#include <SFCGAL/Validity.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/noncopyable.hpp>
#include <map>

namespace SFCGAL {
namespace algorithm {

/**
 * Represents a polyhedral surface as a graph where faces are nodes and egde are graph edges
 * @pre the polygons are valid
 * @todo unittest
 * @ingroup detail
 */

class SFCGAL_API SurfaceGraph : boost::noncopyable {
public:
    typedef size_t VertexIndex;
    typedef size_t FaceIndex;
    typedef std::map< Coordinate, VertexIndex >  CoordinateMap ;
    enum integrals { INVALID_INDEX = size_t( -1 ) } ;
    // an edge is inserted with vtx ordered by the first polygon we treat,
    // we search the edge with reverse ordered vtx indexes.
    // as a result, an inconsistent orientation between polygons can be spotted by
    // finding the edge in the same order
    // note that this situation may be caused if a face is duplicated
    typedef std::map< std::pair < VertexIndex, VertexIndex > , std::pair< FaceIndex, FaceIndex > >  EdgeMap ;
    typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS > FaceGraph;
    /*
     * Construct from PolyHedralSurface
     * @throw Exception if surface is not connected
     */
    SurfaceGraph( const PolyhedralSurface& s );

    /*
     * Construct from TriangulatedSurface
     * @throw Exception if surface is not connected
     */
    SurfaceGraph( const TriangulatedSurface& tin ) ;

    const EdgeMap& edgeMap() const {
        return _edgeMap ;
    }
    const FaceGraph& faceGraph() const {
        return _graph ;
    }
    //const CoordinateMap & coordMap() const { return _coordinateMap ; }
    const Validity isValid() const {
        return _isValid ;
    }

private:
    CoordinateMap _coordinateMap ;
    EdgeMap _edgeMap ;
    FaceGraph _graph ;
    VertexIndex _numVertices ;

    Validity _isValid ;

    void addRing( const LineString& ring, FaceIndex faceIndex ); // helper for ctor
};

/**
 * test if a surface is connected, the graph should be build beforehand
 * @ingroup detail
 */
SFCGAL_API bool isConnected( const SurfaceGraph& graph );

/**
 * test if a surface is closed, the graph should be build beforehand
 * @note the surface may not be connected, eg. two spheres will yield a true result
 * @ingroup detail
 */
SFCGAL_API bool isClosed( const SurfaceGraph& graph );

}
}
#endif
