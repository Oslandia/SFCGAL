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


#ifdef _MSC_VER
#  define _USE_MATH_DEFINES
#endif

#include <SFCGAL/detail/generator/disc.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>

#include <cmath>

namespace SFCGAL {
namespace generator {

///
///
///
std::auto_ptr< Polygon > disc(
    const Point& center,
    const double& radius,
    const unsigned int& nQuadrantSegments
)
{
    BOOST_ASSERT( nQuadrantSegments > 1 );

    std::auto_ptr< LineString > exteriorRing( new LineString() ) ;

    double dTheta = M_PI_4 / nQuadrantSegments ;

    for ( size_t i = 0; i < nQuadrantSegments * 4; i++ ) {
        double x = CGAL::to_double( center.x() ) + radius * cos(i*dTheta);
        double y = CGAL::to_double( center.y() ) + radius * sin(i*dTheta);
        exteriorRing->addPoint( new Point( x, y ) ) ;
    }

    exteriorRing->addPoint( exteriorRing->startPoint() ) ;

    std::auto_ptr< Polygon > result( new Polygon( exteriorRing.release() ) );
    result->forceValidityFlag(true);
    return result ;
}

} // namespace generator
} // namespace SFCGAL

