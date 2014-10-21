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

#include <SFCGAL/GeometryVisitor.h>

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
void GeometryVisitor::visit( Geometry& g )
{
    g.accept( *this );
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
void ConstGeometryVisitor::visit( const Geometry& g )
{
    g.accept( *this );
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

