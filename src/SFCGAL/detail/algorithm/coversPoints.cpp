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


#include <SFCGAL/detail/algorithm/coversPoints.h>
#include <SFCGAL/Geometry.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/detail/GetPointsVisitor.h>

namespace SFCGAL {
namespace detail {
namespace algorithm {
template <int Dim>
bool _coversPoints( const Geometry& ga, const Geometry& gb )
{
    if ( ga.isEmpty() || gb.isEmpty() ) {
        return false;
    }

    GeometrySet<Dim> gsa( ga );

    // get all points of gb;
    detail::GetPointsVisitor visitor;
    gb.accept( visitor );

    for ( detail::GetPointsVisitor::const_iterator it = visitor.points.begin(); it != visitor.points.end(); ++it ) {
        const Point* ppt = *it;

        // a geometry set of one point
        GeometrySet<Dim> gsp( *ppt );

        if ( !SFCGAL::algorithm::intersects( gsp, gsa ) ) {
            return false;
        }
    }

    return true;
}

bool coversPoints( const Geometry& ga, const Geometry& gb )
{
    return _coversPoints<2>( ga, gb );
}

bool coversPoints3D( const Geometry& ga, const Geometry& gb )
{
    return _coversPoints<3>( ga, gb );
}
}
}
}
