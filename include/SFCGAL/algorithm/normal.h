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
		typename Kernel::FT nx, ny, nz;
		nx = ny = nz = 0.0;
		for ( size_t i = 0; i < ls.numPoints() - 1; ++i )
		{
			const Point& pi = ls.pointN(i);
			const Point& pj = ls.pointN( (i+1) % ls.numPoints() );
			typename Kernel::FT zi = pi.is3D() ? pi.z() : 0.0;
			typename Kernel::FT zj = pj.is3D() ? pj.z() : 0.0;
			nx += ( pi.y() - pj.y() ) * ( zi + zj );
			ny += ( zi - zj ) * ( pi.x() + pj.x() );
			nz += ( pi.x() - pj.x() ) * ( pi.y() + pj.y() );
		}
		return CGAL::Vector_3<Kernel>( nx, ny, nz );
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
