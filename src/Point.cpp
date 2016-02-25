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
    _coordinate()
{
}

///
///
///
Point::Point( const Coordinate& coordinate ):
    _coordinate( coordinate )
{

}

///
///
///
Point::Point( const Kernel::FT& x, const Kernel::FT& y ):
    _coordinate( x,y )
{

}

///
///
///
Point::Point( const Kernel::FT& x, const Kernel::FT& y, const Kernel::FT& z, const double& m ):
    _coordinate( x,y,z)
{
    _coordinate.setM(m);
}

///
///
///
Point::Point( const Kernel::Point_2& other ):
    _coordinate( other )
{

}

///
///
///
Point::Point( const Kernel::Point_3& other ):
    _coordinate( other )
{

}


///
///
///
Point::Point( const Point& other ):
    Geometry(other),
    _coordinate( other._coordinate )
{

}

///
///
///
Point& Point::operator = ( const Point& other )
{
    _metadata = other._metadata ;
    _coordinate = other._coordinate ;
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
    return _coordinate.isMeasured() ;
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
        return p->toPoint_2<Kernel>();
    }
};

template <>
struct do_toPoint_d<3> {
    static CGAL::Point_3<Kernel> toPoint( const Point* p ) {
        return p->toPoint_3<Kernel>();
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
