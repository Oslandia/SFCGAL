/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef SFCGAL_COLLECTION_EXTRACT_ALGORITHM
#define SFCGAL_COLLECTION_EXTRACT_ALGORITHM

#include <SFCGAL/config.h>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/GeometryCollection.h>

namespace SFCGAL {
namespace algorithm {
/*
 * Given a geometry collection
 * returns a MultiPolygon from triangles, polygons, polyhedral and polygons
 *
 * @warning Ownership is taken from the parameter
 * @ingroup detail
 */
SFCGAL_API std::auto_ptr<Geometry> collectionExtractPolygons( std::auto_ptr<Geometry> coll );
}
}

#endif
