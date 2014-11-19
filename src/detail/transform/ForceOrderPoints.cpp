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

#include <SFCGAL/detail/transform/ForceOrderPoints.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/algorithm/orientation.h>

namespace SFCGAL {
namespace transform {

///
///
///
ForceOrderPoints::ForceOrderPoints( bool orientCCW ) :
    _orientCCW( orientCCW )
{

}

///
///
///
void ForceOrderPoints::transform( Point& )
{
}


///
///
///
void ForceOrderPoints::visit( Triangle& t )
{
    if ( ! t.is3D() ) {
        if ( ! algorithm::isCounterClockWiseOriented( t ) ) {
            // not pointing up, reverse
            if ( _orientCCW ) {
                t.reverse();
            }
        }
        else {
            if ( ! _orientCCW ) {
                t.reverse();
            }
        }

        Transform::visit( t );
    }

}

void ForceOrderPoints::visit( Polygon& p )
{
    if ( ! p.is3D() ) {
        LineString& ext = p.exteriorRing();

        if ( ! algorithm::isCounterClockWiseOriented( p.exteriorRing() ) ) {
            // exterior ring not pointing up, reverse
            if ( _orientCCW ) {
                ext.reverse();
            }
        }
        else {
            if ( ! _orientCCW ) {
                ext.reverse();
            }
        }

        for ( size_t i = 0; i < p.numInteriorRings(); ++i ) {
            LineString inter = p.interiorRingN( i );

            if ( algorithm::isCounterClockWiseOriented( inter ) ) {
                // interior ring is pointing up, reverse
                if ( _orientCCW ) {
                    inter.reverse();
                }
            }
            else {
                if ( ! _orientCCW ) {
                    inter.reverse();
                }
            }
        }

        Transform::visit( p );
    }

}

}//transform
}//SFCGAL



