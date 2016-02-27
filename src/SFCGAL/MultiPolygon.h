#ifndef _SFCGAL_MULTIPOLYGON_H_
#define _SFCGAL_MULTIPOLYGON_H_

#include <SFCGAL/Collection.h>
#include <SFCGAL/Polygon.h>

namespace SFCGAL {
        
    template < typename K >
    using MultiPolygon = Collection< Polygon<K> > ;

} // SFCGAL

#endif

