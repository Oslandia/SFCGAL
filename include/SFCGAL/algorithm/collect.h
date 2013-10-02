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

#ifndef SFCGAL_COLLECT_ALGORITHM
#define SFCGAL_COLLECT_ALGORITHM

#include <SFCGAL/config.h>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {
namespace algorithm {
/**
 * Returns an aggregate of ga and gb
 * @ingroup detail
 */
SFCGAL_API std::auto_ptr<Geometry> collect( const Geometry& ga, const Geometry& gb );

/**
 * Returns an aggregate of a list of geometries
 * @ingroup detail
 */
template <typename GeometryIterator>
std::auto_ptr<Geometry> collect( GeometryIterator begin, GeometryIterator end )
{
    GeometryIterator it;
    // FIXME: optimize type. For instance, if all the given geometries are points, return a MultiPoint instead of a GeometryCollection
    GeometryCollection* coll = new GeometryCollection();

    for ( it = begin; it != end; ++it ) {
        coll->addGeometry( *it );
    }

    return std::auto_ptr<Geometry>( coll );
}
}
}

#endif
