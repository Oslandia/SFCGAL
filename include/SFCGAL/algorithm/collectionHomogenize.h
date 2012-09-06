#ifndef SFCGAL_COLLECTION_HOMOGENIZE_ALGORITHM
#define SFCGAL_COLLECTION_HOMOGENIZE_ALGORITHM

#include <memory>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {
    namespace algorithm {
	    /*
	     * Given a geometry collection, returns the "simplest" representation of the contents.
	     * Singletons will be returned as singletons.
	     * Collections that are homogeneous will be returned as the appropriate multi-type. 
	     *
	     * @warning Ownership is taken from the parameter
	     */
	    std::auto_ptr<Geometry> collectionHomogenize( std::auto_ptr<Geometry> coll );
    }
}

#endif
