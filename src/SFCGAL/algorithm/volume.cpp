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


#include <SFCGAL/algorithm/volume.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/algorithm/tesselate.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/Solid.h>

namespace SFCGAL {
namespace algorithm {

const Kernel::FT volume( const Solid& solid, NoValidityCheck )
{
    Kernel::FT vol = 0;
    const CGAL::Point_3<Kernel> origin( 0,0,0 );
    const size_t numShells = solid.numShells();

    for ( size_t i=0; i<numShells; i++ ) {
        std::auto_ptr<Geometry> t( tesselate( solid.shellN( i ), NoValidityCheck() ) );
        const TriangulatedSurface& tin = t->as<TriangulatedSurface>();
        const size_t numTriangles = tin.numTriangles();

        for ( size_t j=0; j<numTriangles; j++ ) {
            const Triangle& tri = tin.triangleN( j );
            vol = vol + CGAL::volume( origin, tri.vertex( 0 ).toPoint_3(),
                                      tri.vertex( 1 ).toPoint_3(),
                                      tri.vertex( 2 ).toPoint_3() );
        }
    }

    return vol;
}

const Kernel::FT volume( const Geometry& g )
{
    if ( g.isEmpty() ) {
        return 0;
    }

    SFCGAL_ASSERT_GEOMETRY_VALIDITY( g );

    switch ( g.geometryTypeId() ) {
    case TYPE_POINT:
    case TYPE_LINESTRING:
    case TYPE_POLYGON:
    case TYPE_TRIANGLE:
    case TYPE_MULTIPOINT:
    case TYPE_MULTILINESTRING:
    case TYPE_MULTIPOLYGON:
    case TYPE_TRIANGULATEDSURFACE:
    case TYPE_POLYHEDRALSURFACE:
        return 0;
    case TYPE_SOLID:
        return volume( g.as<Solid>(), NoValidityCheck() );
    case TYPE_MULTISOLID:
    case TYPE_GEOMETRYCOLLECTION:
        Kernel::FT v=0;
        const GeometryCollection& c = g.as<GeometryCollection>();

        for ( size_t i=0; i<c.numGeometries(); i++ ) {
            if ( c.geometryN( i ).is<Solid>() ) {
                v = v + volume( c.geometryN( i ).as<Solid>(), NoValidityCheck() );
            }
        }

        return v;
    }

    BOOST_THROW_EXCEPTION( Exception(
                               ( boost::format( "volume( %s ) is not defined" ) % g.geometryType() ).str()
                           ) );
    return 0; // to avoid warning
}

}
}
