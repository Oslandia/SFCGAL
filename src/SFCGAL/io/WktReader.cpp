/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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
#include <SFCGAL/io/WktReader.h>

#include <memory>

#include <SFCGAL/all.h>
#include <SFCGAL/Exception.h>

namespace SFCGAL {
namespace io {

///
///
///
WktReader::WktReader( std::istream & s ):
	_reader(s)
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
			BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
		}
	}
	return srid;
}

///
///
Geometry*    WktReader::readGeometry()
{
	GeometryType   geometryType   = readGeometryType() ;
	_is3D       = _reader.imatch("Z");
	_isMeasured = _reader.imatch("M");

	switch ( geometryType ){
	case TYPE_POINT :
	{
		std::auto_ptr< Point > g( new Point() );
		readInnerPoint( *g );
		return g.release() ;
	}
	case TYPE_LINESTRING:
	{
		std::auto_ptr< LineString > g( new LineString() );
		readInnerLineString( *g );
		return g.release() ;
	}
	case TYPE_TRIANGLE:
	{
		std::auto_ptr< Triangle > g( new Triangle() );
		if ( _reader.match("EMPTY") ) {
			return g.release();
		}
		if ( ! _reader.match('(') ){
			BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
		}
		readInnerTriangle( *g );
		if ( ! _reader.match(')') ){
			BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
		}
		return g.release() ;
	}
	case TYPE_POLYGON:
	{
		std::auto_ptr< Polygon > g( new Polygon() );
		readInnerPolygon( *g );
		return g.release() ;
	}
	case TYPE_MULTIPOINT :
	{
		std::auto_ptr< MultiPoint > g( new MultiPoint() );
		readInnerMultiPoint( *g );
		return g.release() ;
	}
	case TYPE_MULTILINESTRING :
	{
		std::auto_ptr< MultiLineString > g( new MultiLineString() );
		readInnerMultiLineString( *g );
		return g.release() ;
	}
	case TYPE_MULTIPOLYGON :
	{
		std::auto_ptr< MultiPolygon > g( new MultiPolygon() );
		readInnerMultiPolygon( *g );
		return g.release() ;
	}
	case TYPE_GEOMETRYCOLLECTION :
	{
		std::auto_ptr< GeometryCollection > g( new GeometryCollection() );
		readInnerGeometryCollection( *g );
		return g.release() ;
	}
	case TYPE_TRIANGULATEDSURFACE :
	{
		std::auto_ptr< TriangulatedSurface > g( new TriangulatedSurface() );
		readInnerTriangulatedSurface( *g );
		return g.release() ;
	}
	case TYPE_POLYHEDRALSURFACE :
	{
		std::auto_ptr< PolyhedralSurface > g( new PolyhedralSurface() );
		readInnerPolyhedralSurface( *g );
		return g.release() ;
	}
	case TYPE_SOLID :
	{
		std::auto_ptr< Solid > g( new Solid() );
		readInnerSolid( *g );
		return g.release() ;
	}
	case TYPE_MULTISOLID :
	{
		std::auto_ptr< MultiSolid > g( new MultiSolid() );
		readInnerMultiSolid( *g );
		return g.release() ;
	}
	}
	BOOST_THROW_EXCEPTION( Exception("unexpected geometry") );
}

///
///
///
GeometryType WktReader::readGeometryType()
{
	if ( _reader.imatch("POINT") ){
		return TYPE_POINT ;
	}else if ( _reader.imatch("LINESTRING") ){
		return TYPE_LINESTRING ;
	}else if ( _reader.imatch("POLYGON") ){
		return TYPE_POLYGON ;
	}else if ( _reader.imatch("TRIANGLE") ){
		//not official
		return TYPE_TRIANGLE ;
	}else if ( _reader.imatch("MULTIPOINT") ) {
		return TYPE_MULTIPOINT ;
	}else if ( _reader.imatch("MULTILINESTRING") ){
		return TYPE_MULTILINESTRING ;
	}else if ( _reader.imatch("MULTIPOLYGON") ){
		return TYPE_MULTIPOLYGON ;
	}else if ( _reader.imatch("GEOMETRYCOLLECTION") ){
		return TYPE_GEOMETRYCOLLECTION ;
	}else if ( _reader.imatch("TIN") ){
		return TYPE_TRIANGULATEDSURFACE ;
	}else if ( _reader.imatch("POLYHEDRALSURFACE") ){
		return TYPE_POLYHEDRALSURFACE ;
	}else if ( _reader.imatch("SOLID") ){
		//not official
		return TYPE_SOLID ;
	}else if ( _reader.imatch("MULTISOLID") ){
		//not official
		return TYPE_MULTISOLID ;
	}

	std::ostringstream oss;
	oss << "can't parse WKT geometry type (" << _reader.context() << ")" ;
	BOOST_THROW_EXCEPTION( Exception(oss.str()) );
}


///
///
///
void   WktReader::readInnerPoint( Point & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
	if ( _reader.imatch("EMPTY") ){
		return ;
	}	
	readPointCoordinate( g );
	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}

///
///
///
void   WktReader::readInnerLineString( LineString & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while ( ! _reader.eof() ){
		if ( _reader.imatch("EMPTY") ){
			break;
		}
		std::auto_ptr< Point > p( new Point() ) ;
		if ( readPointCoordinate( *p ) ){
			g.addPoint( p.release() );
		}else{
			BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
		}
		if ( _reader.match(',') ){
			continue ;
		}
		break;
	}


	if ( g.numPoints() < 2U ){
		BOOST_THROW_EXCEPTION( Exception("WKT parse error, LineString should have at least 2 points") );
	}


	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}

///
///
///
void   WktReader::readInnerPolygon( Polygon & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	if ( _reader.imatch("EMPTY") ){
		return ;
	}	

	for( int i = 0; ! _reader.eof() ; i++ ){
		if ( i == 0 ){
			readInnerLineString( g.exteriorRing() ) ;
		}else{
			std::auto_ptr< LineString > interiorRing( new LineString );
			readInnerLineString( *interiorRing ) ;
			g.addRing( interiorRing.release() ) ;
		}
		//break if not followed by another ring
		if ( ! _reader.match(',') ){
			break ;
		}
	}

	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}


///
///
///
void   WktReader::readInnerTriangle( Triangle & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	if ( _reader.imatch("EMPTY") ){
		return ;
	}	

	// 4 points to read
	std::vector< Point > points ;
	while ( ! _reader.eof() ){
		points.push_back( Point() ) ;
		readPointCoordinate( points.back() );
		if ( ! _reader.match(",") ){
			break;
		}
	}

	if ( ! points.size() ){
		BOOST_THROW_EXCEPTION( Exception( "WKT parse error, expected 4 points for triangle" ) );
	}
	if ( points.back() != points.front() ){
		BOOST_THROW_EXCEPTION( Exception( "WKT parse error, first point different of the last point for triangle" ) );
	}
	g = Triangle( points[0], points[1], points[2] );

	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}

///
///
///
void    WktReader::readInnerMultiPoint( MultiPoint & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
		std::auto_ptr< Point > p( new Point() );

		// optional open/close parenthesis
		bool parenthesisOpen = false ;
		if ( _reader.match('(') ){
			parenthesisOpen = true ;
		}

		readPointCoordinate( *p );
		g.addGeometry( p.release() );

		if ( parenthesisOpen && ! _reader.match(')') ){
			BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
		}

		//break if not followed by another points
		if ( ! _reader.match(',') ){
			break ;
		}
	}

	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

}

///
///
///
void   WktReader::readInnerMultiLineString( MultiLineString & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
		if ( _reader.imatch("EMPTY") ){
			break;
		}

		std::auto_ptr< LineString > lineString( new LineString() );
		readInnerLineString( *lineString );
		g.addGeometry( lineString.release() );

		//break if not followed by another points
		if ( ! _reader.match(',') ){
			break ;
		}
	}

	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}

///
///
///
void   WktReader::readInnerMultiPolygon( MultiPolygon & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
		if ( _reader.imatch("EMPTY") ){
			break;
		}

		std::auto_ptr< Polygon > polygon( new Polygon() );
		readInnerPolygon( *polygon );
		g.addGeometry( polygon.release() );

		//break if not followed by another points
		if ( ! _reader.match(',') ){
			break ;
		}
	}

	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}

///
///
///
void   WktReader::readInnerGeometryCollection( GeometryCollection & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
		if ( _reader.imatch("EMPTY") ){
			break ;
		}

		//read a full wkt geometry ex : POINT(2.0 6.0)
		g.addGeometry( readGeometry() );

		//break if not followed by another points
		if ( ! _reader.match(',') ){
			break ;
		}
	}

	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}

///
///
///
void  WktReader::readInnerTriangulatedSurface( TriangulatedSurface & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
		if ( _reader.imatch("EMPTY") ){
			break ;
		}

		if ( ! _reader.match('(') ){
			BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
		}

		while( ! _reader.eof() ){
			std::auto_ptr< Triangle > triangle( new Triangle() ) ;
			readInnerTriangle( *triangle );
			g.addTriangle( triangle.release() ) ;

			//break if not Oui tout à fait, le Z n'est pas pris en compte dans ce cas là.followed by another points
			if ( ! _reader.match(',') ){
				break ;
			}
		}

		if ( ! _reader.match(')') ){
			BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
		}

		if ( ! _reader.match(',') ){
			break ;
		}
	}

	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}

///
///
///
void WktReader::readInnerPolyhedralSurface( PolyhedralSurface & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
		if ( _reader.imatch("EMPTY") ){
			break ;
		}

		std::auto_ptr< Polygon > polygon( new Polygon() ) ;
		readInnerPolygon( *polygon );
		g.addPolygon( polygon.release() );

		//break if not followed by another points
		if ( ! _reader.match(',') ){
			break ;
		}
	}

	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}


///
///
///
void WktReader::readInnerSolid( Solid & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	//solid begin
	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	for ( int i = 0; ! _reader.eof(); i++ ){
		if ( _reader.imatch("EMPTY") ){
			break ;
		}

		if ( i == 0 ){
			readInnerPolyhedralSurface( g.exteriorShell() );
		}else{
			std::auto_ptr< PolyhedralSurface > shell( new PolyhedralSurface ) ;
			readInnerPolyhedralSurface( *shell );
			g.addInteriorShell( shell.release() );
		}

		//break if not followed by another points
		if ( ! _reader.match(',') ){
			break ;
		}
	}

	//solid end
	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}

///
///
///
void WktReader::readInnerMultiSolid( MultiSolid & g )
{
	if ( _reader.imatch("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
		if ( _reader.imatch("EMPTY") ){
			break ;
		}

		std::auto_ptr< Solid > solid( new Solid() );
		readInnerSolid( *solid );
		g.addGeometry( solid.release() );

		//break if not followed by another points
		if ( ! _reader.match(',') ){
			break ;
		}
	}

	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}


///
///
///
bool WktReader::readPointCoordinate( Point & p )
{
	std::vector< Kernel::Exact_kernel::FT > coordinates ;
	Kernel::Exact_kernel::FT d;
	if ( _reader.imatch("EMPTY") ) {
		p = Point();
		return false;
	}
	while ( _reader.read(d) ){
		coordinates.push_back( d );
	}

	if ( coordinates.size() < 2 ){
		BOOST_THROW_EXCEPTION( Exception(
			( boost::format( "WKT parse error, Coordinate dimension < 2 (%s)" ) % _reader.context() ).str()
		) );
	}
	if ( coordinates.size() > 4 ){
		BOOST_THROW_EXCEPTION( Exception( "WKT parse error, Coordinate dimension > 4" ) );
	}

	if ( _isMeasured && _is3D ){
		// XYZM
		if ( coordinates.size() != 4 ){
			BOOST_THROW_EXCEPTION( Exception( "bad coordinate dimension" ) );
		}
		p = Point( coordinates[0], coordinates[1], coordinates[2] );
		p.setM(coordinates[3].to_double());
	}else if ( _isMeasured && ! _is3D ){
		// XYM
		if ( coordinates.size() != 3 ){
			BOOST_THROW_EXCEPTION( Exception( "bad coordinate dimension (expecting XYM coordinates)" ) );
		}
		p = Point( coordinates[0], coordinates[1] );
		p.setM(coordinates[2].to_double());
	}else if ( coordinates.size() == 3 ){
		// XYZ
		p = Point( coordinates[0], coordinates[1], coordinates[2] );
	}else{
		// XY
		p = Point( coordinates[0], coordinates[1] );
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






}//io
}//SFCGAL

