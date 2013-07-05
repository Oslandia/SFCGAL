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
#include <SFCGAL/Point.h>
#include <SFCGAL/GeometryVisitor.h>

#include <SFCGAL/Exception.h>

using namespace SFCGAL::detail;

namespace SFCGAL {

///
///
///
Point::Point():
    Geometry(),
    _coordinate(),
    _m( NaN() )
{
}

///
///
///
Point::Point( const Coordinate& coordinate ):
    _coordinate( coordinate ),
    _m( NaN() )
{

}

///
///
///
Point::Point( const Kernel::FT& x, const Kernel::FT& y ):
    _coordinate( x,y ),
    _m( NaN() )
{

}

///
///
///
Point::Point( const Kernel::FT& x, const Kernel::FT& y, const Kernel::FT& z, const double& m ):
    _coordinate( x,y,z ),
    _m( m )
{

}

///
///
///
Point::Point( const double& x, const double& y ):
    Geometry(),
    _coordinate( x,y ),
    _m( NaN() )
{

}

///
///
///
Point::Point( const double& x, const double& y, const double& z ):
    Geometry(),
    _coordinate( x,y,z ),
    _m( NaN() )
{

}

///
///
///
Point::Point( const double& x, const double& y, const double& z, const double& m ):
    Geometry(),
    _coordinate( x,y,z ),
    _m( m )
{

}

///
///
///
Point::Point( const Kernel::Point_2& other ):
    _coordinate( other ),
    _m( NaN() )
{

}

///
///
///
Point::Point( const Kernel::Point_3& other ):
    _coordinate( other ),
    _m( NaN() )
{

}


///
///
///
Point::Point( const Point& other ):
    Geometry(),
    _coordinate( other._coordinate ),
    _m( other._m )
{

}

///
///
///
Point& Point::operator = ( const Point& other )
{
    _coordinate = other._coordinate ;
    _m          = other._m ;
    return *this ;
}

///
///
///
Point::~Point()
{

}


///
///
///
Point* Point::clone() const
{
    return new Point( *this );
}

///
///
///
std::string Point::geometryType() const
{
    return "Point";
}

///
///
///
GeometryType Point::geometryTypeId() const
{
    return TYPE_POINT ;
}

///
///
///
int Point::dimension() const
{
    return 0 ;
}

///
///
///
int Point::coordinateDimension() const
{
    return _coordinate.coordinateDimension() + ( isMeasured() ? 1 : 0 ) ;
}


///
///
///
bool Point::isEmpty() const
{
    return _coordinate.isEmpty() ;
}

///
///
///
bool Point::is3D() const
{
    return _coordinate.is3D() ;
}

///
///
///
bool  Point::isMeasured() const
{
    return ! isNaN( _m ) ;
}

///
///
///
void Point::accept( GeometryVisitor& visitor )
{
    return visitor.visit( *this );
}

///
///
///
void Point::accept( ConstGeometryVisitor& visitor ) const
{
    return visitor.visit( *this );
}

///
///
///
bool Point::operator < ( const Point& other ) const
{
    return _coordinate < other._coordinate ;
}

///
///
///
bool Point::operator == ( const Point& other ) const
{
    return _coordinate == other._coordinate ;
}

///
///
///
bool Point::operator != ( const Point& other ) const
{
    return _coordinate != other._coordinate ;
}


///
/// Private structures used to implement partial function specialization
template <int D>
struct do_toPoint_d {
    static CGAL::Point_2<Kernel> toPoint( const Point* p ) {
        return p->toPoint_2();
    }
};

template <>
struct do_toPoint_d<3> {
    static CGAL::Point_3<Kernel> toPoint( const Point* p ) {
        return p->toPoint_3();
    }
};

template <int Dim>
typename TypeForDimension<Dim>::Point Point::toPoint_d() const
{
    return do_toPoint_d<Dim>::toPoint( this );
}
// template instanciations
template CGAL::Point_2<Kernel> Point::toPoint_d<2>() const;
template CGAL::Point_3<Kernel> Point::toPoint_d<3>() const;


}//SFCGAL

