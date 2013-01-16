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
