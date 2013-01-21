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
         * @warning exact allows to avoid double rounding at the end of the computation
	*/
	template < typename Kernel >
	CGAL::Vector_3< Kernel > normal3D( const LineString & ls, bool exact = true )
	{
		// Newell's formula
		typename Kernel::FT nx, ny, nz;
		nx = ny = nz = 0.0;
		for ( size_t i = 0; i < ls.numPoints(); ++i )
		{
			const Point& pi = ls.pointN(i);
			const Point& pj = ls.pointN( (i+1) % ls.numPoints() );
			typename Kernel::FT zi = pi.z() ;
			typename Kernel::FT zj = pj.z() ;
			nx += ( pi.y() - pj.y() ) * ( zi + zj );
			ny += ( zi - zj ) * ( pi.x() + pj.x() );
			nz += ( pi.x() - pj.x() ) * ( pi.y() + pj.y() );
		}
		if ( exact ){
			return CGAL::Vector_3<Kernel>( nx, ny, nz );
		}else{
			return CGAL::Vector_3<Kernel>( CGAL::to_double(nx), CGAL::to_double(ny), CGAL::to_double(nz) );
		}	
	} 

	/**
	 * Returns the 3D normal to a polygon (supposed to be planar).
         * @warning exact allows to avoid double rounding at the end of the computation
	 */
	template < typename Kernel >
	CGAL::Vector_3< Kernel > normal3D( const Polygon & polygon, bool exact = true )
	{
		return normal3D< Kernel >( polygon.exteriorRing(), exact );
	}

}//algorithm
}//SFCGAL


#endif
