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

#include <SFCGAL/algorithm/collectionHomogenize.h>

#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>

namespace SFCGAL {
namespace algorithm {

// Use of unique_ptr :
// If nothing has to be built, g will be moved to the result without copy and new allocation.
// Otherwise, a new geometry is built and the old one is deleted
std::unique_ptr<Geometry> collectionHomogenize( std::unique_ptr<Geometry> g )
{
    // unknown type
    int common_type = 0;

    if ( ! g->is<GeometryCollection>() ) {
        // not a collection, nothing to do
        return g;
    }

    const GeometryCollection& coll = g->as<GeometryCollection>();

    // test if it is a singleton
    if ( coll.numGeometries() == 1 ) {
        return std::unique_ptr<Geometry>( coll.geometryN( 0 ).clone() );
    }

    for ( size_t i = 0; i < coll.numGeometries(); ++i ) {
        const Geometry& gi = coll.geometryN( i );

        if ( common_type == 0 ) {
            common_type = gi.geometryTypeId();
            continue;
        }

        if ( gi.geometryTypeId() != common_type ) {
            common_type = 0;
            break;
        }
    }

    if ( common_type == 0 ) {
        // not an homogeneous collection, give back
        return g;
    }

    GeometryCollection* ret_geo = 0;

    if ( common_type == TYPE_POINT ) {
        ret_geo = new MultiPoint;
    }
    else if ( common_type == TYPE_LINESTRING ) {
        ret_geo = new MultiLineString;
    }
    else if ( common_type == TYPE_POLYGON ) {
        ret_geo = new MultiPolygon;
    }
    else if ( common_type == TYPE_SOLID ) {
        ret_geo = new MultiSolid;
    }

    // copy each geometry
    for ( size_t i = 0; i < coll.numGeometries(); ++i ) {
        ret_geo->addGeometry( coll.geometryN( i ) );
    }

    return std::unique_ptr<Geometry>( ret_geo );
}

}
}
