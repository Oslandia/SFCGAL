#ifndef _SFCGAL_GEOMETRYVISITOR_H_
#define _SFCGAL_GEOMETRYVISITOR_H_

#include <SFCGAL/Geometry.h>

namespace SFCGAL {

	/**
	 * GeometryVisitor
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


	/**
	 * Visitor for const geometries
	 */
	class ConstGeometryVisitor {
	public:
		virtual ~ConstGeometryVisitor() ;

		/**
		 * apply visitor
		 */
		virtual void visit( const Geometry & g ) ;

		virtual void visit( const Point & g ) = 0 ;
		virtual void visit( const LineString & g ) = 0 ;
		virtual void visit( const Polygon & g ) = 0 ;
		virtual void visit( const Triangle & g ) = 0 ;
		virtual void visit( const Solid & g ) = 0 ;

		/**
		 * invoke visit(Point) for each point
		 */
		virtual void visit( const MultiPoint & g ) ;
		/**
		 * invoke visit(LineString) for each lineString
		 */
		virtual void visit( const MultiLineString & g ) ;
		/**
		 * invoke visit(Polygon) for each polygon
		 */
		virtual void visit( const MultiPolygon & g ) ;

		/**
		 * invoke g.accept(geometry) for each sub geometry
		 */
		virtual void visit( const GeometryCollection & g ) ;

		/**
		 * invoke g.accept(polygon) for each polygon
		 */
		virtual void visit( const PolyhedralSurface & g ) ;
		/**
		 * invoke g.accept(triangle) for each triangle
		 */
		virtual void visit( const TriangulatedSurface & g ) ;
	};


}//SFCGAL


#endif
