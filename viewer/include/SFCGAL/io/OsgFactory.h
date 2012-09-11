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

		/**
		 * add a SFCGAL::Geometry to a osg::Geometry (dispatch method)
		 */
		void addToGeometry( osg::Geometry*, const Geometry& );

		/**
		 * add a Point to a osg::Geometry
		 */
		void addToGeometry( osg::Geometry*, const Point& );

		/**
		 * add a LineString to a osg::Geometry
		 */
		void addToGeometry( osg::Geometry*, const LineString& );

		/**
		 * add a Triangle to a osg::Geometry
		 */
		void addToGeometry( osg::Geometry*, const Triangle& );

		/**
		 * add a Polygon to a osg::Geometry
		 */
		void addToGeometry( osg::Geometry*, const Polygon& );

		/**
		 * add a TIN to a osg::Geometry
		 */
		void addToGeometry( osg::Geometry*, const TriangulatedSurface& );

		/**
		 * add a PolyhedralSurface to a osg::Geometry
		 */
		void addToGeometry( osg::Geometry*, const PolyhedralSurface& );

		/**
		 * add a Solid to a osg::Geometry
		 */
		void addToGeometry( osg::Geometry*, const Solid& );

		/**
		 * add a GeometryCollection to a osg::Geometry
		 */
		void addToGeometry( osg::Geometry*, const GeometryCollection& );
	};

}//io
}//SFCGAL


#endif
