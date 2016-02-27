#ifndef _SFCGAL_MULTILINESTRING_H_
#define _SFCGAL_MULTILINESTRING_H_

#include <SFCGAL/Collection.h>
#include <SFCGAL/LineString.h>

namespace SFCGAL {
        
    template < typename K >
    using MultiLineString = Collection< LineString<K> > ;

} // SFCGAL

#endif

