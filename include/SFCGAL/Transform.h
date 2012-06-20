#ifndef _SFCGAL_TRANSFORM_H_
#define _SFCGAL_TRANSFORM_H_

#include <SFCGAL/Geometry.h>
#include <SFCGAL/GeometryVisitor.h>


namespace SFCGAL {

	/**
	 * Represents a coordinate transform
	 */
	class Transform : public GeometryVisitor {
	public:
		virtual ~Transform() ;
		/**
		 * apply transform to a geometry
		 */
		virtual void transform( Point & p ) = 0 ;

		virtual void visit( Point & g ) ;
		virtual void visit( LineString & g ) ;
		virtual void visit( Polygon & g ) ;
		virtual void visit( Triangle & g ) ;
		virtual void visit( Solid & g ) ;
		virtual void visit( MultiPoint & g );
		virtual void visit( MultiLineString & g ) ;
		virtual void visit( MultiPolygon & g );
		virtual void visit( GeometryCollection & g ) ;
		virtual void visit( PolyhedralSurface & g ) ;
		virtual void visit( TriangulatedSurface & g ) ;
	};


}//SFCGAL


#endif
