#ifndef _SFCGAL_ALGORITHM_DETAIL_AREA_H_
#define _SFCGAL_ALGORITHM_DETAIL_AREA_H_

#include <cmath>

#include <SFCGAL/Polygon.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/PolyhedralSurface.h>

#include <SFCGAL/algorithm/plane.h>


namespace SFCGAL {
namespace algorithm {
namespace detail {

	/**
	 * Returns 2D area for a Triangle
	 */
	template < typename Kernel >
	double area(
		const Triangle & g
	)
	{
		CGAL::Triangle_2< Kernel > triangle(
			g.vertex(0).toPoint_2(),
			g.vertex(1).toPoint_2(),
			g.vertex(2).toPoint_2()
		);
		return CGAL::abs( CGAL::to_double( triangle.area() ) );
	}

	/**
	 * Returns 2D area for a Polygon
	 */
	template < typename Kernel >
	double area(
		const Polygon & g
	)
	{
		typename Kernel::RT area = 0.0 ;

		for ( size_t i = 0; i < g.numRings(); i++ ){
			const LineString & ring = g.ringN(i);

			CGAL::Polygon_2< Kernel > projectedPolygon;
			for ( size_t j = 0; j < ring.numPoints() - 1 ; j++ ){
				projectedPolygon.push_back( ring.pointN(j).toPoint_2() );
			}

			if ( i == 0 ){
				//exterior ring
				area += CGAL::abs( CGAL::to_double(projectedPolygon.area()) );
			}else{
				//interior ring
				area -= CGAL::abs( CGAL::to_double(projectedPolygon.area()) );
			}
		}

		return CGAL::to_double( area ) ;
	}

	/**
	 * Returns the 2D area for a MultiPolygon
	 * @todo kahnan sum?
	 */
	template < typename Kernel >
	double area(
		const MultiPolygon & g
	)
	{
		double area = 0.0 ;
		for ( size_t i = 0; i < g.numGeometries(); i++ ){
			area += detail::area< Kernel >( g.polygonN(i) ) ;
		}
		return area ;
	}

	/**
	 * Returns 3D area for a Polygon
	 */
	template < typename Kernel >
	double area3D(
		const Polygon & g
	)
	{
		/*
		 * compute 2D area in the Polygon plane
		 */
		if ( ! g.is3D() ){
			return area< Kernel >( g );
		}

//		std::cout << "#" << g.asText(1) << std::endl;

		CGAL::Point_3< Kernel > a, b, c ;
//		algorithm::plane3D< Kernel >( g, a, b, c );

		CGAL::Plane_3< Kernel > plane = algorithm::plane3D< Kernel >( g );
		a = plane.point();
		b = plane.point() + plane.base1();
		c = plane.point() + plane.base2();
		// No need to compute a new basis anymore ?

		/*
		 * compute polygon base
		 * ux = bc
		 * uz = bc^ba
		 * uy = uz^ux
		 */
		CGAL::Vector_3< Kernel > ux = c - b ;
		CGAL::Vector_3< Kernel > uz = CGAL::cross_product( ux, a - b ) ;
		ux = ux / CGAL::sqrt( CGAL::to_double(ux.squared_length()) ) ;
		uz = uz / CGAL::sqrt( CGAL::to_double(uz.squared_length()) ) ;
		CGAL::Vector_3< Kernel > uy = CGAL::cross_product( uz, ux );

//		std::cout << "ux : " << ux << std::endl ;
//		std::cout << "uz : " << uz << std::endl ;
//		std::cout << "uy : " << uy << std::endl ;


		typename Kernel::RT area = 0.0 ;

		for ( size_t i = 0; i < g.numRings(); i++ ){
			const LineString & ring = g.ringN(i);

			CGAL::Polygon_2< Kernel > projectedPolygon;
			for ( size_t j = 0; j < ring.numPoints() - 1 ; j++ ){
				CGAL::Point_3< Kernel > point = ring.pointN(j).toPoint_3();
//				std::cout << "point : " << point << std::endl;
				CGAL::Point_2< Kernel > projectedPoint(
					( point - b ) * ux,
					( point - b ) * uy
				);
//				std::cout << "projectedPoint : " << projectedPoint << std::endl;
				projectedPolygon.push_back( projectedPoint );
			}

			if ( i == 0 ){
				//exterior ring
				area += CGAL::abs( projectedPolygon.area() );
			}else{
				//interior ring
				area -= CGAL::abs( projectedPolygon.area() );
			}
		}

		return CGAL::to_double( area ) ;
	}

	/**
	 * Returns the 3D area for a MultiPolygon
	 * @todo kahnan sum?
	 */
	template < typename Kernel >
	double area3D(
		const MultiPolygon & g
	)
	{
		double area = 0.0 ;
		for ( size_t i = 0; i < g.numGeometries(); i++ ){
			area += area3D< Kernel >( g.polygonN(i) ) ;
		}
		return area ;
	}

	/**
	 * Returns the 3D area for a PolyhedralSurface
	 * @todo kahnan sum?
	 */
	template < typename Kernel >
	double area3D(
		const PolyhedralSurface & g
	)
	{
		double area = 0.0 ;
		for ( size_t i = 0; i < g.numPolygons(); i++ ){
			area += area3D< Kernel >( g.polygonN(i) ) ;
		}
		return area ;
	}

	/**
	 * Returns the 3D area for a Triangle
	 */
	template < typename Kernel >
	double area3D(
		const Triangle & g
	)
	{
		CGAL::Triangle_3< Kernel > triangle(
			g.vertex(0).toPoint_3(),
			g.vertex(1).toPoint_3(),
			g.vertex(2).toPoint_3()
		);
		return sqrt( CGAL::to_double( triangle.squared_area() ) ) ;
	}

	/**
	 * Returns the 3D area for a TriangulatedSurface
	 * @todo kahnan sum?
	 */
	template < typename Kernel >
	double area3D(
		const TriangulatedSurface & g
	)
	{
		double area = 0.0 ;
		for ( size_t i = 0; i < g.numGeometries(); i++ ){
			area += area3D< Kernel >( g.geometryN(i) ) ;
		}
		return area ;
	}


}//detail
}//algorithm
}//SFCGAL


#endif
