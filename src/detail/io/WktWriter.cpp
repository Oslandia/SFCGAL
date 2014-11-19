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

#include <SFCGAL/detail/io/WktWriter.h>

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

#include <exception>
#include <boost/exception/all.hpp>

namespace SFCGAL {
namespace detail {
namespace io {

///
///
///
WktWriter::WktWriter( std::ostream& s ):
    _s( s ),
    _exactWrite( false )
{

}

void WktWriter::writeRec( const Geometry& g )
{
    switch( g.geometryTypeId() ) {
    case TYPE_POINT:
        write( g.as< Point >() );
        return ;

    case TYPE_LINESTRING:
        write( g.as< LineString >() );
        return ;

    case TYPE_POLYGON:
        write( g.as< Polygon >() );
        return ;

    case TYPE_GEOMETRYCOLLECTION:
        write( g.as< GeometryCollection >() );
        return ;

    case TYPE_MULTIPOINT:
        write( g.as< MultiPoint >() );
        return ;

    case TYPE_MULTILINESTRING:
        write( g.as< MultiLineString >() );
        return ;

    case TYPE_MULTIPOLYGON:
        write( g.as< MultiPolygon >() );
        return ;

    case TYPE_TRIANGLE:
        write( g.as< Triangle >() );
        return ;

    case TYPE_TRIANGULATEDSURFACE:
        write( g.as< TriangulatedSurface >() );
        return ;

    case TYPE_POLYHEDRALSURFACE:
        write( g.as< PolyhedralSurface >() );
        return ;

    case TYPE_SOLID:
        write( g.as< Solid >() );
        return ;

    case TYPE_MULTISOLID:
        write( g.as< MultiSolid >() );
        return ;
    }

    std::ostringstream oss;
    oss << "WktWriter : '" << g.geometryType() << "' is not supported" ;
    BOOST_THROW_EXCEPTION( std::runtime_error( oss.str() ) );
}

///
///
///
void WktWriter::write( const Geometry& g, bool exact )
{
    _exactWrite = exact;
    writeRec( g );
}

///
///
///
void WktWriter::writeCoordinateType( const Geometry& g )
{
    if ( g.is3D() && g.isMeasured() ) {
        _s << " ZM";
    }
    else if ( ! g.is3D() && g.isMeasured() ) {
        _s << " M";
    }
}

///
///
///
void WktWriter::writeCoordinate( const Point& g )
{
    if ( _exactWrite ) {
        _s << CGAL::exact( g.x() ) << " " << CGAL::exact( g.y() );

        if ( g.is3D() ) {
            _s << " " << CGAL::exact( g.z() );
        }
    }
    else {
        _s << g.x() << " " << g.y() ;

        if ( g.is3D() ) {
            _s << " " << g.z() ;
        }
    }

    // m coordinate
    if ( g.isMeasured() ) {
        _s << " " << g.m() ;
    }
}

///
///
///
void WktWriter::write( const Point& g )
{
    _s << "POINT" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    writeInner( g );
}

///
///
///
void WktWriter::writeInner( const Point& g )
{
    _s << "(";
    writeCoordinate( g );
    _s << ")";
}

///
///
///
void WktWriter::write( const LineString& g )
{
    _s << "LINESTRING" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    writeInner( g );
}

///
///
///
void WktWriter::writeInner( const LineString& g )
{
    _s << "(";

    for ( size_t i = 0; i < g.numPoints(); i++ ) {
        if ( i != 0 ) {
            _s << ",";
        }

        writeCoordinate( g.pointN( i ) );
    }

    _s << ")";
}


///
///
///
void WktWriter::write( const Polygon& g )
{
    _s << "POLYGON" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    writeInner( g );
}

///
///
///
void WktWriter::writeInner( const Polygon& g )
{
    _s << "(";
    writeInner( g.exteriorRing() );

    for ( size_t i = 0; i < g.numInteriorRings(); i++ ) {
        _s << ",";
        writeInner( g.interiorRingN( i ) );
    }

    _s << ")";
}

///
///
///
void WktWriter::write( const GeometryCollection& g )
{
    _s << "GEOMETRYCOLLECTION" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    _s << "(" ;

    for ( size_t i = 0 ; i < g.numGeometries(); i++ ) {
        if ( i != 0 ) {
            _s << ",";
        }

        writeRec( g.geometryN( i ) );
    }

    _s << ")" ;
}

///
///
///
void WktWriter::write( const MultiPoint& g )
{
    _s << "MULTIPOINT" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    _s << "(";

    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        if ( i != 0 ) {
            _s << "," ;
        }

        writeInner( g.geometryN( i ).as< Point >() );
    }

    _s << ")";
}

///
///
///
void WktWriter::write( const MultiLineString& g )
{
    _s << "MULTILINESTRING" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    _s << "(";

    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        if ( i != 0 ) {
            _s << "," ;
        }

        writeInner( g.geometryN( i ).as< LineString >() );
    }

    _s << ")";
}

///
///
///
void WktWriter::write( const MultiPolygon& g )
{
    _s << "MULTIPOLYGON" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    _s << "(";

    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        if ( i != 0 ) {
            _s << "," ;
        }

        writeInner( g.geometryN( i ).as< Polygon >() );
    }

    _s << ")";
}


///
///
///
void WktWriter::write( const MultiSolid& g )
{
    _s << "MULTISOLID" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    _s << "(";

    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        if ( i != 0 ) {
            _s << "," ;
        }

        writeInner( g.geometryN( i ).as< Solid >() );
    }

    _s << ")";
}

///
///
///
void WktWriter::write( const Triangle& g )
{
    _s << "TRIANGLE" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    writeInner( g );
}

///
///
///
void WktWriter::writeInner( const Triangle& g )
{
    _s << "(";
    _s << "(";

    //close triangle
    for ( size_t i = 0; i < 4; i++ ) {
        if ( i != 0 ) {
            _s << "," ;
        }

        writeCoordinate( g.vertex( i ) );
    }

    _s << ")";
    _s << ")";
}

///
///
///
void WktWriter::write( const TriangulatedSurface& g )
{
    _s << "TIN" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    _s << "(" ; //begin TIN

    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        if ( i != 0 ) {
            _s << ",";
        }

        writeInner( g.geometryN( i ) );
    }

    _s << ")" ; //end TIN
}


///
///
///
void WktWriter::write( const PolyhedralSurface& g )
{
    _s << "POLYHEDRALSURFACE" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    writeInner( g );
}

///
///
///
void WktWriter::writeInner( const PolyhedralSurface& g )
{
    _s << "(" ; //begin POLYHEDRALSURFACE

    for ( size_t i = 0; i < g.numPolygons(); i++ ) {
        if ( i != 0 ) {
            _s << ",";
        }

        writeInner( g.polygonN( i ) );
    }

    _s << ")" ; //end POLYHEDRALSURFACE
}

///
///
///
void WktWriter::write( const Solid& g )
{
    _s << "SOLID" ;
    writeCoordinateType( g );

    if ( g.isEmpty() ) {
        _s << " EMPTY" ;
        return ;
    }

    writeInner( g );
}

///
///
///
void WktWriter::writeInner( const Solid& g )
{
    _s << "(" ; //begin SOLID
    writeInner( g.exteriorShell() );

    for ( size_t i = 0; i < g.numInteriorShells(); i++ ) {
        _s << ",";
        writeInner( g.interiorShellN( i ) );
    }

    _s << ")" ; //end SOLID
}




}//io
}//detail
}//SFCGAL



