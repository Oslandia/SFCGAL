#ifndef _SFCGAL_COORDINATE_H_
#define _SFCGAL_COORDINATE_H_

#include <boost/assert.hpp>

#include <SFCGAL/numeric.h>

#include <CGAL/Vector_2.h>
#include <CGAL/Vector_3.h>

#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>

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
		 */
		Coordinate( const double & x, const double & y, const double & z = NaN() ) ;

		/**
		 * Constructor from CGAL::Point_2<K>
		 */
		template < typename K >
		Coordinate( const CGAL::Point_2< K > & other ):
			_x(CGAL::to_double(other.x())),
			_y(CGAL::to_double(other.y())),
			_z(NaN())
		{

		}

		/**
		 * Constructor from CGAL::Point_3<K>
		 */
		template < typename K >
		Coordinate( const CGAL::Point_3< K > & other ):
			_x(CGAL::to_double(other.x())),
			_y(CGAL::to_double(other.y())),
			_z(CGAL::to_double(other.z()))
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

		inline const double& x() const { return _x ; }
		inline double & x() { return _x ; }

		inline const double& y() const { return _y ; }
		inline double & y() { return _y ; }

		inline const double& z() const { return _z ; }
		inline double & z() { return _z ; }


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
		 */
		template < typename K >
		inline CGAL::Vector_2< K > toVector_2() const
		{
			return CGAL::Vector_2< K >(
				isNaN(_x) ? 0.0 : _x,
				isNaN(_y) ? 0.0 : _y
			);
		}

		/**
		 * Convert to CGAL::Vector_3
		 */
		template < typename K >
		inline CGAL::Vector_3< K > toVector_3() const
		{
			return CGAL::Vector_3< K >(
				isNaN(_x) ? 0.0 : _x,
				isNaN(_y) ? 0.0 : _y,
				isNaN(_z) ? 0.0 : _z
			);
		}

		/**
		 * Convert to CGAL::Point_2
		 */
		template < typename K >
		inline CGAL::Point_2< K > toPoint_2() const
		{
			return CGAL::Point_2< K >(
				isNaN(_x) ? 0.0 : _x,
				isNaN(_y) ? 0.0 : _y
			);
		}

		/**
		 * Convert to CGAL::Point_3
		 */
		template < typename K >
		inline CGAL::Point_3< K > toPoint_3() const
		{
			return CGAL::Point_3< K >(
				isNaN(_x) ? 0.0 : _x,
				isNaN(_y) ? 0.0 : _y,
				isNaN(_z) ? 0.0 : _z
			);
		}

	private:
		double _x ;
		double _y ;
		double _z ;
	};


}//SFCGAL

#endif
