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
#include <SFCGAL/PreparedGeometry.h>

#include <SFCGAL/detail/io/WktWriter.h>

namespace SFCGAL {
PreparedGeometry::PreparedGeometry() :
    _srid( 0 )
{
}

PreparedGeometry::PreparedGeometry( std::auto_ptr<Geometry> geometry, srid_t srid ) :
    _geometry( geometry ),
    _srid( srid )
{
}

PreparedGeometry::PreparedGeometry( Geometry* geometry, srid_t srid ) :
    _geometry( geometry ),
    _srid( srid )
{
}

PreparedGeometry::~PreparedGeometry()
{
}

const Geometry& PreparedGeometry::geometry() const
{
    BOOST_ASSERT( _geometry.get() );
    return *_geometry;
}

Geometry& PreparedGeometry::geometry()
{
    BOOST_ASSERT( _geometry.get() );
    return *_geometry;
}

void PreparedGeometry::resetGeometry( Geometry* geom )
{
    _geometry.reset( geom );
    invalidateCache();
}

const Envelope& PreparedGeometry::envelope() const
{
    if ( ! _envelope ) {
        _envelope.reset( _geometry->envelope() );
    }

    return *_envelope;
}

void PreparedGeometry::invalidateCache()
{
    _envelope.reset();
}

std::string PreparedGeometry::asEWKT( const int& numDecimals ) const
{
    std::ostringstream oss;

    if ( numDecimals >= 0 ) {
        oss << std::fixed ;
        oss.precision( numDecimals );
    }

    if ( _srid != 0 ) {
        oss << "SRID=" << _srid << ";";
    }

    detail::io::WktWriter writer( oss );
    bool exactWrite = false;

    if ( numDecimals == -1 ) {
        exactWrite = true;
    }

    writer.write( *_geometry, exactWrite );
    return oss.str();
}
}
