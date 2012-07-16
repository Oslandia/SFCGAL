#ifndef _SFCGAL_ALGORITHM_NORMAL_H_
#define _SFCGAL_ALGORITHM_NORMAL_H_

#include <SFCGAL/Polygon.h>
#include <SFCGAL/algorithm/plane.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Returns the 3D normal to 3 consecutive points.
	 */
	template < typename Kernel >
	CGAL::Vector_3< Kernel > normal3D(
		const CGAL::Point_3< Kernel > & a,
		const CGAL::Point_3< Kernel > & b,
		const CGAL::Point_3< Kernel > & c
	)
	{
		// bc ^ ba
		return CGAL::cross_product( c - b, a - b ) ;
	}


	/**
	 * Returns the 3D normal to a polygon (supposed to be planar).
	 * @todo use plane
	 */
	template < typename Kernel >
	CGAL::Vector_3< Kernel > normal3D( const Polygon & polygon )
	{
		CGAL::Point_3< Kernel > a, b, c ;
		plane3D< Kernel >( polygon, a, b, c );

		// ba ^ bc
		return normal3D(a,b,c) ;
	}

}//algorithm
}//SFCGAL


#endif
