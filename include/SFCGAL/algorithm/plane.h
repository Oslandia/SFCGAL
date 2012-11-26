#ifndef _SFCGAL_ALGORITHM_PLANE_H_
#define _SFCGAL_ALGORITHM_PLANE_H_

#include <boost/format.hpp>

//#include <SFCGAL/ublas.h>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Polygon.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * Test if a 3D plane can be exatracted from a Polygon
	 */
	template < typename Kernel >
	bool hasPlane3D( const Polygon& polygon,
			 CGAL::Point_3< Kernel > & a,
			 CGAL::Point_3< Kernel > & b,
			 CGAL::Point_3< Kernel > & c )
	{
		typedef CGAL::Point_3< Kernel > Point_3 ;

		const LineString & exteriorRing = polygon.exteriorRing() ;

		/*
		 * look for 3 non collinear points
		 */
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
		return n == 3;
	}

	/**
	 * Test if a 3D plane can be exatracted from a Polygon
	 */
	template < typename Kernel >
	bool hasPlane3D( const Polygon& polygon )
	{
		// temporary arguments
		CGAL::Point_3< Kernel > a, b, c;
		return hasPlane3D( polygon, a, b, c );
	}

	/**
	 * Get 3 non collinear points from a Polygon
	 */
	template < typename Kernel >
	void plane3D(
		const Polygon & polygon,
		CGAL::Point_3< Kernel > & a,
		CGAL::Point_3< Kernel > & b,
		CGAL::Point_3< Kernel > & c
	)
	{
		if ( ! hasPlane3D( polygon, a, b, c) ){
			BOOST_THROW_EXCEPTION(
				Exception(
					( boost::format("can't find plane for Polygon '%1%'") % polygon.asText() ).str()
				)
			);
		}
	}

	/**
	 * Returns the 3D normal to a polygon (supposed to be planar).
	 */
	template < typename Kernel >
	CGAL::Plane_3< Kernel > plane3D( const Polygon & polygon )
	{
		typedef CGAL::Point_3< Kernel > Point_3 ;

		/*
		 * look for 3 non collinear points
		 */
		Point_3 a,b,c;
		plane3D(polygon,a,b,c);
		return CGAL::Plane_3< Kernel >(a,b,c) ;
	}




}//algorithm
}//SFCGAL


#endif
