#include <SFCGAL/GeometryVisitor.h>
#include <SFCGAL/all.h>
//#include <SFCGAL/Exception.h>



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
	g.accept(*this);
}
//
/////
/////
/////
//void GeometryVisitor::visit( MultiPoint & g )
//{
//	for ( size_t i = 0; i < g.numGeometries(); i++ ){
//		visit( g.geometryN(i).as< Point >() );
//	}
//}
//
/////
/////
/////
//void GeometryVisitor::visit( MultiLineString & g )
//{
//	for ( size_t i = 0; i < g.numGeometries(); i++ ){
//		visit( g.geometryN(i).as< LineString >() );
//	}
//}
//
/////
/////
/////
//void GeometryVisitor::visit( MultiPolygon & g )
//{
//	for ( size_t i = 0; i < g.numGeometries(); i++ ){
//		visit( g.geometryN(i).as< Polygon >() );
//	}
//}
//
//
/////
/////
/////
//void GeometryVisitor::visit( GeometryCollection & g )
//{
//	for ( size_t i = 0; i < g.numGeometries(); i++ ){
//		visit( g.geometryN(i) );
//	}
//}
//
/////
/////
/////
//void GeometryVisitor::visit( PolyhedralSurface & g )
//{
//	for ( size_t i = 0; i < g.numPolygons(); i++ ){
//		visit( g.polygonN(i) );
//	}
//}
//
/////
/////
/////
//void GeometryVisitor::visit( TriangulatedSurface & g )
//{
//	for ( size_t i = 0; i < g.numTriangles(); i++ ){
//		visit( g.triangleN(i) );
//	}
//}


//---------------- ConstGeometryVisitor



///
///
///
ConstGeometryVisitor::~ConstGeometryVisitor()
{

}

///
///
///
void ConstGeometryVisitor::visit( const Geometry & g )
{
	g.accept(*this);
}
//
/////
/////
/////
//void ConstGeometryVisitor::visit( const MultiPoint & g )
//{
//	for ( size_t i = 0; i < g.numGeometries(); i++ ){
//		visit( g.geometryN(i).as< Point >() );
//	}
//}
//
/////
/////
/////
//void ConstGeometryVisitor::visit( const MultiLineString & g )
//{
//	for ( size_t i = 0; i < g.numGeometries(); i++ ){
//		visit( g.geometryN(i).as< LineString >() );
//	}
//}
//
/////
/////
/////
//void ConstGeometryVisitor::visit( const MultiPolygon & g )
//{
//	for ( size_t i = 0; i < g.numGeometries(); i++ ){
//		visit( g.geometryN(i).as< Polygon >() );
//	}
//}
//
//
/////
/////
/////
//void ConstGeometryVisitor::visit( const GeometryCollection & g )
//{
//	for ( size_t i = 0; i < g.numGeometries(); i++ ){
//		visit( g.geometryN(i) );
//	}
//}
//
/////
/////
/////
//void ConstGeometryVisitor::visit( const PolyhedralSurface & g )
//{
//	for ( size_t i = 0; i < g.numPolygons(); i++ ){
//		visit( g.polygonN(i) );
//	}
//}
//
/////
/////
/////
//void ConstGeometryVisitor::visit( const TriangulatedSurface & g )
//{
//	for ( size_t i = 0; i < g.numTriangles(); i++ ){
//		visit( g.triangleN(i) );
//	}
//}
//





}//SFCGAL

