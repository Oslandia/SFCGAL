#ifndef _SFCGAL_IO_WAVEFRONTOBJ_H_
#define _SFCGAL_IO_WAVEFRONTOBJ_H_

#include <map>
#include <vector>

#include <SFCGAL/Point.h>

namespace SFCGAL {
namespace io {

	/**
	 * An item in a face
	 *
	 * f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
	 *
	 */
	struct WaveFrontFaceItem {
		/**
		 * constructor with values
		 */
		WaveFrontFaceItem( int v_ = -1, int vt_ = -1, int vn_ = -1 );
		/**
		 * constructor from a string ( ex : v1/vt1/vn1 )
		 */
		WaveFrontFaceItem( const std::string & str );

		/**
		 * convert item to string
		 */
		std::string toString() const ;

		/**
		 * identifier in the vertex list (-1 if not defined)
		 */
		int    v ;
		/*
		 * identifier in the vertex texture list (-1 if not defined)
		 */
		int    vt ;
		/**
		 * identifier in the normal list
		 */
		int    vn ;
	};


	/**
	 * Represents a face in a wavefront obj
	 */
	typedef std::vector< WaveFrontFaceItem > WaveFrontFace ;


	/**
	 * Lightweight wavefront obj loader/writer
	 * (supports only vertices and faces)
	 */
	class WaveFrontObj {
	public:
		/**
		 * default constructor
		 */
		WaveFrontObj();

		/**
		 * clear content
		 */
		void clear() ;

		/**
		 * load a wavefront obj file
		 */
		void load( const std::string & filename ) ;
		/**
		 * save to file
		 */
		void save( const std::string & filename ) const ;
		/**
		 * save to ostream
		 */
		void save( std::ostream & s ) const ;

		/**
		 * returns the number of vertices
		 */
		inline size_t        numVertices() const { return _vertices.size(); }
		/**
		 * returns the n-th vertex
		 */
		inline Point &       vertexN( const size_t & n ) { return _vertices[n]; }
		/**
		 * returns the n-th vertex
		 */
		inline const Point & vertexN( const size_t & n ) const { return _vertices[n]; }


		/**
		 * returns the number of vertices
		 */
		inline size_t                 numFaces() const { return _faces.size(); }
		/**
		 * returns the n-th vertex
		 */
		inline WaveFrontFace &        faceN( const size_t & n ) { return _faces[n]; }
		/**
		 * returns the n-th vertex
		 */
		inline const WaveFrontFace &  faceN( const size_t & n ) const { return _faces[n]; }


		/**
		 * add a vertex, returns its position in the vertex list
		 * @warning no anti-duplicate
		 */
		size_t addVertex( const Point & vertex );
		/**
		 * add a face, returns its position in the face list
		 * @warning no anti-duplicate
		 */
		size_t addFace( const WaveFrontFace & face );

		/**
		 * add a Geometry
		 * @warning supports Polygon, MultiPolygon, TriangulatedSurface
		 */
		void   addGeometry( const Geometry & polygon ) ;
		/**
		 * add a Polygon
		 */
		void   addGeometry( const Polygon & polygon ) ;
		/**
		 * add a Polygon
		 */
		void   addGeometry( const MultiPolygon & polygon ) ;
		/**
		 * add a triangulated surface
		 */
		void   addGeometry( const TriangulatedSurface & triangulatedSurface ) ;


	private:
		/**
		 * vertices in the wavefront obj file
		 *
		 * v 0.123 0.234 0.345 1.0
		 */
		std::vector< Point >         _vertices ;

		/**
		 * The faces in the wavefront obj file
		 *
		 * @warning 0 indexed
		 *
		 * f 1001 901 878
		 */
		std::vector< WaveFrontFace > _faces ;


		/**
		 * load a line
		 */
		void _loadLine( const std::string & line ) ;
	};

}//io
}//SFCGAL


#endif

