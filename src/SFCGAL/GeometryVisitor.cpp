#include <SFCGAL/GeometryVisitor.h>
#include <SFCGAL/all.h>
#include <SFCGAL/Exception.h>



namespace SFCGAL {

///
///
///
GeometryVisitor::~GeometryVisitor()
{

}

///
///
///
void GeometryVisitor::visit( Geometry & g )
{
	switch ( g.geometryTypeId() ){
	case TYPE_POINT:
		return visit( g.as< Point >() );
	case TYPE_LINESTRING:
		return visit( g.as< LineString >() );
	case TYPE_POLYGON:
		return visit( g.as< Polygon >() );
	case TYPE_MULTIPOINT:
		return visit( g.as< MultiPoint >() );
	case TYPE_MULTILINESTRING:
		return visit( g.as< MultiLineString >() );
	case TYPE_MULTIPOLYGON:
		return visit( g.as< MultiPolygon >() );
	case TYPE_GEOMETRYCOLLECTION:
		return visit( g.as< GeometryCollection >() );
	case TYPE_POLYHEDRALSURFACE:
		return visit( g.as< PolyhedralSurface >() );
	case TYPE_TIN:
		return visit( g.as< TriangulatedSurface >() );
	case TYPE_TRIANGLE:
		return visit( g.as< Triangle >() );
	case TYPE_SOLID:
		return visit( g.as< Solid >() );
	}

	BOOST_THROW_EXCEPTION( Exception("!!! missing switch case in visitor dispatching !!!") );
}

///
///
///
void GeometryVisitor::visit( MultiPoint & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.geometryN(i).as< Point >() );
	}
}

///
///
///
void GeometryVisitor::visit( MultiLineString & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.geometryN(i).as< LineString >() );
	}
}

///
///
///
void GeometryVisitor::visit( MultiPolygon & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.geometryN(i).as< Polygon >() );
	}
}


///
///
///
void GeometryVisitor::visit( GeometryCollection & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.geometryN(i) );
	}
}

///
///
///
void GeometryVisitor::visit( PolyhedralSurface & g )
{
	for ( size_t i = 0; i < g.numPolygons(); i++ ){
		visit( g.polygonN(i) );
	}
}

///
///
///
void GeometryVisitor::visit( TriangulatedSurface & g )
{
	for ( size_t i = 0; i < g.numTriangles(); i++ ){
		visit( g.triangleN(i) );
	}
}





}//SFCGAL

