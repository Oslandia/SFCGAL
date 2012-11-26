#ifndef _SFCGAL_COORDINATE_H_
#define _SFCGAL_COORDINATE_H_

#include <boost/assert.hpp>

#include <SFCGAL/numeric.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/detail/CoordinateStorage.h>

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
		 * XY[Z] constructor
		 * @warning x,y,z may be NaN
		 */
		Coordinate( const double & x, const double & y, const double & z = NaN() ) ;

		/**
		 * Constructor from CGAL::Point_2<K>
		 */
		template < typename K >
		Coordinate( const CGAL::Point_2< K > & other ):
			_xyz(other)
		{

		}

		/**
		 * Constructor from CGAL::Point_3<K>
		 */
		template < typename K >
		Coordinate( const CGAL::Point_3< K > & other ):
			_xyz(other)
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
		inline double x() const { return detail::x(_xyz) ; }
		/**
		 * Returns the y value as a double (NaN for empty coordinates)
		 */
		inline double y() const { return detail::y(_xyz) ; }
		/**
		 * Returns the z value as a double (NaN for empty or 2d Coordinate)
		 */
		inline double z() const { return detail::z(_xyz) ; }


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
			return detail::toVector_2( _xyz );
		}

		/**
		 * Convert to CGAL::Vector_3
		 * @todo remove template parameter
		 */
		inline Kernel::Vector_3 toVector_3() const
		{
			return detail::toVector_3( _xyz );
		}

		/**
		 * Convert to CGAL::Point_2
		 * @todo remove template parameter
		 */
		inline Kernel::Point_2 toPoint_2() const
		{
			return detail::toPoint_2( _xyz );
		}

		/**
		 * Convert to CGAL::Point_3
		 * @todo remove template parameter
		 */
		inline Kernel::Point_3 toPoint_3() const
		{
			return detail::toPoint_3( _xyz );
		}

	private:
		detail::CoordinateStorage _xyz ;
		//double _m
	};


}//SFCGAL

#endif
