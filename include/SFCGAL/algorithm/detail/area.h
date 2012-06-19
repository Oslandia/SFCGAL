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
		//CGAL::Plane_3< Kernel > plane = plane3D< Kernel >(g);

		CGAL::Aff_transformation_3< Kernel > transform = affineTransformToPlane3D< Kernel >( g );

		//std::cout << "plane : " << plane << std::endl;
		//std::cout << "origin : " << plane.point() << std::endl;
		//std::cout << "base1  : " << plane.base1() << std::endl;
		//std::cout << "base2  : " << plane.base2() << std::endl;
		//std::cout << "normal : " << plane.orthogonal_vector() << std::endl;

		typename Kernel::RT area = 0.0 ;

		for ( size_t i = 0; i < g.numRings(); i++ ){
			const LineString & ring = g.ringN(i);

			CGAL::Polygon_2< Kernel > projectedPolygon;
			for ( size_t j = 0; j < ring.numPoints() - 1 ; j++ ){
				CGAL::Point_3< Kernel > proj = ring.pointN(j).toPoint_3< Kernel >();
				proj = proj.transform(transform);
				projectedPolygon.push_back( CGAL::Point_2< Kernel >( proj.x(), proj.y() ) );
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
			g.vertex(0).toPoint_3< Kernel >(),
			g.vertex(1).toPoint_3< Kernel >(),
			g.vertex(2).toPoint_3< Kernel >()
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
		for ( size_t i = 0; i < g.numTriangles(); i++ ){
			area += area3D< Kernel >( g.triangleN(i) ) ;
		}
		return area ;
	}


}//detail
}//algorithm
}//SFCGAL


#endif
