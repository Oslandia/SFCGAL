#include <SFCGAL/algorithm/collect.h>
#include <SFCGAL/all.h>

namespace SFCGAL {
namespace algorithm
{
	std::auto_ptr<Geometry> collect( const Geometry& ga, const Geometry& gb )
	{
		if ( ga.geometryTypeId() == gb.geometryTypeId() ) {
			if ( ga.geometryTypeId() == TYPE_POINT ) {
				MultiPoint *mp = new MultiPoint;
				mp->addGeometry( ga );
				mp->addGeometry( gb );
				return std::auto_ptr<Geometry>(mp);
			}
			else if ( ga.geometryTypeId() == TYPE_LINESTRING ) {
				MultiLineString *mls = new MultiLineString();
				mls->addGeometry(ga);
				mls->addGeometry(gb);
				return std::auto_ptr<Geometry>( mls );
			}
			else if ( ga.geometryTypeId() == TYPE_POLYGON ) {
				MultiPolygon *mp = new MultiPolygon();
				mp->addGeometry(ga);
				mp->addGeometry(gb);
				return std::auto_ptr<Geometry>( mp );
			}
			else if ( ga.geometryTypeId() == TYPE_SOLID ) {
				MultiSolid *mp = new MultiSolid();
				mp->addGeometry(ga);
				mp->addGeometry(gb);
				return std::auto_ptr<Geometry>( mp );
			}
		}

		// else
		GeometryCollection* coll = new GeometryCollection();
		coll->addGeometry(ga);
		coll->addGeometry(gb);
		return std::auto_ptr<Geometry>( coll );
	}
}
}
