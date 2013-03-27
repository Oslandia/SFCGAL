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
#ifndef _SFCGAL_IO_WKTWRITER_H_
#define _SFCGAL_IO_WKTWRITER_H_

#include <sstream>

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace io {

	/**
	 * Writer for WKT
	 *
	 * @warning Triangles are transformed into polygons
	 */
	class WktWriter {
	public:
		WktWriter( std::ostream & s ) ;

		/**
		 * @todo replace with visitor dispatch
		 */
		void write( const Geometry & g, bool exact = false ) ;

	protected:
		/**
		 * write coordinate type (""|" Z"|" ZM")
		 */
		void writeCoordinateType( const Geometry& g );

		void writeCoordinate( const Point& g );

		void write( const Point & g ) ;
		void writeInner( const Point & g ) ;

		void write( const LineString & g ) ;
		void writeInner( const LineString & g ) ;

		void write( const Polygon & g ) ;
		void writeInner( const Polygon & g ) ;

		void write( const GeometryCollection & g ) ;

		void write( const MultiPoint & g ) ;
		void write( const MultiLineString & g ) ;
		void write( const MultiPolygon & g ) ;
		void write( const MultiSolid & g ) ;

		void write( const Triangle & g );
		void writeInner( const Triangle & g ) ;

		void write( const TriangulatedSurface & g );

		void write( const PolyhedralSurface & g );
		void writeInner( const PolyhedralSurface & g );

		void write( const Solid & g );
		void writeInner( const Solid & g );
	private:
		std::ostream & _s ;
		bool _exactWrite;
	};


}//io
}//SFCGAL

#endif
