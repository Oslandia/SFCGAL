#include <SFCGAL/algorithm/collectionHomogenize.h>

namespace SFCGAL {
namespace algorithm {

	// Use of auto_ptr :
	// If nothing has to be built, g will be moved to the result without copy and new allocation.
	// Otherwise, a new geometry is built and the old one is deleted
	std::auto_ptr<Geometry> collectionHomogenize( std::auto_ptr<Geometry> g )
	{
		// unknown type
		int common_type = 0;

		if ( ! g->is<GeometryCollection>() ) {
			// not a collection, nothing to do
			return g;
		}

		const GeometryCollection& coll = g->as<GeometryCollection>();

		// test if it is a singleton
		if ( coll.numGeometries() == 1 ) {
			return std::auto_ptr<Geometry>( coll.geometryN(0).clone() );
		}

		for ( size_t i = 0; i < coll.numGeometries(); ++i ) {
			const Geometry& gi = coll.geometryN(i);
			if ( common_type == 0 ) {
				common_type = gi.geometryTypeId();
				continue;
			}
			if ( gi.geometryTypeId() != common_type ) {
				common_type = 0;
				break;
			}
		}

		if ( common_type == 0 ) {
			// not an homogeneous collection, give back
			return g;
		}

		GeometryCollection* ret_geo = 0;
		if ( common_type == TYPE_POINT ) {
			ret_geo = new MultiPoint;
		}
		else if ( common_type == TYPE_LINESTRING ) {
			ret_geo = new MultiLineString;
		}
		else if ( common_type == TYPE_POLYGON ) {
			ret_geo = new MultiPolygon;
		}
		else if ( common_type == TYPE_SOLID ) {
			ret_geo = new MultiSolid;
		}

		// copy each geometry
		for ( size_t i = 0; i < coll.numGeometries(); ++i ) {
			ret_geo->addGeometry( coll.geometryN(i) );
		}
		return std::auto_ptr<Geometry>( ret_geo );
	}
		
}
}
