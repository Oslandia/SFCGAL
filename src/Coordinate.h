/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
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

#include <SFCGAL/detail/CoordinateToPoint2Visitor.h>
#include <SFCGAL/detail/CoordinateToPoint3Visitor.h>

namespace SFCGAL {

// class for Empty coordinate
class Empty {};

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
    Coordinate( const Kernel::FT& x, const Kernel::FT& y ) ;
    /**
     * XYZ Constructor with exact coordinates
     */
    Coordinate( const Kernel::FT& x, const Kernel::FT& y, const Kernel::FT& z ) ;

    /**
     * Constructor from CGAL::Point_2<K>
     */
    Coordinate( const Kernel::Point_2& other ) ;
    /**
     * Constructor from CGAL::Point_3<K>
     */
    Coordinate( const Kernel::Point_3& other ) ;

    /**
     * copy constructor
     */
    Coordinate( const Coordinate& other ) ;
    /**
     * assign operator
     */
    Coordinate& operator = ( const Coordinate& other ) ;
    /**
     * destructor
     */
    ~Coordinate() ;

    /**
     * @brief Get the dimension of the coordinates
     * TODO externalize
     */
    int          coordinateDimension() const ;
    /**
     * @brief Tests if the coordinates are empty
     * TODO externalize
     */
    bool         isEmpty() const ;
    /**
     * @brief Tests if Z is defined
     * TODO externalize
     */
    bool         is3D() const ;

    //--- accessors

    /**
     * @brief Gets the x value
     * @warning Exact, NaN for empty coordinates
     */
    Kernel::FT x() const;

    /**
     * @brief Gets the y value
     * @warning Exact, NaN for empty coordinates
     */
    Kernel::FT y() const;

    /**
     * @brief Gets the z value
     * @warning Exact, NaN for empty or 0 for 2D coordinates
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
    bool operator < ( const Coordinate& other ) const ;

    /**
     * @brief Compares with an other point
     *
     * @warning coordinates must have the same dimension
     */
    bool operator == ( const Coordinate& other ) const ;
    /**
     * @brief Compares with an other point
     *
     * @warning coordinates must have the same dimension
     */
    bool operator != ( const Coordinate& other ) const ;

    /**
     * @brief Converts to Kernel::Vector_2
     */
    template < typename K >
    inline typename K::Vector_2 toVector_2() const {
        return typename K::Vector_2( CGAL::ORIGIN, toPoint_2<K>() );
    }

    /**
     * @brief Converts to Kernel::Vector_3
     */
    template < typename K >
    inline typename K::Vector_3 toVector_3() const {
        return typename K::Vector_3( CGAL::ORIGIN, toPoint_3<K>() );
    }

    /**
     * @brief Converts to Kernel::Point_2
     */
    template < typename K >
    typename K::Point_2 toPoint_2() const {
        detail::CoordinateToPoint2Visitor<K> visitor;
        return boost::apply_visitor(visitor,_storage) ;
    }

    /**
     * @brief Converts to Kernel::Point_3
     */
    template < typename K >
    typename K::Point_3 toPoint_3() const {
        detail::CoordinateToPoint3Visitor<K> visitor;
        return boost::apply_visitor(visitor,_storage) ;
    }

private:
    boost::variant<
        Empty,
        Epick::Point_2,
        Epick::Point_3,
        Epeck::Point_2,
        Epeck::Point_3
    > _storage;
public:
    /**
     * Serialization
     * TODO remove or visit coordinates
     */
    template <class Archive>
    void save( Archive& ar, const unsigned int /*version*/ ) const {
        int dim = coordinateDimension();
        ar << dim;

        if ( _storage.which() > 0 ) {
            Kernel::FT x_ = toPoint_2<Kernel>().x();
            Kernel::FT y_ = toPoint_2<Kernel>().y();
            ar << x_;
            ar << y_;

            if ( _storage.which() == 2 ) {
                Kernel::FT z_ = toPoint_3<Kernel>().z();
                ar << z_;
            }
        }
    }

    template <class Archive>
    void load( Archive& ar, const unsigned int /*version*/ ) {
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
    void serialize( Archive& ar, const unsigned int version ) {
        boost::serialization::split_member( ar, *this, version );
    }
};


}//SFCGAL

#endif
