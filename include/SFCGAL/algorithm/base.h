#ifndef _SFCGAL_ALGORITHM_BASE_H_
#define _SFCGAL_ALGORITHM_BASE_H_

#include <SFCGAL/Polygon.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Returns a 3D base containing a,b,c points in OXY. b is the origin
	 *
	 * @warning The polygon is supposed to be planar.
	 *
	 * @param polygon the input polygon
	 * @param origin the origin of the new base
	 * @param
	 */
	template < typename Kernel >
	void base3D(
		const CGAL::Vector_3< Kernel > & a,
		const CGAL::Vector_3< Kernel > & b,
		const CGAL::Vector_3< Kernel > & c,
		CGAL::Vector_3< Kernel > & ux,
		CGAL::Vector_3< Kernel > & uy,
		CGAL::Vector_3< Kernel > & uz
	)
	{
		/*
		 * ux = bc = c - b
		 * uz = bc ^ ba = ( c - b ) ^ ( a - b )
		 * uy = uz ^ ux
		 */

		//bc
		ux     = c - b ;
		//bc^ba
		uz = CGAL::cross_product( ux, a - b ) ;
		//( bc ^ ba )^bc
		uy = CGAL::cross_product( uz, ux );
	}



	/**
	 * Returns the 3D base containing the polygon in OXY plan
	 *
	 * @warning The polygon is supposed to be planar.
	 *
	 * @param polygon the input polygon
	 * @param origin the origin of the new base
	 * @param
	 */
	template < typename Kernel >
	void base3D(
		const Polygon & polygon,
		CGAL::Vector_3< Kernel > & origin,
		CGAL::Vector_3< Kernel > & ux,
		CGAL::Vector_3< Kernel > & uy,
		CGAL::Vector_3< Kernel > & uz
	)
	{
		Point const & pa = polygon.exteriorRing().pointN(0) ;
		Point const & pb = polygon.exteriorRing().pointN(1) ;
		Point const & pc = polygon.exteriorRing().pointN(2) ;

		bool is3D = polygon.is3D() ;

		CGAL::Vector_3< Kernel > a( pa.x(), pa.y(), is3D ? pa.z() : 0.0 );
		CGAL::Vector_3< Kernel > b( pb.x(), pb.y(), is3D ? pb.z() : 0.0 );
		CGAL::Vector_3< Kernel > c( pc.x(), pc.y(), is3D ? pc.z() : 0.0 );

		origin = b ;
		base3D(a,b,c,ux,uy,uz);
	}

}//algorithm
}//SFCGAL


#endif
