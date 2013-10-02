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

#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
MultiPolygon::MultiPolygon():
    GeometryCollection()
{

}

///
///
///
MultiPolygon::MultiPolygon( MultiPolygon const& other ):
    GeometryCollection( other )
{

}

///
///
///
MultiPolygon& MultiPolygon::operator = ( MultiPolygon other )
{
    swap( other ) ;
    return *this ;
}

///
///
///
MultiPolygon::~MultiPolygon()
{

}

///
///
///
MultiPolygon*    MultiPolygon::clone() const
{
    return new MultiPolygon( *this );
}

///
///
///
std::string    MultiPolygon::geometryType() const
{
    return "MultiPolygon" ;
}

///
///
///
GeometryType   MultiPolygon::geometryTypeId() const
{
    return TYPE_MULTIPOLYGON ;
}

///
///
///
bool           MultiPolygon::isAllowed( Geometry const& g )
{
    return g.geometryTypeId() == TYPE_POLYGON ;
}

///
///
///
void MultiPolygon::accept( GeometryVisitor& visitor )
{
    return visitor.visit( *this );
}

///
///
///
void MultiPolygon::accept( ConstGeometryVisitor& visitor ) const
{
    return visitor.visit( *this );
}

}//SFCGAL

