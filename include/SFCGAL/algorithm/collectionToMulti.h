#ifndef SFCGAL_COLLECTION_TO_MULTI_ALGORITHM
#define SFCGAL_COLLECTION_TO_MULTI_ALGORITHM

#include <SFCGAL/Geometry.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {
    namespace algorithm {
	    /*
	     * Given a geometry collection of triangles, TINs and polygons
	     * returns a MultiPolygon
	     *
	     * @warning Ownership is taken from the parameter
	     */
	    std::auto_ptr<Geometry> collectionToMulti( std::auto_ptr<Geometry> coll );
    }
}

#endif
