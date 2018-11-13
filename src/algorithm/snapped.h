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

#ifndef _SFCGAL_ALGORITHM_SNAPPED_H_
#define _SFCGAL_ALGORITHM_SNAPPED_H_

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>

#include <vector>

namespace SFCGAL {

namespace algorithm {

/**
 * @brief returns geometry with integer coordinates 
 * @ingroup public_api
 * @ note the geometry must have precision
 */

struct SnappedPoint
{
    SnappedPoint( const Point& p )
        : x( round( CGAL::to_double( p.x() ) / pow( 10, p.precisionXY() ) ) )
        , y( round( CGAL::to_double( p.y() ) / pow( 10, p.precisionXY() ) ) )
        , z( p.is3D() ? round( CGAL::to_double( p.z() / pow( 10, p.precisionZ() ) ) ) : 0 )
    {}
    long x;
    long y;
    long z;
};

inline
bool operator!=(const SnappedPoint& a, const SnappedPoint& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

struct SnappedLine: std::vector< SnappedPoint > 
{
    SnappedLine( const LineString& l ) : std::vector< SnappedPoint >( l.begin(), l.end() ){}
};

struct SnappedPolygon: std::vector< SnappedLine >
{
    SnappedPolygon( const Polygon& p ) : std::vector< SnappedLine >( p.begin(), p.end() ){}
};


}//algorithm
}//SFCGAL
#endif

