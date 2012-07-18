#ifndef _SFCGAL_IO_OSGGEOMETRYWRITER_H_
#define _SFCGAL_IO_OSGGEOMETRYWRITER_H_

#include <osg/Geometry>
#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace io {

	/**
	 * OSG factory
	 */
	class OsgFactory {
	public:
		/**
		 * create a osg::Geometry from a Point
		 */
		osg::Geometry* createGeometry( const Geometry & g ) ;

		/**
		 * create a osg::Geometry from a Point
		 */
		osg::Geometry* createGeometry( const Point & g ) ;
		/**
		 * create a osg::Geometry from a LineString
		 */
		osg::Geometry* createGeometry( const LineString & g ) ;
		/**
		 * create a osg::Geometry from a Polygon
		 */
		osg::Geometry* createGeometry( const Polygon & g ) ;
		/**
		 * create a osg::Geometry from a Triangle
		 */
		osg::Geometry* createGeometry( const Triangle & g ) ;
		/**
		 * create a osg::Geometry from a Solid
		 */
		osg::Geometry* createGeometry( const Solid & g ) ;

		/**
		 * create a osg::Geometry from a TriangulatedSurface
		 * @todo compute normals
		 */
		osg::Geometry* createGeometry( const TriangulatedSurface & g ) ;
		/**
		 * create a osg::Geometry from a PolyhedralSurface
		 */
		osg::Geometry* createGeometry( const PolyhedralSurface & g ) ;


		/**
		 * create a osg::Geometry from a Point
		 */
		osg::Geometry* createGeometry( const GeometryCollection & g ) ;

		/**
		 * create a osg::Geometry from a Point
		 */
		osg::Geometry* createGeometry( const MultiPoint & g ) ;
		/**
		 * create a osg::Geometry from a LineString
		 */
		osg::Geometry* createGeometry( const MultiLineString & g ) ;
		/**
		 * create a osg::Geometry from a Polygon
		 */
		osg::Geometry* createGeometry( const MultiPolygon & g ) ;
		/**
		 * create a osg::Geometry from a Solid
		 */
		osg::Geometry* createGeometry( const MultiSolid & g ) ;

		/**
		 * create a osg::Vec3 from a Point
		 */
		osg::Vec3 createVec3( const Point & g ) const ;

	protected:
		/**
		 * create a vertex and returns its position in a vertice array
		 */
		size_t createVertex( osg::Vec3Array * vertices, const Point & g ) ;
		/**
		 * create a vertex and returns its position in a vertice array
		 */
		size_t createVertex( osg::Vec3Array * vertices, const osg::Vec3 & g ) ;
	};

}//io
}//SFCGAL


#endif
