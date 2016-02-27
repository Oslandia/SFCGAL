#ifndef _SFCGAL_LINESTRING_H_
#define _SFCGAL_LINESTRING_H_

#include <SFCGAL/kernels.h>

namespace SFCGAL {
        
    /**
     * SFCGAL Point is simplify defined as a typedef
     *  on a CGAL::Point_3
     */
    template < typename K > 
    using LineString = std::vector< Point<K> > ;

}

#endif
