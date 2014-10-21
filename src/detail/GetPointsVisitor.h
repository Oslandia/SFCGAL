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

#ifndef _SFCGAL_DETAIL_GETPOINTSVISITOR_H_
#define _SFCGAL_DETAIL_GETPOINTSVISITOR_H_

#include <SFCGAL/config.h>

#include <vector>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {
namespace detail {

/**
 * Get the list of points from a Geometry
 */
class SFCGAL_API GetPointsVisitor : public ConstGeometryVisitor {
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
public:
    typedef std::vector< const Point* >::const_iterator const_iterator;
    std::vector< const Point* > points ;
};


}//detail
}//SFCGAL


#endif
