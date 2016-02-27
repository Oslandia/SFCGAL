#ifndef _SFCGAL_MULTIPOINT_H_
#define _SFCGAL_MULTIPOINT_H_

#include <SFCGAL/Point.h>
#include <SFCGAL/Collection.h>

namespace SFCGAL {
        
    template < typename K >
    using MultiPoint = Collection< Point<K> > ;

} // SFCGAL

#endif

