#ifndef _SFCGAL_POLYGON_H_
#define _SFCGAL_POLYGON_H_

#include <SFCGAL/kernels.h>

namespace SFCGAL {
        
    /**
     * A 3D Polygon with holes defined as a strong typing on 
     * a vector of polylines
     * @warning this is a strange GIS object in 3D.
     */
    template < typename K > 
    using Polygon = std::vector< LineString< K > > ;

}

#endif

