#ifndef _SFCGAL_COORDINATE_H_
#define _SFCGAL_COORDINATE_H_

#include <boost/assert.hpp>
#include <boost/array.hpp>
#include <boost/variant.hpp>

#include <SFCGAL/numeric.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/io/Serialization.h>

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
			_storage( other )
		{

		}

		/**
		 * Constructor from CGAL::Point_3<K>
		 */
		template < typename K >
		Coordinate( const CGAL::Point_3< K > & other ):
			_storage(other)
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
		Kernel::FT x() const;

		/**
		 * Returns the y value as a double (NaN for empty coordinates)
		 */
		Kernel::FT y() const;

		/**
		 * Returns the z value as a double (NaN for empty or 2d Coordinate)
		 */
		Kernel::FT z() const;

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
			return Kernel::Vector_2( CGAL::ORIGIN, toPoint_2() );
		}

		/**
		 * Convert to CGAL::Vector_3
		 * @todo remove template parameter
		 */
		inline Kernel::Vector_3 toVector_3() const
		{
			return Kernel::Vector_3( CGAL::ORIGIN, toPoint_3() );
		}

		/**
		 * Convert to CGAL::Point_2
		 * @todo remove template parameter
		 */
		Kernel::Point_2 toPoint_2() const;

		/**
		 * Convert to CGAL::Point_3
		 * @todo remove template parameter
		 */
		Kernel::Point_3 toPoint_3() const;

		// class for Empty coordinate
		class Empty {};
	private:
		boost::variant< Empty, Kernel::Point_2, Kernel::Point_3 > _storage;

	public:
		/**
		 * Serialization
		 */
		template <class Archive>
		void save( Archive& ar, const unsigned int version ) const
		{
			int dim = coordinateDimension();
			ar << dim;
			if ( _storage.which() > 0 ) {
				const Kernel::FT& x_ = x();
				const Kernel::FT& y_ = y();
				ar << x_;
				ar << y_;
				if ( _storage.which() == 2 ) {
					const Kernel::FT& z_ = z();
					ar << z_;
				}
			}
		}

		template <class Archive>
		void load( Archive& ar, const unsigned int version )
		{
			int dim;
			ar >> dim;
			if ( dim == 0 ) {
				_storage = Empty();
			}
			else if ( dim == 2 ) {
				Kernel::FT x_, y_;
				ar >> x_;
				ar >> y_;
				_storage = Kernel::Point_2( x_, y_ );
			}
			else if ( dim == 3 ) {
				Kernel::FT x_, y_, z_;
				ar >> x_;
				ar >> y_;
				ar >> z_;
				_storage = Kernel::Point_3( x_, y_, z_ );
			}
		}

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			boost::serialization::split_member(ar, *this, version);
		}
	};


}//SFCGAL

#endif
