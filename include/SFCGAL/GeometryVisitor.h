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
#ifndef _SFCGAL_GEOMETRYVISITOR_H_
#define _SFCGAL_GEOMETRYVISITOR_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {

/**
 * GeometryVisitor
 */
class SFCGAL_API GeometryVisitor {
public:
    virtual ~GeometryVisitor() ;

    /**
     * apply visitor
     */
    virtual void visit( Geometry& g ) ;

    virtual void visit( Point& g ) = 0 ;
    virtual void visit( LineString& g ) = 0 ;
    virtual void visit( Polygon& g ) = 0 ;
    virtual void visit( Triangle& g ) = 0 ;
    virtual void visit( Solid& g ) = 0 ;
    virtual void visit( MultiPoint& g ) = 0 ;
    virtual void visit( MultiLineString& g ) = 0 ;
    virtual void visit( MultiPolygon& g ) = 0 ;
    virtual void visit( MultiSolid& g ) = 0 ;
    virtual void visit( GeometryCollection& g ) = 0 ;
    virtual void visit( PolyhedralSurface& g ) = 0 ;
    virtual void visit( TriangulatedSurface& g ) = 0 ;
};


/**
 * Visitor for const geometries
 */
class SFCGAL_API ConstGeometryVisitor {
public:
    virtual ~ConstGeometryVisitor() ;

    /**
     * apply visitor
     */
    virtual void visit( const Geometry& g ) ;

    virtual void visit( const Point& g ) = 0 ;
    virtual void visit( const LineString& g ) = 0 ;
    virtual void visit( const Polygon& g ) = 0 ;
    virtual void visit( const Triangle& g ) = 0 ;
    virtual void visit( const Solid& g ) = 0 ;
    virtual void visit( const MultiPoint& g ) = 0 ;
    virtual void visit( const MultiLineString& g ) = 0 ;
    virtual void visit( const MultiPolygon& g ) = 0 ;
    virtual void visit( const MultiSolid& g ) = 0 ;
    virtual void visit( const GeometryCollection& g ) = 0 ;
    virtual void visit( const PolyhedralSurface& g ) = 0 ;
    virtual void visit( const TriangulatedSurface& g ) = 0 ;
};


}//SFCGAL


#endif
