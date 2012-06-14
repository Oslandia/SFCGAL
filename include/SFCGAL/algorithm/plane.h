#ifndef _SFCGAL_ALGORITHM_PLANE_H_
#define _SFCGAL_ALGORITHM_PLANE_H_

#include <boost/format.hpp>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Polygon.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Returns the 3D normal to a polygon (supposed to be planar).
	 */
	template < typename Kernel >
	CGAL::Plane_3< Kernel > plane3D( const Polygon & polygon )
	{
		typedef CGAL::Point_3< Kernel > Point_3 ;

		const LineString & exteriorRing = polygon.exteriorRing() ;

		/*
		 * look for 3 non collinear points
		 */
		Point_3 a,b,c;
		size_t  n = 0 ;
		for ( size_t i = 0; i < exteriorRing.numPoints(); i++ ){
			Point_3 p = exteriorRing.pointN(i).toPoint_3< Kernel >() ;
			if ( n == 0 ){
				a = p ;
				n++ ;
			}else if ( n == 1 && a != p ){
				b = p ;
				n++ ;
			}else if ( n == 2 && ! CGAL::collinear( a, b, p ) ) {
				c = p ;
				n++ ;
				break;
			}
		}

		/*
		 * check
		 */
		if ( n != 3 ){
			BOOST_THROW_EXCEPTION(
				Exception(
					( boost::format("can't find plane for Polygon '%1%'") % polygon.asText() ).str()
				)
			);
		}

		return CGAL::Plane_3< Kernel >(a,b,c) ;
	}

}//algorithm
}//SFCGAL


#endif
