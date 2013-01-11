#include <SFCGAL/algorithm/covers.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/all.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

namespace SFCGAL {
namespace algorithm
{
	bool covers( const Geometry& ga, const Geometry& gb )
	{
		// deal with geometry collection
		// call intersects on each geometry of the collection
		const GeometryCollection* coll;
		if ( (coll = dynamic_cast<const GeometryCollection*>( &ga )) ) {
			for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
				if ( covers( coll->geometryN( i ), gb ) ) {
					return true;
				}
			}
			return false;
		}
		if ( (coll = dynamic_cast<const GeometryCollection*>( &gb )) ) {
			for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
				if ( covers( ga, coll->geometryN( i ) ) ) {
					return true;
				}
			}
			return false;
		}

		// default behaviour: calls intersects (NOT VALID !)
		return intersects( ga, gb );
	}
}
}
