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

#include <SFCGAL/detail/io/Serialization.h>

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

namespace SFCGAL {
namespace io {

BinarySerializer::BinarySerializer( std::ostream& ostr ) : boost::archive::binary_oarchive( ostr )
{
    using namespace SFCGAL;
    register_type< Point >();
    register_type< LineString >();
    register_type< Triangle >();
    register_type< Polygon >();
    register_type< TriangulatedSurface >();
    register_type< PolyhedralSurface >();
    register_type< Solid >();
    register_type< GeometryCollection >();
    register_type< MultiPoint >();
    register_type< MultiLineString >();
    register_type< MultiPolygon >();
    register_type< MultiSolid >();
}

BinaryUnserializer::BinaryUnserializer( std::istream& ostr ) : boost::archive::binary_iarchive( ostr )
{
    using namespace SFCGAL;
    register_type< Point >();
    register_type< LineString >();
    register_type< Triangle >();
    register_type< Polygon >();
    register_type< TriangulatedSurface >();
    register_type< PolyhedralSurface >();
    register_type< Solid >();
    register_type< GeometryCollection >();
    register_type< MultiPoint >();
    register_type< MultiLineString >();
    register_type< MultiPolygon >();
    register_type< MultiSolid >();
}

///
///
///
std::string writeBinaryGeometry( const Geometry& g )
{
    std::ostringstream ostr;
    BinarySerializer arc( ostr );
    // use the pointer version to force dynamic type writing
    const Geometry* pg = &g;
    arc << pg;
    return ostr.str();
}

///
///
///
std::string writeBinaryPrepared( const PreparedGeometry& g )
{
    std::ostringstream ostr;
    BinarySerializer arc( ostr );
    const PreparedGeometry* pg = &g;
    arc << pg;
    return ostr.str();
}

///
///
///
std::auto_ptr<Geometry> readBinaryGeometry( const std::string& str )
{
    std::istringstream istr( str );
    BinaryUnserializer iarc( istr );
    Geometry* g;
    iarc >> g;
    return std::auto_ptr<Geometry>( g );
}

///
///
///
std::auto_ptr<PreparedGeometry> readBinaryPrepared( const std::string& str )
{
    std::istringstream istr( str );
    BinaryUnserializer iarc( istr );
    PreparedGeometry* pg;
    iarc >> pg;
    return std::auto_ptr<PreparedGeometry>( pg );
}
}
}
namespace boost {
namespace serialization {

void save( boost::archive::text_oarchive& ar, const CGAL::Gmpz& z, const unsigned int /*version*/ )
{
    std::ostringstream ostr;
    ostr << z;
    std::string str = ostr.str();
    ar << str;
}

// specialization for binary archives
void save ( boost::archive::binary_oarchive& ar, const CGAL::Gmpz& z, const unsigned int/* version*/ )
{
    const mpz_t& mpz = z.mpz();
    int32_t size = mpz->_mp_size;
    ar& size;
    uint32_t rsize = size >= 0 ? size : -size;

    for ( uint32_t i = 0; i < rsize; ++i ) {
        ar& mpz->_mp_d[i];
    }
}


void load( boost::archive::text_iarchive& ar, CGAL::Gmpz& z, const unsigned int /*version*/ )
{
    std::string line;
    ar >> line;
    std::istringstream istr( line );
    istr >> z;
}

void load( boost::archive::binary_iarchive& ar, CGAL::Gmpz& z, const unsigned int /*version*/ )
{
    int32_t size;
    uint32_t rsize;
    mpz_t& mpz = z.mpz();
    ar& size;
    rsize = size >= 0 ? size : -size;
    mpz->_mp_size = size;
    _mpz_realloc( mpz, rsize );
    uint32_t i;

    for ( i = 0; i < rsize; ++i ) {
        ar& mpz->_mp_d[i];
    }
}

}
}
