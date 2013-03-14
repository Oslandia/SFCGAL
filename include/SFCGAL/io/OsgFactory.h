/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _SFCGAL_IO_OSGFACTORY_H_
#define _SFCGAL_IO_OSGFACTORY_H_

#include <SFCGAL/config.h>

#ifndef SFCGAL_WITH_OSG
#  error "SFCGAL is not built with OpenSceneGraph support, this header can't be included"
#endif

#include <osg/Geometry>

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace io {

	/**
	 * @brief helper class to convert SFCGAL::Geometry to osg::Geometry
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
