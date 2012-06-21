#ifndef _SFCGAL_DETAIL_GETPOINTSVISITOR_H_
#define _SFCGAL_DETAIL_GETPOINTSVISITOR_H_

#include <vector>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {
namespace detail {

	/**
	 * Get the list of points from a Geometry
	 */
	class GetPointsVisitor : public GeometryVisitor {
	public:
		virtual void visit( Point & g ) ;
		virtual void visit( LineString & g ) ;
		virtual void visit( Polygon & g ) ;
		virtual void visit( Triangle & g ) ;
		virtual void visit( Solid & g ) ;
		virtual void visit( MultiPoint & g ) ;
		virtual void visit( MultiLineString & g ) ;
		virtual void visit( MultiPolygon & g ) ;
		virtual void visit( GeometryCollection & g ) ;
		virtual void visit( PolyhedralSurface & g ) ;
		virtual void visit( TriangulatedSurface & g ) ;
	public:
		std::vector< Point * > points ;
	};


}//detail
}//SFCGAL


#endif
