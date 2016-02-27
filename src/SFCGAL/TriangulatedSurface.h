#ifndef _SFCGAL_TRIANGULATEDSSURFACE_H_
#define _SFCGAL_TRIANGULATEDSSURFACE_H_

#include <SFCGAL/Triangle.h>
#include <SFCGAL/Collection.h>

namespace SFCGAL {
        
    template < typename K >
    using TriangulatedSurface = Collection< Triangle<K> > ;

} // SFCGAL

#endif

