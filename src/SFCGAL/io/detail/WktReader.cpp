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

#include <SFCGAL/io/detail/WktReader.h>

#include <SFCGAL/Exception.h>

namespace SFCGAL {
namespace io {
namespace detail {

///
///
///
WktReader::WktReader( std::istream& s ):
    _reader( s )
{


}

///
///
///
srid_t WktReader::readSRID()
{
    srid_t srid = 0;

    if ( _reader.imatch( "SRID=" ) ) {
        _reader.read( srid );

        if ( !_reader.match( ";" ) ) {
            BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
        }
    }

    return srid;
}

///
///
Geometry<Epeck>   WktReader::readGeometry()
{
    GeometryType   geometryType   = readGeometryType() ;
    _is3D       = _reader.imatch( "Z" );
    _isMeasured = _reader.imatch( "M" );

    switch ( geometryType ) {
    case TYPE_POINT : {
        Point<Epeck> g;
        readInnerPoint( g );
        return g ;
    }

    case TYPE_LINESTRING: {
        LineString<Epeck> g;
        readInnerLineString( g );
        return g ;
    }

    case TYPE_TRIANGLE: {
        Triangle<Epeck> g;
        readInnerTriangle( g );
        return g ;
    }

    case TYPE_POLYGON: {
        Polygon<Epeck> g;
        readInnerPolygon( g );
        return g ;
    }

    case TYPE_MULTIPOINT : {
        MultiPoint<Epeck> g;
        readInnerMultiPoint( g );
        return g;
    }

    case TYPE_MULTILINESTRING : {
        MultiLineString<Epeck> g;
        readInnerMultiLineString( g );
        return g ;
    }

    case TYPE_MULTIPOLYGON : {
        MultiPolygon<Epeck> g;
        readInnerMultiPolygon( g );
        return g ;
    }

    case TYPE_GEOMETRYCOLLECTION : {
        GeometryCollection<Epeck> g;
        readInnerGeometryCollection( g );
        return g ;
    }

    case TYPE_TRIANGULATEDSURFACE : {
        TriangulatedSurface<Epeck> g;
        readInnerTriangulatedSurface( g );
        return g ;
    }

    case TYPE_POLYHEDRALSURFACE : {
        PolyhedralSurface<Epeck> g ;
        readInnerPolyhedralSurface( g );
        return g ;
    }

    case TYPE_SOLID : {
        Solid<Epeck> g;
        readInnerSolid( g );
        return g ;
    }

    case TYPE_MULTISOLID : {
        MultiSolid<Epeck> g;
        readInnerMultiSolid( g );
        return g ;
    }
    }

    BOOST_THROW_EXCEPTION( WktParseException( "unexpected geometry" ) );
}

///
///
///
GeometryType WktReader::readGeometryType()
{
    if ( _reader.imatch( "POINT" ) ) {
        return TYPE_POINT ;
    }
    else if ( _reader.imatch( "LINESTRING" ) ) {
        return TYPE_LINESTRING ;
    }
    else if ( _reader.imatch( "POLYGON" ) ) {
        return TYPE_POLYGON ;
    }
    else if ( _reader.imatch( "TRIANGLE" ) ) {
        //not official
        return TYPE_TRIANGLE ;
    }
    else if ( _reader.imatch( "MULTIPOINT" ) ) {
        return TYPE_MULTIPOINT ;
    }
    else if ( _reader.imatch( "MULTILINESTRING" ) ) {
        return TYPE_MULTILINESTRING ;
    }
    else if ( _reader.imatch( "MULTIPOLYGON" ) ) {
        return TYPE_MULTIPOLYGON ;
    }
    else if ( _reader.imatch( "GEOMETRYCOLLECTION" ) ) {
        return TYPE_GEOMETRYCOLLECTION ;
    }
    else if ( _reader.imatch( "TIN" ) ) {
        return TYPE_TRIANGULATEDSURFACE ;
    }
    else if ( _reader.imatch( "POLYHEDRALSURFACE" ) ) {
        return TYPE_POLYHEDRALSURFACE ;
    }
    else if ( _reader.imatch( "SOLID" ) ) {
        //not official
        return TYPE_SOLID ;
    }
    else if ( _reader.imatch( "MULTISOLID" ) ) {
        //not official
        return TYPE_MULTISOLID ;
    }

    std::ostringstream oss;
    oss << "can't parse WKT geometry type (" << _reader.context() << ")" ;
    BOOST_THROW_EXCEPTION( WktParseException( oss.str() ) );
}


///
///
///
void   WktReader::readInnerPoint( Point<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    readPointCoordinate( g );

    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}

///
///
///
void   WktReader::readInnerLineString( LineString<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    while ( ! _reader.eof() ) {

        Point<Epeck> p;
        if ( readPointCoordinate( p ) ) {
            g.push_back(p);
        }
        else {
            BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
        }

        if ( _reader.match( ',' ) ) {
            continue ;
        }

        break;
    }


    if ( g.size() < 2U ) {
        BOOST_THROW_EXCEPTION( WktParseException( "WKT parse error, LineString should have at least 2 points" ) );
    }


    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}

///
///
///
void   WktReader::readInnerPolygon( Polygon<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    for( int i = 0; ! _reader.eof() ; i++ ) {
        if ( i == 0 ) {
            LineString<Epeck> exteriorRing ;
            readInnerLineString( exteriorRing ) ;
            g.push_back(exteriorRing);
        }
        else {
            LineString<Epeck> interiorRing;
            readInnerLineString( interiorRing ) ;
            g.push_back( interiorRing ) ;
        }

        //break if not followed by another ring
        if ( ! _reader.match( ',' ) ) {
            break ;
        }
    }

    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}


///
///
///
void   WktReader::readInnerTriangle( Triangle<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    // 4 points to read
    std::vector< Point<Epeck> > points ;

    while ( ! _reader.eof() ) {
        points.push_back( Point<Epeck>() ) ;
        readPointCoordinate( points.back() );

        if ( ! _reader.match( "," ) ) {
            break;
        }
    }

    if ( points.size() != 4 ) {
        BOOST_THROW_EXCEPTION( WktParseException( "WKT parse error, expected 4 points for triangle" ) );
    }

    if ( points.back() != points.front() ) {
        BOOST_THROW_EXCEPTION( WktParseException( "WKT parse error, first point different of the last point for triangle" ) );
    }

    g = Triangle<Epeck>( points[0], points[1], points[2] );

    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}

///
///
///
void    WktReader::readInnerMultiPoint( MultiPoint<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    while( ! _reader.eof() ) {
        Point<Epeck> p ;

        if ( !_reader.imatch( "EMPTY" ) ) {
            // optional open/close parenthesis
            bool parenthesisOpen = false ;

            if ( _reader.match( '(' ) ) {
                parenthesisOpen = true ;
            }

            readPointCoordinate( p );

            if ( parenthesisOpen && ! _reader.match( ')' ) ) {
                BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
            }
        }

        //if ( !p->isEmpty() ) g.addGeometry( p.release() );
        g.push_back(p);
        
        //break if not followed by another points
        if ( ! _reader.match( ',' ) ) {
            break ;
        }
    }

    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

}

///
///
///
void   WktReader::readInnerMultiLineString( MultiLineString<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    while( ! _reader.eof() ) {

        LineString<Epeck> lineString ;
        readInnerLineString( lineString );
        if ( ! lineString.empty() ){
            g.push_back(lineString);
        }

        //break if not followed by another points
        if ( ! _reader.match( ',' ) ) {
            break ;
        }
    }

    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}

///
///
///
void   WktReader::readInnerMultiPolygon( MultiPolygon<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    while( ! _reader.eof() ) {

        Polygon<Epeck> polygon;
        readInnerPolygon( polygon );
        if ( ! polygon.empty() ){
            g.push_back(polygon);
        }

        //break if not followed by another points
        if ( ! _reader.match( ',' ) ) {
            break ;
        }
    }

    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}

///
///
///
void   WktReader::readInnerGeometryCollection( GeometryCollection<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    while( ! _reader.eof() ) {

        //read a full wkt geometry ex : POINT(2.0 6.0)
        Geometry<Epeck> gg = readGeometry();
        // TODO restore???
        //if ( !gg->isEmpty() )
        //    g.addGeometry( gg );
        g.push_back(gg);

        //break if not followed by another points
        if ( ! _reader.match( ',' ) ) {
            break ;
        }
    }

    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}

///
///
///
void  WktReader::readInnerTriangulatedSurface( TriangulatedSurface<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    while( ! _reader.eof() ) {
        Triangle<Epeck> triangle ;
        readInnerTriangle( triangle );
        g.push_back( triangle ) ;

        //break if not Oui tout à fait, le Z n'est pas pris en compte dans ce cas là.followed by another points
        if ( ! _reader.match( ',' ) ) {
            break ;
        }
    }


    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}

///
///
///
void WktReader::readInnerPolyhedralSurface( PolyhedralSurface<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    while( ! _reader.eof() ) {
        Polygon<Epeck> polygon ;
        readInnerPolygon( polygon );
        g.push_back( polygon );

        //break if not followed by another points
        if ( ! _reader.match( ',' ) ) {
            break ;
        }
    }

    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}


///
///
///
void WktReader::readInnerSolid( Solid<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    //solid begin
    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    for ( int i = 0; ! _reader.eof(); i++ ) {
        PolyhedralSurface<Epeck> shell ;
        readInnerPolyhedralSurface( shell );
        g.push_back(shell);

        //break if not followed by another points
        if ( ! _reader.match( ',' ) ) {
            break ;
        }
    }

    //solid end
    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}

///
///
///
void WktReader::readInnerMultiSolid( MultiSolid<Epeck>& g )
{
    if ( _reader.imatch( "EMPTY" ) ) {
        return ;
    }

    if ( ! _reader.match( '(' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }

    while( ! _reader.eof() ) {

        Solid<Epeck> solid ;
        readInnerSolid( solid );
        if ( ! solid.empty() ){
            g.push_back(solid);
        }

        //break if not followed by another points
        if ( ! _reader.match( ',' ) ) {
            break ;
        }
    }

    if ( ! _reader.match( ')' ) ) {
        BOOST_THROW_EXCEPTION( WktParseException( parseErrorMessage() ) );
    }
}


///
///
///
bool WktReader::readPointCoordinate( Point<Epeck>& p )
{
    //std::vector< Epeck::Exact_kernel::FT > coordinates ;
    //Epeck::Exact_kernel::FT d;
    std::vector< Epeck::FT > coordinates ;
    Epeck::FT d;

    if ( _reader.imatch( "EMPTY" ) ) {
        p = Point<Epeck>();
        return false;
    }

    while ( _reader.read( d ) ) {
        coordinates.push_back( d );
    }

    if ( coordinates.size() < 2 ) {
        BOOST_THROW_EXCEPTION( WktParseException(
            ( boost::format( "WKT parse error, Coordinate dimension < 2 (%s)" ) % _reader.context() ).str()
        ) );
    }

    if ( coordinates.size() > 4 ) {
        BOOST_THROW_EXCEPTION( WktParseException( "WKT parse error, Coordinate dimension > 4" ) );
    }

    if ( _isMeasured && _is3D ) {
        // XYZM
        if ( coordinates.size() != 4 ) {
            BOOST_THROW_EXCEPTION( WktParseException( "bad coordinate dimension" ) );
        }

        p = Point<Epeck>( coordinates[0], coordinates[1], coordinates[2] );
        //p.setM( coordinates[3].to_double() );
    }
    else if ( _isMeasured && ! _is3D ) {
        // XYM
        if ( coordinates.size() != 3 ) {
            BOOST_THROW_EXCEPTION( WktParseException( "bad coordinate dimension (expecting XYM coordinates)" ) );
        }

        p = Point<Epeck>( coordinates[0], coordinates[1], 0 ); // TO 3D
        //p.setM( coordinates[2].to_double() );
    }
    else if ( coordinates.size() == 3 ) {
        // XYZ
        p = Point<Epeck>( coordinates[0], coordinates[1], coordinates[2] );
    }
    else {
        // XY
        p = Point<Epeck>( coordinates[0], coordinates[1], 0 ); // TO 3D
    }

    return true ;
}


///
///
///
std::string WktReader::parseErrorMessage()
{
    std::ostringstream oss;
    oss << "WKT parse error (" << _reader.context() << ")" ;
    return oss.str();
}






}//detail
}//io
}//SFCGAL

