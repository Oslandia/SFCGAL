#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/all.h>


namespace SFCGAL {
namespace detail {

///
///
///
void GetPointsVisitor::visit( Point & g )
{
	points.push_back( &g );
}

///
///
///
void GetPointsVisitor::visit( LineString & g )
{
	for ( size_t i = 0; i < g.numPoints(); i++ ){
		visit(g.pointN(i));
	}
}

///
///
///
void GetPointsVisitor::visit( Polygon & g )
{
	for ( size_t i = 0; i < g.numRings(); i++ ){
		visit( g.ringN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( Triangle & g )
{
	visit( g.vertex(0) );
	visit( g.vertex(1) );
	visit( g.vertex(2) );
}

///
///
///
void GetPointsVisitor::visit( Solid & g )
{
	for ( size_t i = 0; i < g.numShells(); i++ ){
		visit( g.shellN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( MultiPoint & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.pointN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( MultiLineString & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.lineStringN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( MultiPolygon & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.polygonN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( MultiSolid & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.solidN(i) );
	}
}

///
///
///
void GetPointsVisitor::visit( GeometryCollection & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		g.geometryN(i).accept(*this);
	}
}

///
///
///
void GetPointsVisitor::visit( PolyhedralSurface & g )
{
	for ( size_t i = 0; i < g.numPolygons(); i++ ){
		visit(g.polygonN(i));
	}
}

///
///
///
void GetPointsVisitor::visit( TriangulatedSurface & g )
{
	for ( size_t i = 0; i < g.numTriangles(); i++ ){
		visit(g.triangleN(i));
	}
}



}//detail
}//SFCGAL
