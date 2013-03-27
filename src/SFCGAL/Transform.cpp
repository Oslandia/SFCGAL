/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
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

