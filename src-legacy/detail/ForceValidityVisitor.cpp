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

#include <SFCGAL/detail/ForceValidityVisitor.h>

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

namespace SFCGAL {
namespace detail {

///
///
///
void ForceValidityVisitor::visit( Point& g )
{
    g.forceValidityFlag( valid_ );
}

///
///
///
void ForceValidityVisitor::visit( LineString& g )
{
    g.forceValidityFlag( valid_ );
    for ( size_t i = 0; i < g.numPoints(); i++ ) {
        visit( g.pointN( i ) );
    }
}

///
///
///
void ForceValidityVisitor::visit( Polygon& g )
{
    g.forceValidityFlag( valid_ );
    for ( size_t i = 0; i < g.numRings(); i++ ) {
        visit( g.ringN( i ) );
    }
}

///
///
///
void ForceValidityVisitor::visit( Triangle& g )
{
    g.forceValidityFlag( valid_ );
    visit( g.vertex( 0 ) );
    visit( g.vertex( 1 ) );
    visit( g.vertex( 2 ) );
}

///
///
///
void ForceValidityVisitor::visit( Solid& g )
{
    g.forceValidityFlag( valid_ );
    for ( size_t i = 0; i < g.numShells(); i++ ) {
        visit( g.shellN( i ) );
    }
}

///
///
///
void ForceValidityVisitor::visit( MultiPoint& g )
{
    g.forceValidityFlag( valid_ );
    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        visit( g.pointN( i ) );
    }
}

///
///
///
void ForceValidityVisitor::visit( MultiLineString& g )
{
    g.forceValidityFlag( valid_ );
    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        visit( g.lineStringN( i ) );
    }
}

///
///
///
void ForceValidityVisitor::visit( MultiPolygon& g )
{
    g.forceValidityFlag( valid_ );
    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        visit( g.polygonN( i ) );
    }
}

///
///
///
void ForceValidityVisitor::visit( MultiSolid& g )
{
    g.forceValidityFlag( valid_ );
    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        visit( g.solidN( i ) );
    }
}

///
///
///
void ForceValidityVisitor::visit( GeometryCollection& g )
{
    g.forceValidityFlag( valid_ );
    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        g.geometryN( i ).accept( *this );
    }
}

///
///
///
void ForceValidityVisitor::visit( PolyhedralSurface& g )
{
    g.forceValidityFlag( valid_ );
    for ( size_t i = 0; i < g.numPolygons(); i++ ) {
        visit( g.polygonN( i ) );
    }
}

///
///
///
void ForceValidityVisitor::visit( TriangulatedSurface& g )
{
    g.forceValidityFlag( valid_ );
    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        visit( g.geometryN( i ) );
    }
}



}//detail
}//SFCGAL
