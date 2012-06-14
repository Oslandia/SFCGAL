#ifndef _SFCGAL_ALGORITHM_NORMAL_H_
#define _SFCGAL_ALGORITHM_NORMAL_H_


#include <SFCGAL/Polygon.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Returns the 3D normal to 3 consecutive points.
	 */
	template < typename Kernel >
	CGAL::Vector_3< Kernel > normal3D(
		const CGAL::Vector_3< Kernel > & a,
		const CGAL::Vector_3< Kernel > & b,
		const CGAL::Vector_3< Kernel > & c
	)
	{
		// bc ^ ba
		return CGAL::cross_product( c - b, a - b ) ;
	}


	/**
	 * Returns the 3D normal to a polygon (supposed to be planar).
	 */
	template < typename Kernel >
	CGAL::Vector_3< Kernel > normal3D( const Polygon & polygon )
	{
		Point const & pa = polygon.exteriorRing().pointN(0) ;
		Point const & pb = polygon.exteriorRing().pointN(1) ;
		Point const & pc = polygon.exteriorRing().pointN(2) ;

		bool is3D = polygon.is3D() ;

		CGAL::Vector_3< Kernel > a( pa.x(), pa.y(), is3D ? pa.z() : 0.0 );
		CGAL::Vector_3< Kernel > b( pb.x(), pb.y(), is3D ? pb.z() : 0.0 );
		CGAL::Vector_3< Kernel > c( pc.x(), pc.y(), is3D ? pc.z() : 0.0 );

		// ba ^ bc
		return normal3D(a,b,c) ;
	}

}//algorithm
}//SFCGAL


#endif
