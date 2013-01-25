#include <SFCGAL/algorithm/area.h>
#include <SFCGAL/all.h>

#include <SFCGAL/algorithm/plane.h>

#include <CGAL/Point_2.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Polygon_2.h>

#include <CGAL/Point_3.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Plane_3.h>


#include <SFCGAL/Exception.h>
#include <boost/format.hpp>

namespace SFCGAL {
namespace algorithm {

typedef CGAL::Point_2< SFCGAL::Kernel >    Point_2 ;
typedef CGAL::Triangle_2< SFCGAL::Kernel > Triangle_2 ;
typedef CGAL::Polygon_2< SFCGAL::Kernel >  Polygon_2 ;

typedef CGAL::Point_3< SFCGAL::Kernel >    Point_3 ;
typedef CGAL::Triangle_3< SFCGAL::Kernel > Triangle_3 ;
typedef CGAL::Plane_3< SFCGAL::Kernel >    Plane_3 ;


///
///
///
double area( const Geometry & g )
{
	switch ( g.geometryTypeId() ){
	case TYPE_POINT:
	case TYPE_LINESTRING:
		return 0 ;
	case TYPE_POLYGON:
		return area( g.as< Polygon >() ) ;
	case TYPE_TRIANGLE:
		return area( g.as< Triangle >() ) ;
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_MULTIPOLYGON:
	case TYPE_GEOMETRYCOLLECTION:
		return area( g.as< GeometryCollection >() ) ;
	case TYPE_TRIANGULATEDSURFACE:
		return area( g.as< TriangulatedSurface >() ) ;
	case TYPE_POLYHEDRALSURFACE:
		return area( g.as< PolyhedralSurface >() ) ;
	case TYPE_SOLID:
	case TYPE_MULTISOLID:
		return 0 ;
	}
	BOOST_THROW_EXCEPTION( Exception(
		( boost::format( "Unexpected geometry type (%s) in SFCGAL::algorithm::area" ) % g.geometryType() ).str()
	) );
}

///
///
///
Kernel::FT signedArea( const Triangle & g )
{
	Triangle_2 triangle = g.toTriangle_2() ;
	return triangle.area() ;
}

///
///
///
Kernel::FT signedArea( const LineString & g )
{
	return g.toPolygon_2().area() ;
}

///
///
///
double area( const Triangle & g )
{
	return CGAL::to_double( CGAL::abs( signedArea(g) ) ) ;
}

///
///
///
double area( const Polygon & g )
{
	Kernel::RT result = 0.0 ;
	for ( size_t i = 0; i < g.numRings(); i++ ){
		Kernel::FT ringArea = signedArea( g.ringN(i) ) ;
		if ( i == 0 ){
			//exterior ring
			result += CGAL::abs( ringArea );
		}else{
			//interior ring
			result -= CGAL::abs( ringArea );
		}
	}
	return CGAL::to_double( result ) ;
}


///
///
///
double area( const GeometryCollection & g )
{
	double result = 0.0 ;
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		result += area( g.geometryN(i) ) ;
	}
	return result ;
}

///
///
///
double area( const TriangulatedSurface & g )
{
	double result = 0.0 ;
	for ( size_t i = 0; i < g.numTriangles(); i++ ){
		result += area( g.triangleN(i) ) ;
	}
	return result ;
}

///
///
///
double area( const PolyhedralSurface & g )
{
	double result = 0.0 ;
	for ( size_t i = 0; i < g.numPolygons(); i++ ){
		result += area( g.polygonN(i) ) ;
	}
	return result ;
}


///----------------------------------------------------------------------------------
/// -- area3D
///----------------------------------------------------------------------------------


///
///
///
double area3D( const Geometry & g )
{
	switch ( g.geometryTypeId() ){
	case TYPE_POINT:
	case TYPE_LINESTRING:
		return 0 ;
	case TYPE_POLYGON:
		return area3D( g.as< Polygon >() );
	case TYPE_TRIANGLE:
		return area3D( g.as< Triangle >() );

	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_MULTIPOLYGON:
	case TYPE_GEOMETRYCOLLECTION:
		return area3D( g.as< GeometryCollection >() );
	case TYPE_TRIANGULATEDSURFACE:
		return area3D( g.as< TriangulatedSurface >() );
	case TYPE_POLYHEDRALSURFACE:
		return area3D( g.as< PolyhedralSurface >() );
	case TYPE_SOLID:
	case TYPE_MULTISOLID:
		return 0 ;
	}
	BOOST_THROW_EXCEPTION( Exception("missing case in SFCGAL::algorithm::area3D") );
}


///
///
///
double area3D( const Polygon & g )
{
	double result = 0.0 ;

	if ( g.isEmpty() ){
		return result ;
	}

	CGAL::Point_3< Kernel > a, b, c ;
	algorithm::plane3D< Kernel >( g, a, b, c );

	/*
	 * compute polygon basis (CGAL doesn't build an orthonormal basis so that computing
	 * the 2D area in this basis would lead to scale effects)
	 * ux = bc
	 * uz = bc^ba
	 * uy = uz^ux
	 *
	 * Note that the basis is rounded to double (CGAL::sqrt)
	 */
	CGAL::Vector_3< Kernel > ux = c - b ;
	CGAL::Vector_3< Kernel > uz = CGAL::cross_product( ux, a - b ) ;
	ux = ux / CGAL::sqrt( CGAL::to_double(ux.squared_length()) ) ;
	uz = uz / CGAL::sqrt( CGAL::to_double(uz.squared_length()) ) ;
	CGAL::Vector_3< Kernel > uy = CGAL::cross_product( uz, ux );

	/*
	 * compute the area for each ring in the local basis
	 */
	for ( size_t i = 0; i < g.numRings(); i++ ){
		const LineString & ring = g.ringN(i);

		CGAL::Polygon_2< Kernel > projectedPolygon;
		for ( size_t j = 0; j < ring.numPoints() - 1 ; j++ ){
			CGAL::Point_3< Kernel > point = ring.pointN(j).toPoint_3();
			CGAL::Point_2< Kernel > projectedPoint(
				( point - b ) * ux,
				( point - b ) * uy
			);
			projectedPolygon.push_back( projectedPoint );
		}

		if ( i == 0 ){
			//exterior ring
			result += CGAL::to_double( CGAL::abs( projectedPolygon.area() ) ) ;
		}else{
			//interior ring
			result -= CGAL::to_double( CGAL::abs( projectedPolygon.area() ) ) ;
		}
	}

	return result ;
}

///
///
///
double area3D( const Triangle & g )
{
	CGAL::Triangle_3< Kernel > triangle(
		g.vertex(0).toPoint_3(),
		g.vertex(1).toPoint_3(),
		g.vertex(2).toPoint_3()
	);
	return sqrt( CGAL::to_double( triangle.squared_area() ) ) ;
}


///
///
///
double area3D( const GeometryCollection & g )
{
	double result = 0.0 ;
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		result += area3D( g.geometryN(i) ) ;
	}
	return result ;
}

///
///
///
double area3D( const PolyhedralSurface & g )
{
	double area = 0.0 ;
	for ( size_t i = 0; i < g.numPolygons(); i++ ){
		area += area3D( g.polygonN(i) ) ;
	}
	return area ;
}

///
///
///
double area3D( const TriangulatedSurface & g )
{
	double result = 0.0 ;
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		result += area3D( g.geometryN(i) ) ;
	}
	return result ;
}


}//algorithm
}//SFCGAL


