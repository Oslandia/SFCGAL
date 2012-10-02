#include <SFCGAL/io/WktWriter.h>

#include <SFCGAL/all.h>

#include <exception>
#include <boost/exception/all.hpp>

namespace SFCGAL {
namespace io {

///
///
///
WktWriter::WktWriter( std::ostream & s ):
	_s(s)
{

}

///
///
///
void WktWriter::write( const Geometry& g )
{
	switch( g.geometryTypeId() ){
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
void WktWriter::writeCoordinate( const Point& g )
{
	_s << g.x() << " " << g.y() ;
	if ( g.is3D() ){
		_s << " " << g.z() ;
	}
}

///
///
///
void WktWriter::write( const Point & g )
{
	_s << "POINT" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}
	writeInner( g );
}

///
///
///
void WktWriter::writeInner( const Point & g )
{
	_s << "(";
	writeCoordinate( g );
	_s << ")";
}

///
///
///
void WktWriter::write( const LineString & g )
{
	_s << "LINESTRING" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}
	writeInner(g);
}

///
///
///
void WktWriter::writeInner( const LineString & g )
{
	_s << "(";
	for ( size_t i = 0; i < g.numPoints(); i++ ){
		if ( i != 0 )
			_s << ",";

		writeCoordinate( g.pointN(i) );
	}
	_s << ")";
}


///
///
///
void WktWriter::write( const Polygon & g )
{
	_s << "POLYGON" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}
	writeInner(g);
}

///
///
///
void WktWriter::writeInner( const Polygon & g )
{
	_s << "(";
	writeInner(g.exteriorRing());
	for ( size_t i = 0; i < g.numInteriorRings(); i++ ){
		_s << ",";
		writeInner( g.interiorRingN(i) );
	}
	_s << ")";
}

///
///
///
void WktWriter::write( const GeometryCollection & g )
{
	_s << "GEOMETRYCOLLECTION" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}

	_s << "(" ;
	for ( size_t i = 0 ; i < g.numGeometries(); i++ ){
		if ( i != 0 )
			_s << ",";
		write( g.geometryN(i) );
	}
	_s << ")" ;
}

///
///
///
void WktWriter::write( const MultiPoint & g )
{
	_s << "MULTIPOINT" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}

	_s << "(";
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		if ( i != 0 )
			_s << "," ;
		writeInner( g.geometryN(i).as< Point >() );
	}
	_s << ")";
}

///
///
///
void WktWriter::write( const MultiLineString & g )
{
	_s << "MULTILINESTRING" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}

	_s << "(";
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		if ( i != 0 )
			_s << "," ;
		writeInner( g.geometryN(i).as< LineString >() );
	}
	_s << ")";
}

///
///
///
void WktWriter::write( const MultiPolygon & g )
{
	_s << "MULTIPOLYGON" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}

	_s << "(";
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		if ( i != 0 )
			_s << "," ;
		writeInner( g.geometryN(i).as< Polygon >() );
	}
	_s << ")";
}


///
///
///
void WktWriter::write( const MultiSolid & g )
{
	_s << "MULTISOLID" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}

	_s << "(";
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		if ( i != 0 )
			_s << "," ;
		writeInner( g.geometryN(i).as< Solid >() );
	}
	_s << ")";
}

///
///
///
void WktWriter::write( const Triangle & g )
{
	_s << "POLYGON" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}
	writeInner(g);
}

///
///
///
void WktWriter::writeInner( const Triangle & g )
{
	_s << "(";
	_s << "(";
	//close triangle
	for ( size_t i = 0; i < 4; i++ ){
		if ( i != 0 ){
			_s << "," ;
		}
		writeCoordinate( g.vertex(i) );
	}
	_s << ")";
	_s << ")";
}

///
///
///
void WktWriter::write( const TriangulatedSurface & g )
{
	_s << "TIN" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}

	_s << "(" ; //begin TIN
	for ( size_t i = 0; i < g.numTriangles(); i++ ){
		if ( i != 0 ){
			_s << ",";
		}
		writeInner( g.triangleN(i) );
	}
	_s << ")" ; //end TIN
}


///
///
///
void WktWriter::write( const PolyhedralSurface & g )
{
	_s << "POLYHEDRALSURFACE" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}
	writeInner( g );
}

///
///
///
void WktWriter::writeInner( const PolyhedralSurface & g )
{
	_s << "(" ; //begin POLYHEDRALSURFACE
	for ( size_t i = 0; i < g.numPolygons(); i++ ){
		if ( i != 0 ){
			_s << ",";
		}
		writeInner( g.polygonN(i) );
	}
	_s << ")" ; //end POLYHEDRALSURFACE
}

///
///
///
void WktWriter::write( const Solid & g )
{
	_s << "SOLID" ;
	if ( g.isEmpty() ){
		_s << " EMPTY" ;
		return ;
	}
	writeInner( g );
}

///
///
///
void WktWriter::writeInner( const Solid & g )
{
	_s << "(" ; //begin SOLID
	writeInner(g.exteriorShell());
	for ( size_t i = 0; i < g.numInteriorShells(); i++ ){
		_s << ",";
		writeInner( g.interiorShellN(i) );
	}
	_s << ")" ; //end SOLID
}




}//io
}//SFCGAL



