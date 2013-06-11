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

const Validity isValid( const LineString & l, const double & toleranceAbs )
{
    BOOST_ASSERT( !l.isEmpty() );
    return length3D( l ) > toleranceAbs ? Validity::valid() : Validity::invalid("LineString has no length");
}

const Validity isValid( const Polygon & p, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_ASSERT( !p.isEmpty() );
    // Au moins 4 points par ring (couvert par simplicité, mais à peut-être à conserver car pas cher à tester)
    // Surface d'un ring non nulle (couvert par simplicité)
    // Rings fermants
    // Rings simple (Pas d'adjacence d'un ring avec lui même et pas d'intersection)
    // Rings correctement orientés
    // Si ring(s) interne(s) pas plus d'un point de contact entre ring externe et ring interne
    // Si ring(s) interne(s) chaque ring interne doit etre strictement interne au ring externe
    // Si rings internes, pas plus de 1 point de contact entre chaque ring interne

    // question: si 3 rings internes avec chacune un point de contact avec 2 autres, est-ce valide ? Non, l'intérieur doit être connecté.

    // Closed simple rings
    const Polygon::const_iterator end = p.end();
    for ( Polygon::const_iterator r=p.begin(); r!=end; ++r ){
        if ( r->numPoints() < 4 ) return Validity::invalid("not enought points in Plygon ring");
        
    } 

    


    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return Validity::valid();
}

const Validity isValid( const Triangle & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return Validity::valid();
}

const Validity isValid( const Solid & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return Validity::valid();
}

const Validity isValid( const MultiLineString & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return Validity::valid();
}

const Validity isValid( const MultiPolygon & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return Validity::valid();
}

const Validity isValid( const MultiSolid & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return Validity::valid();
}

const Validity isValid( const GeometryCollection & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return Validity::valid();
}

const Validity isValid( const TriangulatedSurface & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return Validity::valid();
}

const Validity isValid( const PolyhedralSurface & l, const double & toleranceAbs, const double & toleranceRel )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return Validity::valid();
}


const Validity isValid( const Geometry& g )
{
	if ( g.isEmpty() ) return Validity::valid();

    const double toleranceAbs = 1e-9;
    const double toleranceRel = 1e-4;

	switch ( g.geometryTypeId() ){
        case TYPE_POINT:              return Validity::valid();
		case TYPE_LINESTRING:         return isValid( g.as< LineString >(),          toleranceAbs ) ;
		case TYPE_POLYGON:            return isValid( g.as< Polygon >(),             toleranceAbs, toleranceRel ) ;
		case TYPE_TRIANGLE:           return isValid( g.as< Triangle >(),            toleranceAbs, toleranceRel ) ;
		case TYPE_SOLID:              return isValid( g.as< Solid >(),               toleranceAbs, toleranceRel ) ;
        case TYPE_MULTIPOINT:         return Validity::valid();
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
    return Validity::invalid(( boost::format("isValid( %s ) is not defined") % g.geometryType() ).str()); // to avoid warning
}
} // namespace algorithm
} // namespace SFCGAL
