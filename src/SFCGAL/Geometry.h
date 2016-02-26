#ifndef _SFCGAL_GEOMETRY_H_
#define _SFCGAL_GEOMETRY_H_

#include <vector>
#include <boost/variant.hpp>

#include <SFCGAL/kernels.h>

namespace SFCGAL {
    
    /**
     * A 3D point
     */
    template < typename K > 
    using Point = CGAL::Point_3< K > ;
    
    /**
     * A 3D LineString
     */
    template < typename K > 
    using LineString = std::vector< Point< K > > ;
    
    /**
     * A 3D Polygon with holes
     * @warning this is a strange GIS object in 3D
     */
    template < typename K > 
    using Polygon = std::vector< LineString< K > > ;
    
    /**
     * A 3D Triangle
     */
    template < typename K > 
    using Triangle = CGAL::Triangle_3<K> ;
    
    /**
     * A geometry is variant.
     * 
     * @warning An empty geometry is a empty GeometryCollection
     */
    template < typename K >
    using Geometry = boost::make_recursive_variant<
        Point<K>,                              // Point
        LineString<K>,                         // LineString
        Polygon<K>,                            // Polygon
        Triangle<K>,                           // Triangle
        std::vector<boost::recursive_variant_> // GeometryCollection
    > ;
    
    /**
     * A GeometryCollection
     */
    template < typename K >
    using GeometryCollection = std::vector< Geometry<K> > ;
    

} // SFCGAL

#endif
