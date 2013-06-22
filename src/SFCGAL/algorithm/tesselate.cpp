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
#include <SFCGAL/algorithm/tesselate.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/triangulate/triangulatePolygon.h>
#include <SFCGAL/algorithm/isValid.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
std::auto_ptr<Geometry> tesselate( const Geometry & g, NoValidityCheck )
{
	switch ( g.geometryTypeId() ) {
	case TYPE_POINT:
	case TYPE_LINESTRING:
	case TYPE_TRIANGLE:
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
		return std::auto_ptr<Geometry>( g.clone() );

	case TYPE_POLYGON:
	case TYPE_POLYHEDRALSURFACE: {
		TriangulatedSurface* triSurf = new TriangulatedSurface();
		triangulate::triangulatePolygon3D( g, *triSurf );
		return std::auto_ptr<Geometry>( triSurf );
	}
	case TYPE_SOLID: {
		std::auto_ptr<GeometryCollection> ret( new GeometryCollection );
		for ( size_t i = 0; i < g.as<Solid>().numShells(); ++i ) {
			ret->addGeometry( tesselate( g.as<Solid>().shellN(i) ).release() );
		}
		return std::auto_ptr<Geometry>( ret.release() );
    }
	// multipolygon and multisolid return a geometrycollection
	case TYPE_MULTIPOLYGON:
	case TYPE_MULTISOLID:
	case TYPE_GEOMETRYCOLLECTION: {
		std::auto_ptr<GeometryCollection> ret( new GeometryCollection );
		for ( size_t i = 0; i < g.numGeometries(); ++i ) {
			ret->addGeometry( tesselate( g.geometryN(i) ).release() );
		}
		return std::auto_ptr<Geometry>( ret.release() );
	}
	default:
		break;
	}
	return std::auto_ptr<Geometry>( g.clone() );
}

std::auto_ptr<Geometry> tesselate( const Geometry & g )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY( g );

    return tesselate( g, NoValidityCheck() );
}

} // namespace algorithm
} // namespace SFCGAL


