#include <SFCGAL/Transform.h>
#include <SFCGAL/all.h>

namespace SFCGAL {

///
///
///
Transform::~Transform()
{

}


///
///
///
void Transform::visit( Point & g )
{
	transform(g);
}

///
///
///
void Transform::visit( LineString & g )
{
	for ( size_t i = 0; i < g.numPoints(); i++ ){
		visit( g.pointN(i) );
	}
}

///
///
///
void Transform::visit( Polygon & g )
{
	for ( size_t i = 0; i < g.numRings(); i++ ){
		visit( g.ringN(i) );
	}
}

///
///
///
void Transform::visit( Triangle & g )
{
	visit( g.vertex(0) );
	visit( g.vertex(1) );
	visit( g.vertex(2) );
}

///
///
///
void Transform::visit( Solid & g )
{
	for ( size_t i = 0; i < g.numShells(); i++ ){
		visit( g.shellN(i) );
	}
}

///
///
///
void Transform::visit( MultiPoint & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.pointN(i) );
	}
}

///
///
///
void Transform::visit( MultiLineString & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.lineStringN(i) );
	}
}


///
///
///
void Transform::visit( MultiPolygon & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.polygonN(i) );
	}
}

///
///
///
void Transform::visit( MultiSolid & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.solidN(i) );
	}
}


///
///
///
void Transform::visit( GeometryCollection & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		GeometryVisitor::visit( g.geometryN(i) );
	}
}


///
///
///
void Transform::visit( PolyhedralSurface & g )
{
	for ( size_t i = 0; i < g.numPolygons(); i++ ){
		visit( g.polygonN(i) );
	}
}


///
///
///
void Transform::visit( TriangulatedSurface & g )
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		visit( g.geometryN(i) ) ;
	}
}





}//SFCGAL

