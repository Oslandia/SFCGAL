#ifndef SFCGAL_COLLECT_ALGORITHM
#define SFCGAL_COLLECT_ALGORITHM

#include <SFCGAL/Geometry.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {
    namespace algorithm {
	/*
	 * Returns an aggregate of ga and gb
	 */
	std::auto_ptr<Geometry> collect( const Geometry& ga, const Geometry& gb );

	/*
	 * Returns an aggregate of a list of geometries
	 */
	template <typename GeometryIterator>
	std::auto_ptr<Geometry> collect( GeometryIterator begin, GeometryIterator end )
	{
	    GeometryIterator it;
	    // FIXME: optimize type. For instance, if all the given geometries are points, return a MultiPoint instead of a GeometryCollection
	    GeometryCollection coll;
	    for ( it = begin; it != end; ++it ) {
		coll.addGeometry( *it );
	    }
	    return std::auto_ptr<Geometry>(coll);
	}
    }
}

#endif
