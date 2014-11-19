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

#include <SFCGAL/detail/transform/ForceZOrderPoints.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/algorithm/orientation.h>

namespace SFCGAL {
namespace transform {

///
///
///
ForceZOrderPoints::ForceZOrderPoints( const Kernel::FT& defaultZ ):
    _defaultZ( defaultZ )
{

}

///
///
///
void ForceZOrderPoints::transform( Point& p )
{
    if ( ! p.is3D() ) {
        p = Point( p.x(), p.y(), _defaultZ );
    }
}


///
///
///
void ForceZOrderPoints::visit( Triangle& t )
{
    if ( ! t.is3D() ) {
        if ( ! algorithm::isCounterClockWiseOriented( t ) ) {
            // not pointing up, reverse
            t.reverse();
        }

        Transform::visit( t );
    }

}

void ForceZOrderPoints::visit( Polygon& p )
{
    if ( ! p.is3D() ) {
        LineString& ext = p.exteriorRing();

        if ( ! algorithm::isCounterClockWiseOriented( p.exteriorRing() ) ) {
            // exterior ring not pointing up, reverse
            ext.reverse();
        }

        for ( size_t i = 0; i < p.numInteriorRings(); ++i ) {
            LineString& inter = p.interiorRingN( i );

            if ( algorithm::isCounterClockWiseOriented( inter ) ) {
                // interior ring is pointing up, reverse
                inter.reverse();
            }
        }

        Transform::visit( p );
    }

}

}//transform
}//SFCGAL



