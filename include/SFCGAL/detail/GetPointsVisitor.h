#ifndef _SFCGAL_DETAIL_GETPOINTSVISITOR_H_
#define _SFCGAL_DETAIL_GETPOINTSVISITOR_H_

#include <vector>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {
namespace detail {

	/**
	 * Get the list of points from a Geometry
	 */
	class GetPointsVisitor : public ConstGeometryVisitor {
	public:
		virtual void visit( const Point & g ) ;
		virtual void visit( const LineString & g ) ;
		virtual void visit( const Polygon & g ) ;
		virtual void visit( const Triangle & g ) ;
		virtual void visit( const Solid & g ) ;
		virtual void visit( const MultiPoint & g ) ;
		virtual void visit( const MultiLineString & g ) ;
		virtual void visit( const MultiPolygon & g ) ;
		virtual void visit( const MultiSolid & g ) ;
		virtual void visit( const GeometryCollection & g ) ;
		virtual void visit( const PolyhedralSurface & g ) ;
		virtual void visit( const TriangulatedSurface & g ) ;
	public:
		std::vector< const Point * > points ;
	};


}//detail
}//SFCGAL


#endif
