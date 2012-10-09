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

///Oui tout à fait, le Z n'est pas pris en compte dans ce cas là.
///
///
Geometry*    WktReader::readGeometry()
{
	GeometryType   geometryType   = readGeometryType() ;
	_is3D       = _reader.match("Z");
	_isMeasured = _reader.match("M");

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
	if ( _reader.match("POINT") ){
		return TYPE_POINT ;
	}else if ( _reader.match("LINESTRING") ){
		return TYPE_LINESTRING ;
	}else if ( _reader.match("POLYGON") ){
		return TYPE_POLYGON ;
	}else if ( _reader.match("TRIANGLE") ){
		//not official
		return TYPE_TRIANGLE ;
	}else if ( _reader.match("MULTIPOINT") ) {
		return TYPE_MULTIPOINT ;
	}else if ( _reader.match("MULTILINESTRING") ){
		return TYPE_MULTILINESTRING ;
	}else if ( _reader.match("MULTIPOLYGON") ){
		return TYPE_MULTIPOLYGON ;
	}else if ( _reader.match("GEOMETRYCOLLECTION") ){
		return TYPE_GEOMETRYCOLLECTION ;
	}else if ( _reader.match("TIN") ){
		return TYPE_TRIANGULATEDSURFACE ;
	}else if ( _reader.match("POLYHEDRALSURFACE") ){
		return TYPE_POLYHEDRALSURFACE ;
	}else if ( _reader.match("SOLID") ){
		//not official
		return TYPE_SOLID ;
	}else if ( _reader.match("MULTISOLID") ){
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
	if ( _reader.match("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
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
	if ( _reader.match("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while ( ! _reader.eof() ){
		Point p ;
		if ( readPointCoordinate( p ) ){
			g.addPoint( p );
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
	if ( _reader.match("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	std::vector< LineString > rings ;
	for( size_t i = 0;;i++){
		rings.push_back( LineString() );
		readInnerLineString( rings.back() );
		//break if not followed by another ring
		if ( ! _reader.match(',') ){
			break ;
		}
	}
	g = Polygon( rings );

	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}
}


///
///
///
void   WktReader::readInnerTriangle( Triangle & g )
{
	if ( _reader.match("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
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
	if ( _reader.match("EMPTY") ){
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
	if ( _reader.match("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
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
	if ( _reader.match("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
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
	if ( _reader.match("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
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
	if ( _reader.match("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
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
	if ( _reader.match("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
		Polygon polygon ;
		readInnerPolygon( polygon );
		g.addPolygon( polygon );

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
	if ( _reader.match("EMPTY") ){
		return ;
	}

	//solid begin
	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	std::vector< PolyhedralSurface > shells ;
	while( ! _reader.eof() ){
		PolyhedralSurface shell ;
		readInnerPolyhedralSurface( shell );
		shells.push_back( shell );

		//break if not followed by another points
		if ( ! _reader.match(',') ){
			break ;
		}
	}

	//solid end
	if ( ! _reader.match(')') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	g = Solid( shells );
}

///
///
///
void WktReader::readInnerMultiSolid( MultiSolid & g )
{
	if ( _reader.match("EMPTY") ){
		return ;
	}

	if ( ! _reader.match('(') ){
		BOOST_THROW_EXCEPTION( Exception( parseErrorMessage() ) );
	}

	while( ! _reader.eof() ){
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
	std::vector< double > coordinates ;
	double d;
	while ( _reader.read(d) ){
		coordinates.push_back( d );
	}

	if ( coordinates.size() < 2 ){
		BOOST_THROW_EXCEPTION( Exception( "WKT parse error, Coordinate dimension < 2" ) );
	}
	if ( coordinates.size() > 4 ){
		BOOST_THROW_EXCEPTION( Exception( "WKT parse error, Coordinate dimension > 4" ) );
	}

	if ( coordinates.size() >= 3 && ! ( _isMeasured && ! _is3D ) ){
		p = Point( coordinates[0], coordinates[1], coordinates[2] );
	}else{
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

