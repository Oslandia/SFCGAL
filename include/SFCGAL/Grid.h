#ifndef _SFCGAL_GRID_H_
#define _SFCGAL_GRID_H_

#include <SFCGAL/ublas.h>
#include <SFCGAL/Point.h>

namespace SFCGAL {

	/**
	 *
	 * Represents a surface where points are located on a grid. The boundary of the grid is provided by an Envelope.
	 *
	 * Points may have no value represented by NaN.
	 *
	 *           columns / X
	 * +----------->
	 * |
	 * |
	 * |
	 * v lines / Y
	 *
	 * @warning not SFA (equivalent available in CityGML)
	 * @todo rename methods and refactor in order to match CityGML geometry
	 */
	class Grid {
	public:
		/**
		 * Empty grid builder
		 */
		Grid() ;
		/**
		 * Constructor with an extent and a grid size
		 */
		Grid( const ublas::matrix< double > & data, const Envelope & extent ) ;
		/**
		 * Copy constructor
		*/
		Grid( const Grid & other ) ;
		/**
		 * Copy constructor
		*/
		Grid& operator = ( const Grid & other ) ;
		/**
		 * Copy constructor
		*/
		virtual ~Grid();

		//-- SFCGAL::Geometry
		virtual bool           isEmpty() const ;

		/**
		 * Convert the Grid to a TriangulatedSurface.
		 *
		 * @warning triangles with no value are filtered
		 */
		TriangulatedSurface    toTriangulatedSurface() const ;


		/**
		virtual void visit( TriangulatedSurface & g ) ;
		 * returns the number of values along X axis
		 */
		inline size_t width() const { return _data.size1(); }
		/**
		 * returns the number of values along Y axis
		 */
		inline size_t height() const { return _data.size2(); }


		/**
		 * access points
		 */
		inline Point  point( const size_t & i, const size_t& j ) const {
			return Point( x(i,j), y(i,j), z(i,j) );
		}

		/**
		 * access X value (ro)
		 */
		inline double x( const size_t & i, const size_t & j ) const {
			return _xMin + j * _dx ;
		}
		/**
		 * access Y value (ro)
		 */
		inline double y( const size_t & i, const size_t & j ) const {
			return _yMax - i * _dy ;
		}

		/**
		 * access Z value (rw)
		 */
		inline double &       z( const size_t & i, const size_t & j ) {
			return _data(i,j) ;
		}
		/**
		 * access Z value (rw)
		 */
		inline const double & z( const size_t & i, const size_t & j ) const {
			return _data(i,j) ;
		}

	private:
		/**
		 * grid data
		 */
		ublas::matrix< double > _data ;
		/**
		 * top left corner
		 */
		double _xMin, _yMax ;
		/**
		 * "pixel" width, height
		 */
		double _dx, _dy ;
	};


}//SFCGAL

#endif
