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

#include <SFCGAL/Coordinate.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Exception.h>
#include <SFCGAL/numeric.h>

namespace SFCGAL {

///
///
///
Coordinate::Coordinate():
    _measure(),
    _storage(0,0,0),
    _storageDimension(0)
{
}

///
///
///
Coordinate::Coordinate( const double& x, const double& y )
{
    if ( !std::isfinite( x ) || !std::isfinite( y ) ) {
        BOOST_THROW_EXCEPTION( NonFiniteValueException( "cannot create coordinate with non finite value" ) );
    }
    _storage = Epeck::Point_3( x, y, 0 );
    _storageDimension = 2;
}

///
///
///
Coordinate::Coordinate( const double& x, const double& y, const double& z )
{
    if ( !std::isfinite( x ) || !std::isfinite( y ) || !std::isfinite( z ) ) {
        BOOST_THROW_EXCEPTION( NonFiniteValueException( "cannot create coordinate with non finite value" ) );
    }
    _storage = Epeck::Point_3( x, y, z );
    _storageDimension = 3;
}

///
///
///
Coordinate::Coordinate( const Epeck::FT& x, const Epeck::FT& y ):
    _measure(),
    _storage(x,y,0),
    _storageDimension(2)
{

}

///
///
///
Coordinate::Coordinate( const Epeck::FT& x, const Epeck::FT& y, const Epeck::FT& z ):
    _measure(),
    _storage(x,y,z),
    _storageDimension(3)
{

}


///
///
///
Coordinate::Coordinate( const Coordinate& other ):
    _measure(other._measure),
    _storage( other._storage ),
    _storageDimension(other._storageDimension)
{

}

///
///
///
Coordinate& Coordinate::operator = ( const Coordinate& other )
{
    _measure = other._measure ;
    _storage = other._storage;
    _storageDimension = other._storageDimension;
    return *this ;
}

///
///
///
Coordinate::~Coordinate()
{

}


// TODO externalize
Epeck::FT _roundFT( const Epeck::FT& v, const long& scaleFactor ) 
{
    return Epeck::FT( CGAL::Gmpq(
        SFCGAL::round( v.exact() * scaleFactor ),
        scaleFactor
    ) ) ;
}

// TODO externalize
Coordinate& Coordinate::round( const long& scaleFactor )
{
    _storage = Epeck::Point_3(
        _roundFT( _storage.x(), scaleFactor ),
        _roundFT( _storage.y(), scaleFactor ),
        _roundFT( _storage.z(), scaleFactor )
    );
    return *this;
}



//----------------------

///
///
///
bool Coordinate::operator < ( const Coordinate& other ) const
{
    // comparison along x
    if ( x() < other.x() ) {
        return true ;
    }
    else if ( other.x() < x() ) {
        return false;
    }

    // comparison along y
    if ( y() < other.y() ) {
        return true ;
    }
    else if ( other.y() < y() ) {
        return false;
    }

    // comparison along z
    if ( z() < other.z() ) {
        return true ;
    }
    else if ( other.z() < z() ) {
        return false;
    }

    // points are equals
    return false;
}

///
///
///
bool Coordinate::operator == ( const Coordinate& other ) const
{
    return x() == other.x() && y() == other.y()  && z() == other.z() ;
}

///
///
///
bool Coordinate::operator != ( const Coordinate& other ) const
{
    return ! ( *this == other );
}


}//SFCGAL
