#ifndef SFCGAL_COLLECTION_EXTRACT_ALGORITHM
#define SFCGAL_COLLECTION_EXTRACT_ALGORITHM

#include <SFCGAL/Geometry.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {
    namespace algorithm {
	    /*
	     * Given a geometry collection
	     * returns a MultiPolygon from triangles, polygons, polyhedral and polygons
	     *
	     * @warning Ownership is taken from the parameter
	     */
	    std::auto_ptr<Geometry> collectionExtractPolygons( std::auto_ptr<Geometry> coll );
    }
}

#endif
