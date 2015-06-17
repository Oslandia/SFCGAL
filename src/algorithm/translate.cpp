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

#include <SFCGAL/algorithm/translate.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>

#include <SFCGAL/detail/transform/AffineTransform3.h>
#include <SFCGAL/detail/transform/AffineTransform2.h>


namespace SFCGAL {
namespace algorithm {

///
///
///
void       translate( Geometry& g, const Kernel::Vector_3& v )
{
    transform::AffineTransform3 visitor(
        CGAL::Aff_transformation_3< Kernel >( CGAL::TRANSLATION, v )
    );
    g.accept( visitor ) ;
}

///
///
///
void       translate( Geometry& g, const Kernel::Vector_2& v )
{
    transform::AffineTransform2 visitor(
        CGAL::Aff_transformation_2< Kernel >( CGAL::TRANSLATION, v )
    );
    g.accept( visitor ) ;
}

///
///
///
void   translate( Geometry& g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
    translate( g, Kernel::Vector_3( dx,dy,dz ) );
}

} // namespace algorithm
} // namespace SFCGAL


