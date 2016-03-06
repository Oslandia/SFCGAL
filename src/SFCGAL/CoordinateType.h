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
#ifndef _SFCGAL_COORDINATETYPE_H_
#define _SFCGAL_COORDINATETYPE_H_

namespace SFCGAL {

/**
 * @brief coordinate types (XY, XYZ, XYM, etc.)
 * @see SFA 2.8.3 LineStringZ = 1003 ( coordinateType + geometryType)
 * @ingroup public_api
 */
typedef enum {
    COORDINATE_XY    = 0 ,
    COORDINATE_XYZ   = 1000,
    COORDINATE_XYM   = 2000,
    COORDINATE_XYZM  = 3000
} CoordinateType ;

} // SFCGAL

#endif

