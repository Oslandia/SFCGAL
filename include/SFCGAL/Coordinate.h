#ifndef _SFCGAL_COORDINATE_H_
#define _SFCGAL_COORDINATE_H_

#include <boost/assert.hpp>
#include <boost/array.hpp>

#include <SFCGAL/numeric.h>

#include <SFCGAL/Kernel.h>

namespace SFCGAL {

	/**
	 * Represents the Coordinate of a Point
	 */
	class Coordinate {
	public:
		/**
		 * Empty Coordinate constructor
		 */
		Coordinate() ;
		/**
		 * XY Constructor with exact coordinates
		 */
		Coordinate( const Kernel::FT & x, const Kernel::FT & y ) ;
		/**
		 * XYZ Constructor with exact coordinates
		 */
		Coordinate( const Kernel::FT & x, const Kernel::FT & y, const Kernel::FT & z ) ;
		/**
		 * XY[Z] constructor
		 * @warning x,y,z may be NaN
		 */
		Coordinate( const double & x, const double & y, const double & z = NaN() ) ;

		/**
		 * Constructor from CGAL::Point_2<K>
		 */
		template < typename K >
		Coordinate( const CGAL::Point_2< K > & other ):
			_dimension(2),
			_x(other.x()),
			_y(other.y()),
			_z(0)
		{

		}

		/**
		 * Constructor from CGAL::Point_3<K>
		 */
		template < typename K >
		Coordinate( const CGAL::Point_3< K > & other ):
			_dimension(3),
			_x(other.x()),
			_y(other.y()),
			_z(other.z())
		{

		}


		/**
		 * copy constructor
		 */
		Coordinate( const Coordinate & other ) ;
		/**
		 * assign operator
		 */
		Coordinate& operator = ( const Coordinate & other ) ;
		/**
		 * destructor
		 */
		~Coordinate() ;

		//-- SFCGAL::Geometry
		int          coordinateDimension() const ;
		//-- SFCGAL::Geometry
		bool         isEmpty() const ;
		//-- SFCGAL::Geometry
		bool         is3D() const ;

		//--- accessors

		/**
		 * Returns the x value as a double (NaN for empty coordinates)
		 */
		inline Kernel::FT x() const { return _x ; }
		/**
		 * Returns the y value as a double (NaN for empty coordinates)
		 */
		inline Kernel::FT y() const { return _y ; }
		/**
		 * Returns the z value as a double (NaN for empty or 2d Coordinate)
		 */
		inline Kernel::FT z() const { return _z ; }


		/**
		 * compare two points
		 */
		bool operator < ( const Coordinate & other ) const ;

		/**
		 * compare with an other point
		 */
		bool operator == ( const Coordinate & other ) const ;
		/**
		 * compare with an other point
		 */
		bool operator != ( const Coordinate & other ) const ;

		/**
		 * Convert to CGAL::Vector_2
		 * @todo remove template parameter
		 */
		inline Kernel::Vector_2 toVector_2() const
		{
			return Kernel::Vector_2( _x, _y );
		}

		/**
		 * Convert to CGAL::Vector_3
		 * @todo remove template parameter
		 */
		inline Kernel::Vector_3 toVector_3() const
		{
			return Kernel::Vector_3( _x, _y, _z );
		}

		/**
		 * Convert to CGAL::Point_2
		 * @todo remove template parameter
		 */
		inline Kernel::Point_2 toPoint_2() const
		{
			return Kernel::Point_2( _x, _y );
		}

		/**
		 * Convert to CGAL::Point_3
		 * @todo remove template parameter
		 */
		inline Kernel::Point_3 toPoint_3() const
		{
			return Kernel::Point_3( _x, _y, _z );
		}

	private:
		size_t     _dimension ;
		Kernel::FT _x ;
		Kernel::FT _y ;
		Kernel::FT _z ;

		//double _m
	};


}//SFCGAL

#endif
