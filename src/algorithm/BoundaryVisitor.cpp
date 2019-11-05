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

#include <SFCGAL/algorithm/BoundaryVisitor.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>

#include <map>
#include <complex>
#include <SFCGAL/detail/ComplexComparator.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
void BoundaryVisitor::visit( const Point& )
{
    _boundary.reset();
}

///
///
///
void BoundaryVisitor::visit( const LineString& g )
{
    if ( g.isEmpty() ) {
        _boundary.reset();
        return ;
    }

    if ( g.startPoint().coordinate() == g.endPoint().coordinate() ) {
        _boundary.reset() ;
    }
    else {
        std::unique_ptr< MultiPoint > boundary( new MultiPoint );
        boundary->addGeometry( g.startPoint() );
        boundary->addGeometry( g.endPoint() );
        _boundary.reset( boundary.release() );
    }
}

///
///
///
void BoundaryVisitor::visit( const Polygon& g )
{
    if ( g.isEmpty() ) {
        _boundary.reset();
        return ;
    }

    if ( ! g.hasInteriorRings() ) {
        _boundary.reset( g.exteriorRing().clone() );
    }
    else {
        std::unique_ptr< MultiLineString > boundary( new MultiLineString );

        for ( size_t i = 0; i < g.numRings(); i++ ) {
            boundary->addGeometry( g.ringN( i ) );
        }

        _boundary.reset( boundary.release() );
    }
}

///
///
///
void BoundaryVisitor::visit( const Triangle& g )
{
    if ( g.isEmpty() ) {
        _boundary.reset();
        return ;
    }

    std::unique_ptr< LineString > boundary( new LineString );

    for ( size_t i = 0; i < 4; i++ ) {
        boundary->addPoint( g.vertex( i ) );
    }

    _boundary.reset( boundary.release() );
}

///
///
///
void BoundaryVisitor::visit( const Solid& g )
{
    BOOST_THROW_EXCEPTION( Exception(
                               ( boost::format( "unsupported type %1% in boundary operation" ) % g.geometryType() ).str()
                           ) );
}

///
///
///
void BoundaryVisitor::visit( const MultiPoint& )
{
    _boundary.reset();
}

///
///
///
void BoundaryVisitor::visit( const MultiLineString& g )
{
    if ( g.isEmpty() ) {
        _boundary.reset();
        return ;
    }

    /*
     * create a GeometryGraph and rely on vertex degree (1 means boundary)
     */
    graph::GeometryGraph        graph ;
    graph::GeometryGraphBuilder graphBuilder( graph ) ;

    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        graphBuilder.addLineString( g.lineStringN( i ) );
    }

    getBoundaryFromLineStrings( graph ) ;
}

///
///
///
void BoundaryVisitor::visit( const MultiPolygon& g )
{
    graph::GeometryGraph        graph ;
    graph::GeometryGraphBuilder graphBuilder( graph ) ;

    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        graphBuilder.addPolygon( g.polygonN( i ) );
    }

    getBoundaryFromPolygons( graph ) ;
}

///
///
///
void BoundaryVisitor::visit( const MultiSolid& g )
{
    BOOST_THROW_EXCEPTION( Exception(
                               ( boost::format( "unsupported type %1% in boundary operation" ) % g.geometryType() ).str()
                           ) );
}

///
///
///
void BoundaryVisitor::visit( const GeometryCollection& g )
{
    BOOST_THROW_EXCEPTION( Exception(
                               ( boost::format( "unsupported type %1% in boundary operation" ) % g.geometryType() ).str()
                           ) );
}

///
///
///
void BoundaryVisitor::visit( const PolyhedralSurface& g )
{
    graph::GeometryGraph        graph ;
    graph::GeometryGraphBuilder graphBuilder( graph ) ;

    graphBuilder.addPolyhedralSurface( g );
    getBoundaryFromPolygons( graph ) ;
}

///
///
///
void BoundaryVisitor::visit( const TriangulatedSurface& g )
{
    graph::GeometryGraph        graph ;
    graph::GeometryGraphBuilder graphBuilder( graph ) ;

    graphBuilder.addTriangulatedSurface( g );
    getBoundaryFromPolygons( graph ) ;
}


///
///
///
Geometry* BoundaryVisitor::releaseBoundary()
{
    if ( _boundary.get() ) {
        return _boundary.release() ;
    }
    else {
        return new GeometryCollection() ;
    }
}


///
///
///
void BoundaryVisitor::getBoundaryFromLineStrings( const graph::GeometryGraph& graph )
{
    typedef graph::GeometryGraph::vertex_descriptor vertex_descriptor ;
    typedef graph::GeometryGraph::vertex_iterator   vertex_iterator ;


    std::vector< vertex_descriptor > vertices ;

    vertex_iterator it,end ;

    for ( boost::tie( it,end ) = graph.vertices(); it != end; ++it ) {
        vertex_descriptor vertex = *it ;

        if ( graph.degree( vertex ) == 1 ) {
            vertices.push_back( vertex );
        }
    }

    if ( vertices.empty() ) {
        _boundary.reset() ;
    }
    else if ( vertices.size() == 1 ) {
        _boundary.reset( new Point( graph[ vertices[0] ].coordinate ) );
    }
    else {
        std::unique_ptr< MultiPoint > boundary( new MultiPoint );

        for ( size_t i = 0; i < vertices.size(); i++ ) {
            boundary->addGeometry( new Point( graph[ vertices[i] ].coordinate ) ) ;
        }

        _boundary.reset( boundary.release() );
    }
}


///
///
///
void BoundaryVisitor::getBoundaryFromPolygons( const graph::GeometryGraph& g )
{
    typedef graph::GeometryGraph::vertex_descriptor vertex_descriptor ;
    //typedef graph::GeometryGraph::vertex_iterator   vertex_iterator ;
    typedef graph::GeometryGraph::edge_descriptor   edge_descriptor ;
    typedef graph::GeometryGraph::edge_iterator     edge_iterator ;


    std::vector< edge_descriptor > boundaryEdges ;

    edge_iterator it, end ;

    for ( boost::tie( it, end ) = g.edges(); it != end ; ++it ) {
        if ( g.edges( g.source( *it ), g.target( *it ) ).size() == 1U ) {
            boundaryEdges.push_back( *it ) ;
        }
    }

    if ( boundaryEdges.empty() ) {
        _boundary.reset() ;
    }
    else {
        //TODO merge Line Segments into LineString
        std::unique_ptr< MultiLineString > boundary( new MultiLineString );

        for ( size_t i = 0; i < boundaryEdges.size(); i++ ) {
            const edge_descriptor& edge = boundaryEdges[i] ;

            vertex_descriptor source = g.source( edge );
            vertex_descriptor target = g.target( edge );

            boundary->addGeometry(
                new LineString(
                    Point( g[ source ].coordinate ),
                    Point( g[ target ].coordinate )
                )
            ) ;
        }

        _boundary.reset( boundary.release() );
    }
}





}//algorithm
}//SFCGAL

