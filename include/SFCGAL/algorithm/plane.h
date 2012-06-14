#ifndef _SFCGAL_ALGORITHM_PLANE_H_
#define _SFCGAL_ALGORITHM_PLANE_H_


#include <SFCGAL/Polygon.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Returns the 3D normal to a polygon (supposed to be planar).
	 */
	template < typename Kernel >
	CGAL::Plane_3< Kernel > plane3D( const Polygon & polygon )
	{
		Point const & pa = polygon.exteriorRing().pointN(0) ;
		Point const & pb = polygon.exteriorRing().pointN(1) ;
		Point const & pc = polygon.exteriorRing().pointN(2) ;

		bool is3D = polygon.is3D() ;

		CGAL::Point_3< Kernel > a( pa.x(), pa.y(), is3D ? pa.z() : 0.0 );
		CGAL::Point_3< Kernel > b( pb.x(), pb.y(), is3D ? pb.z() : 0.0 );
		CGAL::Point_3< Kernel > c( pc.x(), pc.y(), is3D ? pc.z() : 0.0 );

		// ba ^ bc
		return CGAL::Plane_3< Kernel >(a,b,c) ;
	}

}//algorithm
}//SFCGAL


#endif
