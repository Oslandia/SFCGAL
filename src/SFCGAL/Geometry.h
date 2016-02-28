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
#ifndef _SFCGAL_GEOMETRY_H_
#define _SFCGAL_GEOMETRY_H_

#include <vector>
#include <boost/variant.hpp>

#include <SFCGAL/kernels.h>

#include <SFCGAL/CoordinateType.h>
#include <SFCGAL/GeometryType.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Solid.h>

#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/MultiSolid.h>

namespace SFCGAL {
    
    typedef int srid_t ;
    
    /**
     * A geometry is variant.
     * 
     * @warning An empty geometry is a empty GeometryCollection
     */
    template < typename K >
    using Geometry = typename boost::make_recursive_variant<
        // primitive types
        Point<K>,                                // Point
        LineString<K>,                           // LineString
        Polygon<K>,                              // Polygon
        Triangle<K>,                             // Triangle
        Solid<K>,                                // Solid

        // homonegous collections
        MultiPoint<K>,                           // MultiPoint
        MultiLineString<K>,                      // MultiLineString
        MultiPolygon<K>,                         // MultiPolygon        
        TriangulatedSurface<K>,                  // TriangulatedSurface
        MultiSolid<K>,                           // MultiSolid

        // heterogenous GeometryCollection
        Collection< boost::recursive_variant_ >  // GeometryCollection
    >::type ;
    
    /**
     * A collection of geometry with differents types
     */
    template < typename K >
    using GeometryCollection = Collection< Geometry<K> > ;


} // SFCGAL

#endif
