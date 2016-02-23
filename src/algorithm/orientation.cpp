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

#include <SFCGAL/algorithm/orientation.h>

#include <SFCGAL/algorithm/ConsistentOrientationBuilder.h>

#include <SFCGAL/detail/graph/GeometryGraph.h>
#include <SFCGAL/detail/graph/GeometryGraphBuilder.h>
#include <SFCGAL/detail/graph/algorithm/isHalfEdge.h>

#include <SFCGAL/detail/graph/algorithm/orientation.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
void makeValidOrientation( CGAL::Polygon_2< Kernel >& polygon )
{
    if ( polygon.orientation() != CGAL::COUNTERCLOCKWISE ) {
        polygon.reverse_orientation() ;
    }
}

///
///
///
void makeValidOrientation( CGAL::Polygon_with_holes_2< Kernel >& polygon )
{
    typedef CGAL::Polygon_with_holes_2< Kernel > Polygon_with_holes_2 ;

    if ( polygon.outer_boundary().orientation() != CGAL::COUNTERCLOCKWISE ) {
        polygon.outer_boundary().reverse_orientation() ;
    }

    for ( Polygon_with_holes_2::Hole_iterator it = polygon.holes_begin(); it != polygon.holes_end(); ++it ) {
        if ( it->orientation() != CGAL::CLOCKWISE ) {
            it->reverse_orientation() ;
        }
    }
}

///
///
///
void makeValidOrientation( Polygon& polygon )
{
    for ( size_t i = 0; i < polygon.numRings(); i++ ) {
        LineString& ring = polygon.ringN( i );

        if ( i == 0 ) {
            if ( ring.toPolygon_2().orientation() != CGAL::COUNTERCLOCKWISE ) {
                ring.reverse() ;
            }
        }
        else {
            if ( ring.toPolygon_2().orientation() != CGAL::CLOCKWISE ) {
                ring.reverse();
            }
        }
    }
}

///
///
///
bool hasConsistentOrientation3D( const TriangulatedSurface& g )
{
    using namespace graph ;

    if ( g.isEmpty() ) {
        return true ;
    }


    GeometryGraph graph ;
    GeometryGraphBuilder graphBuilder( graph ) ;
    graphBuilder.addTriangulatedSurface( g );
    return graph::algorithm::isHalfEdge( graph ) ;
}


///
///
///
bool hasConsistentOrientation3D( const PolyhedralSurface& g )
{
    using namespace graph ;

    if ( g.isEmpty() ) {
        return true ;
    }

    GeometryGraph graph ;
    GeometryGraphBuilder graphBuilder( graph ) ;
    graphBuilder.addPolyhedralSurface( g );
    return graph::algorithm::isHalfEdge( graph ) ;
}


///
///
///
void makeConsistentOrientation3D( TriangulatedSurface& g )
{
    ConsistentOrientationBuilder builder ;
    builder.addTriangulatedSurface( g );
    g = builder.buildTriangulatedSurface() ;
}

///
///
///
bool isCounterClockWiseOriented( const LineString& ls )
{
    // Compute the 'z' part of the Newell's formula
    // and test against 0
    Kernel::FT z = 0 ;

    for ( size_t i = 0; i < ls.numSegments(); ++i ) {
        Kernel::Point_2 pi = ls.pointN( i ).toPoint_2<Kernel>();
        Kernel::Point_2 pj = ls.pointN( i+1 ).toPoint_2<Kernel>();
        z += ( pi.x() - pj.x() ) * ( pi.y() + pj.y() );
    }

    return z > 0;
}

///
///
///
bool isCounterClockWiseOriented( const Triangle& tri )
{
    // Compute the 'z' part of the cross product
    Kernel::Point_2 v0 = tri.vertex( 0 ).toPoint_2<Kernel>() ;
    Kernel::Point_2 v1 = tri.vertex( 1 ).toPoint_2<Kernel>() ;
    Kernel::Point_2 v2 = tri.vertex( 2 ).toPoint_2<Kernel>() ;
    
    return ( v2.x() - v1.x() ) * ( v0.y() - v1.y() ) -
           ( v2.y() - v1.y() ) * ( v0.x() - v1.x() ) > 0;
}

///
///
///
bool isCounterClockWiseOriented( const Polygon& poly )
{
    return isCounterClockWiseOriented( poly.exteriorRing() );
}

}//algorithm
}//SFCGAL

