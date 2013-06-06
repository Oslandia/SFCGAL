/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
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
#ifndef _SFCGAL_IO_WKTREADER_H_
#define _SFCGAL_IO_WKTREADER_H_

#include <sstream>

#include <SFCGAL/config.h>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/PreparedGeometry.h>

#include <SFCGAL/detail/tools/InputStreamReader.h>

namespace SFCGAL {
namespace detail {
namespace io {

	/**
	 * read WKT geometry
	 *
	 * @warning M is ignored
	 * @ŧodo take M in account?
	 */
	class SFCGAL_API WktReader {
	public:
		/**
		 * read WKT from input stream
		 */
		WktReader( std::istream & s );

		/**
		 * read an SRID, if present
		 *
		 */
		srid_t        readSRID() ;

		/**
		 * read a geometry from a string
		 *
		 * @warning returns new instance
		 */
		Geometry*     readGeometry() ;

		/**
		 * read geometry type
		 */
		GeometryType   readGeometryType() ;
		/**
		 * read coordinate type [Z][M]
		 */
		CoordinateType readCoordinateType() ;

		/**
		 * Read Point content from wkt
		 *
		 * ex : (1.0 2.0 14.0)
		 */
		void                     readInnerPoint( Point & g ) ;
		/**
		 * Read LineString content from wkt
		 *
		 * ex : (1.0 2.0,1.0,6.0)
		 */
		void                     readInnerLineString( LineString & g ) ;
		/**
		 * Read Polygon content from wkt
		 *
		 * ex : ((30 10, 10 20, 20 40, 40 40, 30 10))
		 */
		void                     readInnerPolygon( Polygon & g ) ;
		/**
		 * Read Triangle content from wkt
		 */
		void                     readInnerTriangle( Triangle & g ) ;

		/**
		 * Read MultiPoint content from wkt
		 *
		 * ex : (0.0 1.0,5.0 6.0) or ((0.0 4.0),(5.0 6.0))
		 */
		void                     readInnerMultiPoint( MultiPoint & g ) ;
		/**
		 * Read MultiLineString content from wkt
		 */
		void                     readInnerMultiLineString( MultiLineString & g ) ;
		/**
		 * Read MultiPolygon content from wkt
		 */
		void                     readInnerMultiPolygon( MultiPolygon & g ) ;

		/**
		 * Read GeometryCollection content from wkt
		 */
		void                     readInnerGeometryCollection( GeometryCollection & g ) ;

		/**
		 * Read TriangulatedSurface content from wkt
		 */
		void                     readInnerTriangulatedSurface( TriangulatedSurface & g ) ;

		/**
		 * Read PolyhedralSurface content from wkt
		 */
		void                     readInnerPolyhedralSurface( PolyhedralSurface & g ) ;


		/**
		 * Read Solid content from wkt
		 */
		void                     readInnerSolid( Solid & g ) ;

		/**
		 * Read MultiSolid content from wkt
		 */
		void                     readInnerMultiSolid( MultiSolid & g ) ;

		/**
		 * Read coordinate from WKT
		 * @todo ZM management
		 */
		bool                     readPointCoordinate( Point & p ) ;

	private:
		/**
		 * input stream
		 */
		tools::InputStreamReader _reader;

		/**
		 * actually reading 3D ?
		 */
		bool _is3D ;
		/**
		 * actually reading Measured ?
		 */
		bool _isMeasured ;


		/**
		 * returns default parse error message
		 */
		std::string parseErrorMessage() ;
	};

}//io
}//detail
}//SFCGAL

#endif
