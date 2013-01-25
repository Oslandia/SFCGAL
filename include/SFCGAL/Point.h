#ifndef _SFCGAL_POINT_H_
#define _SFCGAL_POINT_H_

#include <SFCGAL/Coordinate.h>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/TypeForDimension.h>

#include <boost/serialization/base_object.hpp>

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
		 * Constructor with Coordinate
		 */
		Point( const Coordinate & coordinate ) ;
		/**
		 * XY Constructor with exact coordinates
		 */
		Point( const Kernel::FT & x, const Kernel::FT & y ) ;
		/**
		 * XY Constructor with exact coordinates
		 */
		Point( const Kernel::FT & x, const Kernel::FT & y, const Kernel::FT & z ) ;
		/**
		 * XY[Z] constructor
		 */
		Point( const double & x, const double & y, const double & z = NaN() ) ;

		/**
		 * Constructor from CGAL::Point_2<K>
		 */
		template < typename K >
		Point( const CGAL::Point_2< K > & other ):
			_coordinate(other)
		{

		}

		/**
		 * Constructor from CGAL::Point_3<K>
		 */
		template < typename K >
		Point( const CGAL::Point_3< K > & other ):
			_coordinate(other)
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

		/**
		 * Returns the x value as a double (NaN for empty Point)
		 */
		inline Kernel::RT x() const { return _coordinate.x() ; }
		/**
		 * Returns the y value as a double (NaN for empty Point)
		 */
		inline Kernel::RT y() const { return _coordinate.y() ; }
		/**
		 * Returns the z value as a double (NaN for empty or 2d Point)
		 */
		inline Kernel::RT z() const { return _coordinate.z() ; }


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
		 * @see Coordinate::toVector_2()
		 */
		inline Kernel::Vector_2 toVector_2() const
		{
			return _coordinate.toVector_2();
		}

		/**
		 * @see Coordinate::toVector_3()
		 */
		inline Kernel::Vector_3 toVector_3() const
		{
			return _coordinate.toVector_3();
		}

		/**
		 * @see Coordinate::toPoint_2()
		 */
		inline Kernel::Point_2 toPoint_2() const
		{
			return _coordinate.toPoint_2();
		}

		/**
		 * @see Coordinate::toPoint_3()
		 */
		inline Kernel::Point_3 toPoint_3() const
		{
			return _coordinate.toPoint_3();
		}

		/**
		 * @brief Converts to CGAL::Point_2 or CGAL::Point_3
		 */
		template <int D>
		typename TypeForDimension<D>::Point toPoint_d() const;

		inline Coordinate &       coordinate() { return _coordinate; }
		inline const Coordinate & coordinate() const { return _coordinate; }

		/**
		 * Serializer
		 */
		template <class Archive>
		void serialize( Archive& ar, const unsigned int version )
		{
			ar & boost::serialization::base_object<Geometry>(*this);
			ar & _coordinate;
		}
	private:
		Coordinate _coordinate ;
		//add m here, keep coordinate as a spatial position that can be shared
	};


}

#endif
