#ifndef _SFCGAL_GEOMETRYVISITOR_H_
#define _SFCGAL_GEOMETRYVISITOR_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {

	/**
	 * GeometryVisitor
	 * @todo remove switch an move implementation to virtual void Geometry::accept( GeometryVisitor & visitor)
	 */
	class GeometryVisitor {
	public:
		virtual ~GeometryVisitor() ;

		/**
		 * apply visitor
		 */
		virtual void visit( Geometry & g ) ;

		virtual void visit( Point & g ) = 0 ;
		virtual void visit( LineString & g ) = 0 ;
		virtual void visit( Polygon & g ) = 0 ;
		virtual void visit( Triangle & g ) = 0 ;
		virtual void visit( Solid & g ) = 0 ;

		/**
		 * invoke visit(Point) for each point
		 */
		virtual void visit( MultiPoint & g ) ;
		/**
		 * invoke visit(LineString) for each lineString
		 */
		virtual void visit( MultiLineString & g ) ;
		/**
		 * invoke visit(Polygon) for each polygon
		 */
		virtual void visit( MultiPolygon & g ) ;

		/**
		 * invoke g.accept(geometry) for each sub geometry
		 */
		virtual void visit( GeometryCollection & g ) ;

		/**
		 * invoke g.accept(polygon) for each polygon
		 */
		virtual void visit( PolyhedralSurface & g ) ;
		/**
		 * invoke g.accept(triangle) for each triangle
		 */
		virtual void visit( TriangulatedSurface & g ) ;

	};



}//SFCGAL


#endif
