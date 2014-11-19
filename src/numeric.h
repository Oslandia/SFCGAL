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

#ifndef _SFCGAL_NUMERIC_H_
#define _SFCGAL_NUMERIC_H_

#include <limits>
#include <cmath>

#include <SFCGAL/export.h>

#include <SFCGAL/Kernel.h>

namespace SFCGAL {
/**
 * shortcut to get NaN for double
 */
inline double NaN()
{
    return std::numeric_limits< double >::quiet_NaN();
}
/**
 * shortcut to test NaN for double
 */
inline bool   isNaN( const double& value )
{
    return value != value ;
}

/**
 * @brief round a double to the nearest integer
 */
inline double round( const double& v )
{
    if ( v < 0.0 ) {
        return ::ceil( v - 0.5 ) ;
    }
    else {
        return ::floor( v + 0.5 ) ;
    }
}


/**
 * @brief floor a rational to an integer
 */
SFCGAL_API CGAL::Gmpz floor( const CGAL::Gmpq& v ) ;
/**
 * @brief ceil a rational to an integer
 */
SFCGAL_API CGAL::Gmpz ceil( const CGAL::Gmpq& v ) ;
/**
 * @brief round a rational to an integer
 */
SFCGAL_API CGAL::Gmpz round( const CGAL::Gmpq& v ) ;

}//SFCGAL

#endif
