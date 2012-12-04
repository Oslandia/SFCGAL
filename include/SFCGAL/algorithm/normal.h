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
		const CGAL::Point_3< Kernel > & a,
		const CGAL::Point_3< Kernel > & b,
		const CGAL::Point_3< Kernel > & c
	)
	{
		// bc ^ ba
		return CGAL::cross_product( c - b, a - b ) ;
	}


	/**
	 * Returns the 3D normal to a ring (supposed to be planar and closed).
	 */
	template < typename Kernel >
	CGAL::Vector_3< Kernel > normal3D( const LineString & ls )
	{
		// Newell's formula
		CGAL::Vector_3< Kernel > normal;
		for ( size_t i = 0; i < ls.numPoints(); ++i )
		{
			const Point& pi = ls.pointN(i);
			const Point& pj = ls.pointN( (i+1) % ls.numPoints() );
			normal[0] += ( pi.y() - pj.y() ) * ( pi.z() + pj.z() );
			normal[1] += ( pi.z() - pj.z() ) * ( pi.x() + pj.x() );
			normal[2] += ( pi.x() - pj.x() ) * ( pi.y() + pj.y() );
		}
		return normal;
	}

	/**
	 * Returns the 3D normal to a polygon (supposed to be planar).
	 */
	template < typename Kernel >
	CGAL::Vector_3< Kernel > normal3D( const Polygon & polygon )
	{
		return normal3D< Kernel >( polygon.exteriorRing() );
	}

}//algorithm
}//SFCGAL


#endif
