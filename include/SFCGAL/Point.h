#ifndef _SFCGAL_POINT_H_
#define _SFCGAL_POINT_H_

#include <boost/assert.hpp>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/Coordinate.h>

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
		 * XY constructor
		 */
		Point( const double & x, const double & y ) ;
		/**
		 * XYZ constructor
		 */
		Point( const double & x, const double & y, const double & z ) ;
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

		inline const double& x() const { BOOST_ASSERT(_coordinates.size() >= 1); return _coordinates[0] ; }
		inline double & x() { BOOST_ASSERT(_coordinates.size() >= 1); _isEmpty = false; return _coordinates[0] ; }

		inline const double& y() const { BOOST_ASSERT(_coordinates.size() >= 2); return _coordinates[1] ; }
		inline double & y() { BOOST_ASSERT(_coordinates.size() >= 2); _isEmpty = false; return _coordinates[1] ; }

		inline const double& z() const { BOOST_ASSERT(_coordinates.size() >= 3); return _coordinates[2] ; }
		inline double & z() { BOOST_ASSERT(_coordinates.size() >= 3); _isEmpty = false; return _coordinates[2] ; }

		/**
		 * @warning may change
		 */
		inline Coordinate &       coordinates() { _isEmpty = false; return _coordinates; }
		/**
		 * @warning may change
		 */
		inline const Coordinate & coordinates() const { return _coordinates; }


		//--- comparators

		bool operator == (const Point & other ) const ;
		bool operator != (const Point &p ) const ;

		//-- visitors

		//-- SFCGAL::Geometry
		virtual void accept( GeometryVisitor & visitor ) ;
		//-- SFCGAL::Geometry
		virtual void accept( ConstGeometryVisitor & visitor ) const ;

	private:
		/**
		 * Represents the coordinates of the point (resizable)
		 */
		Coordinate _coordinates ;

		bool _is3D;
		bool _isEmpty;
	};


}

#endif
