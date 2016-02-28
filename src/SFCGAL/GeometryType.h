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
#ifndef _SFCGAL_GEOMETRYTYPE_H_
#define _SFCGAL_GEOMETRYTYPE_H_

namespace SFCGAL {

/**
 * [OGC/SFA]8.2.3 "A common list of codes for geometric types"
 *
 * @todo solid and triangles as non OGC/SFA geometric types?
 * @warning codes for abstract classes and unimplemented classes are hidden
 * @warning code values have are important for WKB
 *
 * @ingroup public_api
 */
typedef enum {
//      TYPE_GEOMETRY            = 0, //abstract
    TYPE_POINT               = 1,
    TYPE_LINESTRING          = 2,
    TYPE_POLYGON             = 3,
    TYPE_MULTIPOINT          = 4,
    TYPE_MULTILINESTRING     = 5,
    TYPE_MULTIPOLYGON        = 6,
    TYPE_GEOMETRYCOLLECTION  = 7,
//     TYPE_CIRCULARSTRING      = 8, // not yet supported
//     TYPE_COMPOUNDCURVE       = 9, // not yet supported
//     TYPE_CURVEPOLYGON        = 10, // not yet supported
//     TYPE_MULTICURVE          = 11, //abstract
//     TYPE_MULTISURFACE        = 12, //abstract
//     TYPE_CURVE               = 13, //abstract
//     TYPE_SURFACE             = 14, //abstract
    TYPE_POLYHEDRALSURFACE   = 15,
    TYPE_TRIANGULATEDSURFACE = 16,

    //-- not official codes
    TYPE_TRIANGLE            = 100, //17 in Wikipedia???
    TYPE_SOLID               = 101,
    TYPE_MULTISOLID          = 102
} GeometryType ;

} // SFCGAL

#endif

