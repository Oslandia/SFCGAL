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

#ifndef _SFCGAL_ALGORITHM_BOUNDARYVISITOR_H_
#define _SFCGAL_ALGORITHM_BOUNDARYVISITOR_H_

#include <SFCGAL/GeometryVisitor.h>

#include <SFCGAL/detail/graph/GeometryGraph.h>
#include <SFCGAL/detail/graph/GeometryGraphBuilder.h>


namespace SFCGAL {
namespace algorithm {


/**
 * Compute the boundary for a Geometry
 *
 * boundary( Point )      : GEOMETRYCOLLECTION EMPTY
 * boundary( LineString ) : either GEOMETRYCOLLECTION EMPTY is the LineString is closed, or MULTIPOINT(2)
 * boundary( Polygon )    : LINESTRING | MULTILINESTRING (polygon rings)
 * boundary( Triangle )   : either GEOMETRYCOLLECTION EMPTY is the LineString is closed, or MULTIPOINT(2)
 *
 * boundary( MultiPoint )      : GEOMETRYCOLLECTION EMPTY
 * boundary( MultiLineString ) : either GEOMETRYCOLLECTION EMPTY or single occurance points
 *
 * @warning GeometryCollection are not supported in the general case
 *
 * @ŧodo Solid
 *
 * @todo MultiPolygon, PolyhedralSurface, TriangulatedSurface (same graph algorithm, edges without parallel or opposite)
 *
 *
 * @todo GeometryCollection : complex for heterogeneous collection (not supported in GEOS)
 * @todo MultiSolid : faced elimination
 * @ingroup detail
 *
 */
class SFCGAL_API BoundaryVisitor : public ConstGeometryVisitor {
public:

    virtual void visit( const Point& g ) ;
    virtual void visit( const LineString& g ) ;
    virtual void visit( const Polygon& g ) ;
    virtual void visit( const Triangle& g ) ;
    virtual void visit( const Solid& g ) ;
    virtual void visit( const MultiPoint& g ) ;
    virtual void visit( const MultiLineString& g ) ;
    virtual void visit( const MultiPolygon& g ) ;
    virtual void visit( const MultiSolid& g ) ;
    virtual void visit( const GeometryCollection& g ) ;
    virtual void visit( const PolyhedralSurface& g ) ;
    virtual void visit( const TriangulatedSurface& g ) ;

    /**
     * get the boundary
     */
    Geometry* releaseBoundary() ;

protected:

    /**
     * get the boundary vertices for a set of LineString in a GeometryGraph
     */
    void getBoundaryFromLineStrings( const graph::GeometryGraph& g );
    /**
     * get the boundary edges for a set of Polygons in a GeometryGraph
     * @warning not optimal (edges could be counted using complex< vertex_descriptor >)
     * @todo merge resulting edges
     */
    void getBoundaryFromPolygons( const graph::GeometryGraph& g );

private:
    std::auto_ptr< Geometry > _boundary ;
};



}//algorithm
}//SFCGAL


#endif
