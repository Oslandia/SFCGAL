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

#include <SFCGAL/detail/generator/sierpinski.h>

#include <SFCGAL/Triangle.h>
#include <SFCGAL/MultiPolygon.h>

namespace SFCGAL {
namespace generator {


std::vector< Kernel::Triangle_2 > _sierpinski( const std::vector< Kernel::Triangle_2 >& triangles )
{
    std::vector< Kernel::Triangle_2 > result ;
    result.reserve( triangles.size() * 3 ) ;

    for ( size_t i = 0; i < triangles.size(); i++ ) {
        const Kernel::Point_2& a = triangles[i].vertex( 0 ) ;
        const Kernel::Point_2& b = triangles[i].vertex( 1 ) ;
        const Kernel::Point_2& c = triangles[i].vertex( 2 ) ;

        Kernel::Point_2 iAB = a + ( b - a ) / 2 ;
        Kernel::Point_2 iBC = b + ( c - b ) / 2 ;
        Kernel::Point_2 iCA = c + ( a - c ) / 2 ;

        result.push_back( Kernel::Triangle_2( a, iAB, iCA ) );
        result.push_back( Kernel::Triangle_2( b, iBC, iAB ) );
        result.push_back( Kernel::Triangle_2( c, iCA, iBC ) );
    }

    return result ;
}

///
///
///
std::auto_ptr< MultiPolygon > sierpinski( const unsigned int& order )
{
    std::vector< Kernel::Triangle_2 > triangles ;
    triangles.push_back( Kernel::Triangle_2(
                             Kernel::Point_2( 1.0, sqrt( 3.0 ) ),
                             Kernel::Point_2( 2.0, 0.0 ),
                             Kernel::Point_2( 0.0, 0.0 )
                         ) );

    for ( unsigned int k = 0 ; k < order; k++ ) {
        triangles = _sierpinski( triangles );
    }

    std::auto_ptr< MultiPolygon > result( new MultiPolygon ) ;

    for ( size_t i = 0; i < triangles.size(); i++ ) {
        result->addGeometry( Triangle( triangles[i] ).toPolygon() ) ;
    }

    return result ;
}

} // namespace generator
} // namespace SFCGAL


