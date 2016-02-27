#ifndef _SFCGAL_TRIANGLE_H_
#define _SFCGAL_TRIANGLE_H_

#include <SFCGAL/kernels.h>

namespace SFCGAL {

    /**
     * A 3D Triangle
     */
    template < typename K > 
    using Triangle = CGAL::Triangle_3<K> ;

}

#endif
