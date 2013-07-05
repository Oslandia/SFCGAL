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
#ifndef _SFCGAL_DETAIL_INTERVAL_H_
#define _SFCGAL_DETAIL_INTERVAL_H_

#include <SFCGAL/config.h>

namespace SFCGAL {
namespace detail {

/**
 * Represents an interval
 */
class SFCGAL_API Interval {
public:
    /**
     * default constructor (empty interval)
     */
    Interval() ;
    /**
     * collapsed interval constructor
     */
    Interval( const double& value ) ;
    /**
     * constructor with two values
     */
    Interval( const double& v1, const double& v2 ) ;
    /**
     * copy constructor
     */
    Interval( const Interval& other ) ;
    /**
     * assign operator
     */
    Interval& operator = ( const Interval& other ) ;
    /**
     * destructor
     */
    ~Interval() ;

    /**
     * indicates if the interval is empty
     */
    bool isEmpty() const ;


    /**
     * returns the lower value
     */
    inline const double& lower() const {
        return _lower;
    }
    /**
     * returns the upper value
     */
    inline const double& upper() const {
        return _upper;
    }
    /**
     * returns the with of the interval
     */
    inline double         width() const {
        return _upper - _lower ;
    }

    /**
     * expand the interval
     *
     * @warning no effect if isEmpty()
     */
    void                  expandBy( const double& d ) ;
    /**
     * expand the interval to include an other interval.
     *
     * @warning no effect if other.isEmpty()
     */
    void                  expandToInclude( const Interval& other ) ;
    /**
     * expand the interval to include a value
     *
     * @warning no effect if value is NaN
     */
    void                  expandToInclude( const double& value ) ;


    /**
     * test if this intersects other
     */
    bool                  intersects( const Interval& other ) const  ;


    /**
     * compare two intervals
     * @warning true for empty intervals
     */
    bool operator == ( const Interval& other ) const ;
    /**
     * compare two intervals
     * @warning false for empty intervals
     */
    bool operator != ( const Interval& other ) const ;

private:
    double _lower ;
    double _upper ;
};

}//detail
}//SFCGAL

#endif
