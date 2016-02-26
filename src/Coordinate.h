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

#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <boost/array.hpp>

#include <SFCGAL/numeric.h>
#include <SFCGAL/Kernel.h>

namespace SFCGAL {

/**
 * @brief Coordinate class represents the Coordinate of a Point. It is 
 * stored as an Epeck::Point_3. The real dimension (Empty, Point_2, Point_3) is 
 * tagged as a "storageDimension".
 *
 * As it's often necessary to attach data to Point in 3D (textures coordinates, normals, etc.), 
 * the Coordinate is also able to carry a user data as a "Measure". In OGC SFA,
 * the measure is a "double" corresponding to the M coordinate. At coordinate level, 
 * SFCGAL extends this concept in order to allow the storage of any user data bellow a boost::any (a safe void*).
 * 
 * @warning In order to keep standard conformance, at geometry level, the coordinate is measured if
 *  and only if the measure is a double considered as NaN if cast to double is impossible.
 * 
 */
class SFCGAL_API Coordinate {
public:
    typedef boost::uint8_t storage_dimension_t ;
    /**
     * Empty Coordinate constructor
     */
    Coordinate() ;
    /**
     * XY Constructor with double coordinates
     * @deprecated
     */
    Coordinate( const double& x, const double& y ) ;
    /**
     * XYZ Constructor with double coordinates
     * @deprecated
     */
    Coordinate( const double& x, const double& y, const double& z ) ;
    /**
     * XY Constructor with exact coordinates
     * @deprecated
     */
    Coordinate( const Epeck::FT& x, const Epeck::FT& y ) ;
    /**
     * XYZ Constructor with exact coordinates
     * @deprecated
     */
    Coordinate( const Epeck::FT& x, const Epeck::FT& y, const Epeck::FT& z ) ;

    /**
     * Constructor from CGAL::Point_2<K>
     */
    template < typename K >
    Coordinate( const CGAL::Point_2<K>& other ) {
        CGAL::Cartesian_converter<K,Epeck> converter;
        _storage = converter(
            CGAL::Point_3<K>(other.x(),other.y(),0)
        );
        _storageDimension = 2;
    }

    /**
     * Constructor from CGAL::Point_3<K>
     */
    template < typename K >
    Coordinate( const CGAL::Point_3<K>& point ) {
        CGAL::Cartesian_converter<K,Epeck> converter;
        _storage = converter(point);
        _storageDimension = 3;
    }

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
     * @brief Tests if the coordinates are empty
     */
    bool isEmpty() const {
        return _storageDimension == 0 ;
    }
    /**
     * @brief Tests if Z is defined
     */
    bool is3D() const {
        return _storageDimension == 3 ;
    }

    //--- accessors

    /**
     * @brief Gets the x value
     * @warning Exact, NaN for empty coordinates
     */
    Epeck::FT x() const {
        return _storage.x() ;
    }

    /**
     * @brief Gets the y value
     * @warning Exact, 0 for empty coordinates
     */
    Epeck::FT y() const {
        return _storage.y() ;
    }

    /**
     * @brief Gets the z value
     * @warning Exact, 0 if not 3D
     */
    Epeck::FT z() const {
        return _storage.z() ;
    }
    
    
    /**
     * Returns the measure
     */
    inline boost::any measure() const {
        return _measure ;
    }
    /**
     * Set the measure
     */
    inline void setMeasure( boost::any measure ) {
        _measure = measure;
    }

    //-- helper

    /**
     *
     * @brief round coordinates with a scale factor. This operation
     * is performed in order to reduce the size of the integer in
     * CGAL exact number representation (mpq_class in GMP). 
     * 
     * @return *this
     */
    Coordinate& round( const long& scaleFactor = 1 ) ;


    //-- comparator

    /**
     * @brief Compares two points (lexicographic order)
     *
     * @warning compare each value of the 3D storage vector, should 
     *   be used with caution when working different dimension
     */
    bool operator < ( const Coordinate& other ) const ;

    /**
     * @brief Compares with an other point
     *
     * @warning compare each value of the 3D storage vector, should 
     *   be used with caution when working different dimension
     */
    bool operator == ( const Coordinate& other ) const ;
    /**
     * @brief Compares with an other point
     *
     * @warning compare each value of the 3D storage vector, should 
     *   be used with caution when working different dimension
     */
    bool operator != ( const Coordinate& other ) const ;

    /**
     * @brief Converts to K::Vector_2
     */
    template < typename K >
    inline typename K::Vector_2 toVector_2() const {
        return typename K::Vector_2( CGAL::ORIGIN, toPoint_2<K>() );
    }

    /**
     * @brief Converts to K::Vector_3
     */
    template < typename K >
    inline typename K::Vector_3 toVector_3() const {
        return typename K::Vector_3( CGAL::ORIGIN, toPoint_3<K>() );
    }

    /**
     * @brief Converts to K::Point_2
     */
    template < typename K >
    typename K::Point_2 toPoint_2() const {
        CGAL::Cartesian_converter<Epeck,K> converter;
        return converter( Epeck::Point_2( x(), y() ) );
    }

    /**
     * @brief Converts to K::Point_3
     */
    template < typename K >
    typename K::Point_3 toPoint_3() const {
        CGAL::Cartesian_converter<Epeck,K> converter;
        return converter( _storage );
    }

private:
    boost::any _measure ;
    Epeck::Point_3 _storage ;
    storage_dimension_t _storageDimension ;
public:
    template<class Archive>
    void serialize( Archive& ar, const unsigned int /*version*/ ) {
        ar & _storage & _storageDimension ;
    }
};


}//SFCGAL

#endif
