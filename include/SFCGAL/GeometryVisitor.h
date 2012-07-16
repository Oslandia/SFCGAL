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
		virtual void visit( MultiPoint & g ) = 0 ;
		virtual void visit( MultiLineString & g ) = 0 ;
		virtual void visit( MultiPolygon & g ) = 0 ;
		virtual void visit( MultiSolid & g ) = 0 ;
		virtual void visit( GeometryCollection & g ) = 0 ;
		virtual void visit( PolyhedralSurface & g ) = 0 ;
		virtual void visit( TriangulatedSurface & g ) = 0 ;
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
		virtual void visit( const MultiPoint & g ) = 0 ;
		virtual void visit( const MultiLineString & g ) = 0 ;
		virtual void visit( const MultiPolygon & g ) = 0 ;
		virtual void visit( const MultiSolid & g ) = 0 ;
		virtual void visit( const GeometryCollection & g ) = 0 ;
		virtual void visit( const PolyhedralSurface & g ) = 0 ;
		virtual void visit( const TriangulatedSurface & g ) = 0 ;
	};


}//SFCGAL


#endif
