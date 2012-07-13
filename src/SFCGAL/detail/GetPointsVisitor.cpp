#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/all.h>


namespace SFCGAL {
namespace detail {

///
///
///
void GetPointsVisitor::visit( const Point & g )
{
	points.push_back( &g );
}

///
///
///
void GetPointsVisitor::visit( const LineString & g )
{
	for ( size_t i = 0; i < g.numPoints(); i++ ){
		visit(g.pointN(i));
	}
}

///
///
///
void GetPointsVisitor::visit( const Polygon & g )
{
	for ( size_t i = 0; i < g.numRings(); i++ ){
		visit( g.ringN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( const Triangle & g )
{
	visit( g.vertex(0) );
	visit( g.vertex(1) );
	visit( g.vertex(2) );
}

///
///
///
void GetPointsVisitor::visit( const Solid & g )
{
	for ( size_t i = 0; i < g.numShells(); i++ ){
		visit( g.shellN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( const MultiPoint & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.pointN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( const MultiLineString & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.lineStringN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( const MultiPolygon & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.polygonN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( const GeometryCollection & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		g.geometryN(i).accept(*this);
	}
}

///
///
///
void GetPointsVisitor::visit( const PolyhedralSurface & g )
{
	for ( size_t i = 0; i < g.numPolygons(); i++ ){
		visit(g.polygonN(i));
	}
}

///
///
///
void GetPointsVisitor::visit( const TriangulatedSurface & g )
{
	for ( size_t i = 0; i < g.numTriangles(); i++ ){
		visit(g.triangleN(i));
	}
}



}//detail
}//SFCGAL
