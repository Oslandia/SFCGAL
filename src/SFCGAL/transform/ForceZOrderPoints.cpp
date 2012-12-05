#include <SFCGAL/transform/ForceZOrderPoints.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/algorithm/orientation.h>

namespace SFCGAL {
namespace transform {

///
///
///
ForceZOrderPoints::ForceZOrderPoints( const double & defaultZ ):
	_defaultZ(defaultZ)
{

}

///
///
///
void ForceZOrderPoints::transform( Point & p )
{
	if ( ! p.is3D() ) {
		p = Point( p.x(), p.y(), _defaultZ );
	}
}


///
///
///
void ForceZOrderPoints::visit( Triangle & t )
{
	if ( ! t.is3D() ) {
		if ( ! algorithm::isCounterClockWiseOriented( t ) ) {
			// not pointing up, reverse
			t.reverse();
		}
		Transform::visit( t );
	}
	
}

void ForceZOrderPoints::visit( Polygon& p )
{
	if ( ! p.is3D() ) {
		LineString& ext = p.exteriorRing();
		if ( ! algorithm::isCounterClockWiseOriented( p.exteriorRing() ) ) {
			// exterior ring not pointing up, reverse
			ext.reverse();
		}
		for ( size_t i = 0; i < p.numInteriorRings(); ++i ) {
			LineString inter = p.interiorRingN( i );
			if ( algorithm::isCounterClockWiseOriented( inter ) ) {
				// interior ring is pointing up, reverse
				inter.reverse();
			}
		}
		Transform::visit( p );
	}
	
}

}//transform
}//SFCGAL



