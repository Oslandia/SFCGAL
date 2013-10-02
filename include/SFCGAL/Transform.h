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

#ifndef _SFCGAL_TRANSFORM_H_
#define _SFCGAL_TRANSFORM_H_

#include <SFCGAL/Geometry.h>
#include <SFCGAL/GeometryVisitor.h>


namespace SFCGAL {

/**
 * Represents a coordinate transform
 */
class SFCGAL_API Transform : public GeometryVisitor {
public:
    virtual ~Transform() ;
    /**
     * apply transform to a geometry
     */
    virtual void transform( Point& p ) = 0 ;

    virtual void visit( Point& g ) ;
    virtual void visit( LineString& g ) ;
    virtual void visit( Polygon& g ) ;
    virtual void visit( Triangle& g ) ;
    virtual void visit( Solid& g ) ;
    virtual void visit( MultiPoint& g );
    virtual void visit( MultiLineString& g ) ;
    virtual void visit( MultiPolygon& g );
    virtual void visit( MultiSolid& g );
    virtual void visit( GeometryCollection& g ) ;
    virtual void visit( PolyhedralSurface& g ) ;
    virtual void visit( TriangulatedSurface& g ) ;
};


}//SFCGAL


#endif
