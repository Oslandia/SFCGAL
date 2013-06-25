/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _SFCGAL_COORDINATE_H_
#define _SFCGAL_COORDINATE_H_

#include <SFCGAL/config.h>

#include <boost/assert.hpp>
#include <boost/array.hpp>
#include <boost/variant.hpp>
#include <boost/serialization/split_member.hpp>

#include <SFCGAL/numeric.h>

#include <SFCGAL/Kernel.h>

namespace SFCGAL {

	/**
	 * @brief Represents the Coordinate of a Point (wraps either an empty structure, or a Kernel::Point_2,
	 * or a Kernel::Point_3)
	 */
	class SFCGAL_API Coordinate {
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
		Coordinate( const Kernel::Point_2 & other ) ;
		/**
		 * Constructor from CGAL::Point_3<K>
		 */
		Coordinate( const Kernel::Point_3 & other ) ;

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

		/**
		 * @brief Get the dimension of the coordinates
		 */
		int          coordinateDimension() const ;
		/**
		 * @brief Tests if the coordinates are empty
		 */
		bool         isEmpty() const ;
		/**
		 * @brief Tests if Z is defined
		 */
		bool         is3D() const ;

		//--- accessors

		/**
		 * @brief Gets the x value
		 * @warning Exact, 0 for empty coordinates
		 */
		Kernel::FT x() const;

		/**
		 * @brief Gets the y value
		 * @warning Exact, 0 for empty coordinates
		 */
		Kernel::FT y() const;

		/**
		 * @brief Gets the z value
		 * @warning Exact, 0 for empty or 2D coordinates
		 */
		Kernel::FT z() const;

		//-- helper

		/**
		 * @brief round coordinates with a scale factor
		 * @return *this
		 */
		Coordinate& round( const long& scaleFactor = 1 ) ;


		//-- comparator

		/**
		 * @brief Compares two points (lexicographic order)
		 *
		 * @warning coordinates must have the same dimension
		 */
		bool operator < ( const Coordinate & other ) const ;

		/**
		 * @brief Compares with an other point
		 *
		 * @warning coordinates must have the same dimension
		 */
		bool operator == ( const Coordinate & other ) const ;
		/**
		 * @brief Compares with an other point
		 *
		 * @warning coordinates must have the same dimension
		 */
		bool operator != ( const Coordinate & other ) const ;

		/**
		 * @brief Converts to Kernel::Vector_2
		 */
		inline Kernel::Vector_2 toVector_2() const
		{
			return Kernel::Vector_2( CGAL::ORIGIN, toPoint_2() );
		}

		/**
		 * @brief Converts to Kernel::Vector_3
		 */
		inline Kernel::Vector_3 toVector_3() const
		{
			return Kernel::Vector_3( CGAL::ORIGIN, toPoint_3() );
		}

		/**
		 * @brief Converts to Kernel::Point_2
		 */
		Kernel::Point_2 toPoint_2() const;

		/**
		 * @brief Converts to Kernel::Point_3
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
		void save( Archive& ar, const unsigned int /*version*/ ) const
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
		void load( Archive& ar, const unsigned int /*version*/ )
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
