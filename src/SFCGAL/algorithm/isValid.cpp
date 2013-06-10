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
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/algorithm/length.h>
#include <SFCGAL/all.h>

namespace SFCGAL {
namespace algorithm {

/** @note empty geometries are valid, but the test is only performed in the interface function
 * in individual functions for implementation, an assertion !empty is present for this reason
 */

bool isValid( const LineString & l, const double & toleranceAbs )
{
    BOOST_ASSERT( !l.isEmpty() );
    return length3D( l ) > toleranceAbs;
}

bool isValid( const Polygon & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}

bool isValid( const Triangle & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}

bool isValid( const Solid & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}

bool isValid( const MultiLineString & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}

bool isValid( const MultiPolygon & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}

bool isValid( const MultiSolid & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}

bool isValid( const GeometryCollection & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}

bool isValid( const TriangulatedSurface & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}

bool isValid( const PolyhedralSurface & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}


bool isValid( const Geometry& g )
{
	if ( g.isEmpty() ) return true;

    const double toleranceAbs = 1e-9;
    const double toleranceRel = 1e-4;

	switch ( g.geometryTypeId() ){
		case TYPE_POINT:              return true;
		case TYPE_LINESTRING:         return isValid( g.as< LineString >(),          toleranceAbs ) ;
		case TYPE_POLYGON:            return isValid( g.as< Polygon >(),             toleranceAbs, toleranceRel ) ;
		case TYPE_TRIANGLE:           return isValid( g.as< Triangle >(),            toleranceAbs, toleranceRel ) ;
		case TYPE_SOLID:              return isValid( g.as< Solid >(),               toleranceAbs, toleranceRel ) ;
		case TYPE_MULTIPOINT:         return true;
		case TYPE_MULTILINESTRING:    return isValid( g.as< MultiLineString >(),     toleranceAbs, toleranceRel ) ;
		case TYPE_MULTIPOLYGON:       return isValid( g.as< MultiPolygon >(),        toleranceAbs, toleranceRel ) ;
		case TYPE_MULTISOLID:         return isValid( g.as< MultiSolid >(),          toleranceAbs, toleranceRel ) ;
		case TYPE_GEOMETRYCOLLECTION: return isValid( g.as< GeometryCollection >(),  toleranceAbs, toleranceRel ) ;
		case TYPE_TRIANGULATEDSURFACE:return isValid( g.as< TriangulatedSurface >(), toleranceAbs, toleranceRel ) ;
		case TYPE_POLYHEDRALSURFACE:  return isValid( g.as< PolyhedralSurface >(),   toleranceAbs, toleranceRel ) ;
	}
	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("isValid( %s ) is not defined") % g.geometryType() ).str()
	));
    return false; // to avoid warning
}
} // namespace algorithm
} // namespace SFCGAL
