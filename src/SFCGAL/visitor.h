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

#ifndef _SFCGAL_VISITOR_H_
#define _SFCGAL_VISITOR_H_

#include <SFCGAL/types.h>

namespace SFCGAL {
    
    /**
     * Static visitor on geometries
     */
    template < typename GT, typename Visitor >
    typename Visitor::result_type apply_visitor(
        Visitor & visitor,
        const GT & g
    ){
        using K = typename GT::Kernel ;

        switch(g.geometryTypeId()){
        case TYPE_POINT:
            return visitor( static_cast< const Point<K>& >(g) );
        case TYPE_LINESTRING:
            return visitor( static_cast< const LineString<K>& >(g) );
        case TYPE_POLYGON:
            return visitor( static_cast< const Polygon<K>& >(g) );
        case TYPE_MULTIPOINT:
            return visitor( static_cast< const MultiPoint<K>& >(g) );
        case TYPE_MULTILINESTRING:
            return visitor( static_cast< const MultiLineString<K>& >(g) );
        case TYPE_MULTIPOLYGON:
            return visitor( static_cast< const MultiPolygon<K>& >(g) );
        case TYPE_GEOMETRYCOLLECTION:
            return visitor( static_cast< const GeometryCollection<K>& >(g) );
        case TYPE_POLYHEDRALSURFACE:
            return visitor( static_cast< const PolyhedralSurface<K>& >(g) );
        case TYPE_TRIANGULATEDSURFACE:
            return visitor( static_cast< const TriangulatedSurface<K>& >(g) );
        case TYPE_TRIANGLE:
            return visitor( static_cast< const Triangle<K>& >(g) );
        case TYPE_SOLID:
            return visitor( static_cast< const Solid<K>& >(g) );
        case TYPE_MULTISOLID:
            return visitor( static_cast< const MultiSolid<K>& >(g) );
        }
        BOOST_ASSERT(false); // missing case in visitor!
    }
    
}//SFCGAL

#endif
