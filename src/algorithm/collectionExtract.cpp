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

#include <SFCGAL/algorithm/collectionExtract.h>

#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>

#include <SFCGAL/detail/transform/ForceZ.h>

namespace SFCGAL {
namespace algorithm {

// Use of unique_ptr :
// If nothing has to be built, g will be moved to the result without copy and new allocation.
// Otherwise, a new geometry is built and the old one is deleted
std::unique_ptr<Geometry> collectionExtractPolygons( std::unique_ptr<Geometry> g )
{
    if ( ! g->is<GeometryCollection>() ) {
        // not a collection, nothing to do
        return g;
    }

    const GeometryCollection& coll = g->as<GeometryCollection>();

    // if it is empty, do not do anything
    if ( coll.isEmpty() ) {
        return g;
    }

    MultiPolygon* ret_geo = new MultiPolygon;

    // copy each geometry
    for ( size_t i = 0; i < coll.numGeometries(); ++i ) {

        Geometry* gi = coll.geometryN( i ).clone();

        switch ( gi->geometryTypeId() ) {
        case TYPE_TRIANGLE:
            ret_geo->addGeometry( Polygon( gi->as<Triangle>() ) );
            break;

        case TYPE_TRIANGULATEDSURFACE: {
            for ( size_t j = 0; j < gi->numGeometries(); ++j ) {
                ret_geo->addGeometry( Polygon( gi->geometryN( j ).as<Triangle>() ) );
            }
        }
        break;

        case TYPE_POLYHEDRALSURFACE: {
            for ( size_t j = 0; j < gi->numGeometries(); ++j ) {
                ret_geo->addGeometry( gi->geometryN( j ) );
            }
        }
        break;

        case TYPE_POLYGON:
            ret_geo->addGeometry( *gi );
            break;

        default:
            // nothing
            break;
        }
    }

    return std::unique_ptr<Geometry>( ret_geo );
}

}
}
