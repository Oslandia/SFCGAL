#ifndef _SFCGAL_ALGORITHM_PLANE_H_
#define _SFCGAL_ALGORITHM_PLANE_H_

#include <boost/format.hpp>

//#include <SFCGAL/ublas.h>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Polygon.h>




namespace SFCGAL {
namespace algorithm {

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


	/**
	 * Returns an afine transform to a 3D plane defined by the polygon
	 */
	template < typename Kernel >
	CGAL::Aff_transformation_3< Kernel > affineTransformToPlane3D( const Polygon & polygon )
	{
		if ( ! polygon.is3D() ){
			return CGAL::Aff_transformation_3< Kernel >();
		}

		typedef CGAL::Point_3< Kernel > Point_3 ;

		Point_3 a,b,c;
		plane3D(polygon,a,b,c);

		CGAL::Vector_3< Kernel > ba = a - b ;
		ba = ba / CGAL::sqrt( ba.squared_length() );

		CGAL::Vector_3< Kernel > uy = c - b ;
		uy = uy / CGAL::sqrt( uy.squared_length() ) ;

		CGAL::Vector_3< Kernel > uz = CGAL::cross_product( uy, ba ) ;
		CGAL::Vector_3< Kernel > ux = CGAL::cross_product( uy, uz ) ;

		CGAL::Aff_transformation_3< Kernel > transform(
			ux.x(), ux.y(), ux.z(), -b.x(),
			uy.x(), uy.y(), uy.z(), -b.y(),
			uz.x(), uz.y(), uz.z(), -b.z()
		) ;
		return transform ;
	}


}//algorithm
}//SFCGAL


#endif
