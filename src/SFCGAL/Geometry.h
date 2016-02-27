#ifndef _SFCGAL_GEOMETRY_H_
#define _SFCGAL_GEOMETRY_H_

#include <vector>
#include <boost/variant.hpp>

#include <SFCGAL/kernels.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>

#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/TriangulatedSurface.h>

namespace SFCGAL {
    
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

        // homonegous collections
        MultiPoint<K>,                           // MultiPoint
        MultiLineString<K>,                      // MultiLineString
        MultiPolygon<K>,                         // MultiPolygon        
        TriangulatedSurface<K>,                  // TriangulatedSurface

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
