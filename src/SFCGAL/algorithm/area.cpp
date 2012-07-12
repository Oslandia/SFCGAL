#include <SFCGAL/algorithm/area.h>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

/*
#include <gmp.h>
#include <mpfr.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
*/

#include <CGAL/Point_3.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Polygon_2.h>

#include <SFCGAL/algorithm/detail/area.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel ;
//typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt Kernel ;

namespace SFCGAL {
namespace algorithm {

///
///
///
double area2D( const Geometry & g )
{
	switch ( g.geometryTypeId() ){
	case TYPE_POLYGON:
		return detail::area< Kernel >( g.as< Polygon >() );
	case TYPE_MULTIPOLYGON:
	case TYPE_POLYHEDRALSURFACE:
	case TYPE_TIN:
	case TYPE_TRIANGLE:
	case TYPE_GEOMETRYCOLLECTION:
	case TYPE_POINT:
	case TYPE_LINESTRING:
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_SOLID:
		BOOST_THROW_EXCEPTION(
			Exception(
				( boost::format("can't compute area2D for %1% type") % g.geometryType() ).str()
			)
		);
	}
	BOOST_THROW_EXCEPTION( Exception("missing case in SFCGAL::algorithm::area2D") );
}

///
///
///
double area3D( const Geometry & g )
{
	switch ( g.geometryTypeId() ){
	case TYPE_POLYGON:
		return detail::area3D< Kernel >( g.as< Polygon >() );
	case TYPE_MULTIPOLYGON:
		return detail::area3D< Kernel >( g.as< MultiPolygon >() );
	case TYPE_POLYHEDRALSURFACE:
		return detail::area3D< Kernel >( g.as< PolyhedralSurface >() );
	case TYPE_TIN:
		return detail::area3D< Kernel >( g.as< TriangulatedSurface >() );
	case TYPE_TRIANGLE:
		return detail::area3D< Kernel >( g.as< Triangle >() );
	case TYPE_GEOMETRYCOLLECTION:
		//TODOsqrt
	case TYPE_POINT:
	case TYPE_LINESTRING:
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_SOLID:
		BOOST_THROW_EXCEPTION(
			Exception(
				( boost::format("can't compute area3D for %1% type") % g.geometryType() ).str()
			)
		);
	}
	BOOST_THROW_EXCEPTION( Exception("missing case in SFCGAL::algorithm::area3D") );
}


}//algorithm
}//SFCGAL


