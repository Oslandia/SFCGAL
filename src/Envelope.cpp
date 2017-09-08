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

#include <SFCGAL/Envelope.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/PolyhedralSurface.h>

namespace SFCGAL {

///
///
///
Envelope::Envelope()
{
    for ( size_t i = 0; i < 3; i++ ) {
        _bounds[i] = detail::Interval();
    }
}

///
///
///
Envelope::Envelope(
    const double& xmin, const double& xmax,
    const double& ymin, const double& ymax
)
{
    _bounds[0] = detail::Interval( xmin,xmax );
    _bounds[1] = detail::Interval( ymin,ymax );
    _bounds[2] = detail::Interval();
}

///
///
///
Envelope::Envelope(
    const double& xmin, const double& xmax,
    const double& ymin, const double& ymax,
    const double& zmin, const double& zmax
)
{
    _bounds[0] = detail::Interval( xmin,xmax );
    _bounds[1] = detail::Interval( ymin,ymax );
    _bounds[2] = detail::Interval( zmin,zmax );
}

///
///
///
Envelope::Envelope( const Coordinate& p )
{
    expandToInclude( p );
}

///
///
///
Envelope::Envelope( const Coordinate& p1, const Coordinate& p2 )
{
    expandToInclude( p1 );
    expandToInclude( p2 );
}

///
///
///
Envelope::Envelope( const Envelope& other )
{
    for ( size_t i = 0; i < 3; i++ ) {
        _bounds[i] = other._bounds[i] ;
    }
}

///
///
///
Envelope& Envelope::operator = ( const Envelope& other )
{
    for ( size_t i = 0; i < 3; i++ ) {
        _bounds[i] = other._bounds[i] ;
    }

    return *this ;
}

///
///
///
Envelope::~Envelope()
{

}


///
///
///
bool Envelope::isEmpty() const
{
    return _bounds[0].isEmpty() || _bounds[1].isEmpty() ;
}

///
///
///
bool Envelope::is3D() const
{
    return ! isEmpty() && ! _bounds[2].isEmpty() ;
}

///
///
///
void Envelope::expandToInclude( const Coordinate& coordinate )
{
    if ( ! coordinate.isEmpty() ) {
        _bounds[0].expandToInclude( CGAL::to_double( coordinate.x() ) );
        _bounds[1].expandToInclude( CGAL::to_double( coordinate.y() ) );
    }

    if ( coordinate.is3D() ) {
        _bounds[2].expandToInclude( CGAL::to_double( coordinate.z() ) );
    }
}

///
///
///
bool Envelope::contains( const Envelope& a, const Envelope& b )
{
    if ( a.is3D() ) {
        return b.xMin() >= a.xMin() &&
               b.xMax() <= a.xMax() &&
               b.yMin() >= a.yMin() &&
               b.yMax() <= a.yMax() &&
               b.zMin() >= a.zMin() &&
               b.zMax() <= a.zMax();
    }

    return b.xMin() >= a.xMin() &&
           b.xMax() <= a.xMax() &&
           b.yMin() >= a.yMin() &&
           b.yMax() <= a.yMax();
}

///
///
///
bool Envelope::overlaps( const Envelope& a, const Envelope& b )
{
    if ( a.is3D() ) {
        CGAL::Bbox_3 abox = a.toBbox_3();
        CGAL::Bbox_3 bbox = b.toBbox_3();
        return CGAL::do_overlap( abox, bbox );
    }

    CGAL::Bbox_2 abox = a.toBbox_2();
    CGAL::Bbox_2 bbox = b.toBbox_2();
    return CGAL::do_overlap( abox, bbox );
}

///
///
///
std::unique_ptr< LineString > Envelope::toRing() const
{
    std::unique_ptr< LineString > ring( new LineString() ) ;

    if ( isEmpty() ) {
        return ring ;
    }

    ring->addPoint( new Point( xMin(), yMin() ) );
    ring->addPoint( new Point( xMax(), yMin() ) );
    ring->addPoint( new Point( xMax(), yMax() ) );
    ring->addPoint( new Point( xMin(), yMax() ) );
    ring->addPoint( ring->startPoint() );

    return ring ;
}

///
///
///
std::unique_ptr< Polygon > Envelope::toPolygon() const
{
    return std::unique_ptr< Polygon >( new Polygon( toRing().release() ) ) ;
}


///
///
///
std::unique_ptr< PolyhedralSurface >   Envelope::toShell() const
{
    std::unique_ptr< PolyhedralSurface > shell( new PolyhedralSurface() ) ;

    if ( ! is3D() ) {
        return shell ;
    }

    Point a( xMin(), yMin(), zMin() );
    Point b( xMax(), yMin(), zMin() );
    Point c( xMax(), yMax(), zMin() );
    Point d( xMin(), yMax(), zMin() );

    Point e( xMin(), yMin(), zMax() );
    Point f( xMax(), yMin(), zMax() );
    Point g( xMax(), yMax(), zMax() );
    Point h( xMin(), yMax(), zMax() );

    //bottom : a,d,c,b
    {
        LineString ring ;
        ring.addPoint( a );
        ring.addPoint( d );
        ring.addPoint( c );
        ring.addPoint( b );
        ring.addPoint( ring.startPoint() );

        shell->addPolygon( Polygon( ring ) ) ;
    }

    //top : e,f,g,h
    {
        LineString ring ;
        ring.addPoint( e );
        ring.addPoint( f );
        ring.addPoint( g );
        ring.addPoint( h );
        ring.addPoint( ring.startPoint() );

        shell->addPolygon( Polygon( ring ) ) ;
    }

    //front : a,b,f,e
    {
        LineString ring ;
        ring.addPoint( a );
        ring.addPoint( b );
        ring.addPoint( f );
        ring.addPoint( e );
        ring.addPoint( ring.startPoint() );

        shell->addPolygon( Polygon( ring ) ) ;
    }

    //back : c,d,h,g
    {
        LineString ring ;
        ring.addPoint( c );
        ring.addPoint( d );
        ring.addPoint( h );
        ring.addPoint( g );
        ring.addPoint( ring.startPoint() );

        shell->addPolygon( Polygon( ring ) ) ;
    }

    //right : b,c,g,f
    {
        LineString ring ;
        ring.addPoint( b );
        ring.addPoint( c );
        ring.addPoint( g );
        ring.addPoint( f );
        ring.addPoint( ring.startPoint() );

        shell->addPolygon( Polygon( ring ) ) ;
    }

    //left : a,e,h,d
    {
        LineString ring ;
        ring.addPoint( a );
        ring.addPoint( e );
        ring.addPoint( h );
        ring.addPoint( d );
        ring.addPoint( ring.startPoint() );

        shell->addPolygon( Polygon( ring ) ) ;
    }

    return shell ;
}

///
///
///
std::unique_ptr< Solid >  Envelope::toSolid() const
{
    return std::unique_ptr< Solid >(
               new Solid( toShell().release() )
           );
}


///
///
///
std::ostream& Envelope::print( std::ostream& ostr ) const
{
    ostr << "[ " << xMin();
    ostr << ", " << xMax();
    ostr << ", " << yMin();
    ostr << ", " << yMax();

    if ( is3D() ) {
        ostr << ", " << zMin() << ", " << zMax();
    }

    ostr << " ]";
    return ostr;
}

bool operator==( const Envelope& a, const Envelope& b )
{
    if ( a.is3D() ) {
        return a.xMin() == b.xMin() &&
               a.yMin() == b.yMin() &&
               a.zMin() == b.zMin() &&
               a.xMax() == b.xMax() &&
               a.yMax() == b.yMax() &&
               a.zMax() == b.zMax();
    }

    return a.xMin() == b.xMin() &&
           a.yMin() == b.yMin() &&
           a.xMax() == b.xMax() &&
           a.yMax() == b.yMax();
}
}//SFCGAL

