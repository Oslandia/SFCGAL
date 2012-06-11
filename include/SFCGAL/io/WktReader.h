#ifndef _SFCGAL_IO_WKTREADER_H_
#define _SFCGAL_IO_WKTREADER_H_

#include <sstream>


#include <SFCGAL/Geometry.h>

#include <SFCGAL/tools/InputStreamReader.h>

namespace SFCGAL {
namespace io {

	/**
	 * read WKT geometry
	 *
	 * @warning M is ignored
	 * @ŧodo take M in account?
	 */
	class WktReader {
	public:
		/**
		 * read WKT from input stream
		 */
		WktReader( std::istream & s );

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
}//SFCGAL

#endif
