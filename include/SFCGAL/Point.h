#ifndef _SFCGAL_POINT_H_
#define _SFCGAL_POINT_H_

#include <boost/assert.hpp>

#include <SFCGAL/numeric.h>

#include <SFCGAL/Geometry.h>

#include <CGAL/Vector_2.h>
#include <CGAL/Vector_3.h>

#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>

namespace SFCGAL {

	/**
	 * A point in SFA.
	 *
	 * The x(),y(),z() interface is based on CGAL kernel requirements, taken
	 * from examples/Kernel_23/MyPointC2.h
	 *
	 * Coordinates are stored as an ublas::vector
	 *
	 * @todo replace Coordinate by CGAL::Point_2 or CGAL::Point_3?
	 * @todo strong typing on coordinate dimension?
	 * @todo kernel as parameter?
	 */
	class Point : public Geometry {
	public:
		/**
		 * Empty point constructor
		 */
		Point() ;
		/**
		 * XY[Z] constructor
		 */
		Point( const double & x, const double & y, const double & z = NaN() ) ;

		/**
		 * Constructor from CGAL::Point_2<K>
		 */
		template < typename K >
		Point( const CGAL::Point_2< K > & other ):
			_x(other.x()),
			_y(other.y()),
			_z(NaN())
		{

		}

		/**
		 * Constructor from CGAL::Point_3<K>
		 */
		template < typename K >
		Point( const CGAL::Point_3< K > & other ):
			_x(other.x()),
			_y(other.y()),
			_z(other.z())
		{

		}


		/**
		 * copy constructor
		 */
		Point( const Point & other ) ;
		/**
		 * assign operator
		 */
		Point& operator = ( const Point & other ) ;
		/**
		 * destructor
		 */
		~Point() ;

		//-- SFCGAL::Geometry
		virtual Point *  clone() const ;

		//-- SFCGAL::Geometry
		virtual std::string  geometryType() const ;
		//-- SFCGAL::Geometry
		virtual GeometryType geometryTypeId() const ;
		//-- SFCGAL::Geometry
		virtual int          dimension() const ;
		//-- SFCGAL::Geometry
		virtual int          coordinateDimension() const ;
		//-- SFCGAL::Geometry
		virtual bool         isEmpty() const ;
		//-- SFCGAL::Geometry
		virtual bool         is3D() const ;

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
		bool operator < ( const Point & other ) const ;

		/**
		 * compare with an other point
		 */
		bool operator == ( const Point & other ) const ;
		/**
		 * compare with an other point
		 */
		bool operator != ( const Point & other ) const ;

		//-- visitors

		//-- SFCGAL::Geometry
		virtual void accept( GeometryVisitor & visitor ) ;
		//-- SFCGAL::Geometry
		virtual void accept( ConstGeometryVisitor & visitor ) const ;



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


}

#endif
