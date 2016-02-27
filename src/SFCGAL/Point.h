#ifndef _SFCGAL_POINT_H_
#define _SFCGAL_POINT_H_

#include <SFCGAL/kernels.h>

namespace SFCGAL {
        
    /**
     * SFCGAL Point is simplify defined as a typedef
     *  on a CGAL::Point_3
     */
    template < typename K > 
    using Point = CGAL::Point_3< K > ;

}

#endif
