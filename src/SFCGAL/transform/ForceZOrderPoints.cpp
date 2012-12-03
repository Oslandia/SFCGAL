#include <SFCGAL/transform/ForceZOrderPoints.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Polygon.h>

namespace SFCGAL {
namespace transform {

	bool pointingUp( const Point& p1, const Point& p2, const Point& p3 )
	{
		// u = p3 - p2
		// v = p1 - p2
		// P = (0, 0, 1) * ( u ^ v ) [vector product]
		// return Pz > 0
		return CGAL::orientation( p1.toPoint_2(), p2.toPoint_2(), p3.toPoint_2() )
			== CGAL::LEFT_TURN;
		//		return (p3.x() - p2.x()) * (p1.y() - p2.y()) - (p3.y() - p2.y()) * (p1.x() - p2.x()) > 0;
	}

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
		if ( ! pointingUp( t.vertex(0), t.vertex(1), t.vertex(2) ) ) {
			// not pointing up, reverse
			t.reverse();
		}
		Transform::visit( t );
	}
	
}

void ForceZOrderPoints::visit( Polygon& p )
{
	std::cout << "p.is3D(): " << p.is3D() << std::endl;
	if ( ! p.is3D() ) {
		LineString& ext = p.exteriorRing();
		// FIXME: This is not correct
		// 3 points are not always enough to find the orientation
		if ( !pointingUp( ext.pointN(0), ext.pointN(1), ext.pointN(2) ) ) {
			// exterior ring not pointing up, reverse
			std::cout << "reverse" << std::endl;
			ext.reverse();
		}
		for ( size_t i = 0; i < p.numInteriorRings(); ++i ) {
			LineString inter = p.interiorRingN( i );
			if ( pointingUp( inter.pointN(0), inter.pointN(1), inter.pointN(2) ) ) {
				// interior ring is pointing up, reverse
				inter.reverse();
			}
		}
		Transform::visit( p );
	}
	
}

}//transform
}//SFCGAL



