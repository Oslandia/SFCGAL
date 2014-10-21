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

#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
Solid::Solid()
{
    _shells.push_back( new PolyhedralSurface()  );
}

///
///
///
Solid::Solid( const PolyhedralSurface& exteriorShell )
{
    _shells.push_back( exteriorShell.clone() );
}

///
///
///
Solid::Solid( PolyhedralSurface* exteriorShell )
{
    _shells.push_back( exteriorShell );
}

///
///
///
Solid::Solid( const std::vector< PolyhedralSurface > & shells )
{
    if ( shells.empty() ) {
        _shells.resize( 1, new PolyhedralSurface() );
    }
    else {
        for ( size_t i = 0; i < shells.size(); i++ ) {
            _shells.push_back( shells[i].clone() ) ;
        }
    }
}

///
///
///
Solid::Solid( const Solid& other ):
    Geometry( other )
{
    for ( size_t i = 0; i < other.numShells(); i++ ) {
        _shells.push_back( other.shellN( i ).clone() );
    }
}

///
///
///
Solid& Solid::operator = ( Solid other )
{
    swap( other );
    return *this ;
}

///
///
///
Solid::~Solid()
{

}

///
///
///
Solid*   Solid::clone() const
{
    return new Solid( *this );
}




///
///
///
std::string  Solid::geometryType() const
{
    return "Solid" ;
}

///
///
///
GeometryType  Solid::geometryTypeId() const
{
    return TYPE_SOLID ;
}

///
///
///
int  Solid::dimension() const
{
    return 3 ;
}

///
///
///
int  Solid::coordinateDimension() const
{
    return exteriorShell().coordinateDimension() ;
}

///
///
///
bool  Solid::isEmpty() const
{
    return exteriorShell().isEmpty();
}

///
///
///
bool  Solid::is3D() const
{
    return exteriorShell().is3D();
}

///
///
///
bool  Solid::isMeasured() const
{
    return exteriorShell().isMeasured();
}

///
///
///
void Solid::accept( GeometryVisitor& visitor )
{
    return visitor.visit( *this );
}

///
///
///
void Solid::accept( ConstGeometryVisitor& visitor ) const
{
    return visitor.visit( *this );
}

}//SFCGAL



