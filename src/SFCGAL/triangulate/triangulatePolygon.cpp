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
#include <SFCGAL/triangulate/triangulatePolygon.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>

#include <SFCGAL/triangulate/ConstraintDelaunayTriangulation.h>

#include <SFCGAL/algorithm/plane.h>

#include <iostream>


namespace SFCGAL {
namespace triangulate {

typedef ConstraintDelaunayTriangulation::Vertex_handle Vertex_handle ;

///
///
///
void triangulatePolygon3D(
	const Geometry & g,
	TriangulatedSurface & triangulatedSurface
)
{
	switch ( g.geometryTypeId() ){
	case TYPE_TRIANGLE:
		return triangulatePolygon3D( g.as< Triangle >(), triangulatedSurface );
	case TYPE_POLYGON:
		return triangulatePolygon3D( g.as< Polygon >(), triangulatedSurface );
	case TYPE_TRIANGULATEDSURFACE:
		return triangulatePolygon3D( g.as< TriangulatedSurface >(), triangulatedSurface );
	case TYPE_POLYHEDRALSURFACE:
		return triangulatePolygon3D( g.as< PolyhedralSurface >(), triangulatedSurface );
	case TYPE_SOLID:
		return triangulatePolygon3D( g.as< Solid >(), triangulatedSurface );
	case TYPE_MULTIPOLYGON:
	case TYPE_MULTISOLID:
	case TYPE_GEOMETRYCOLLECTION:
		return triangulatePolygon3D( g.as< GeometryCollection >(), triangulatedSurface );
	default:
		BOOST_THROW_EXCEPTION(
				      Exception(
						( boost::format( "can't triangulate 3d polygons for type '%1%'" ) % g.geometryType() ).str()
						)
				      );
	}
}

///
///
///
void triangulatePolygon3D(
	const Triangle & triangle,
	TriangulatedSurface & triangulatedSurface
)
{
	/*
	 * filter empty polygon
	 */
	if ( triangle.isEmpty() )
		return ;

	triangulatedSurface.addTriangle( triangle ) ;
}

///
///
///
void triangulatePolygon3D(
	const TriangulatedSurface & g,
	TriangulatedSurface & triangulatedSurface
)
{
	triangulatedSurface.addTriangles( g ) ;
}

///
///
///
void triangulatePolygon3D(
	const GeometryCollection & g,
	TriangulatedSurface & triangulatedSurface
)
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		triangulatePolygon3D( g.geometryN(i), triangulatedSurface );
	}
}

///
///
///
void triangulatePolygon3D(
	const Polygon & polygon,
	TriangulatedSurface & triangulatedSurface
)
{
	/*
	 * filter empty polygon
	 */
	if ( polygon.isEmpty() )
		return ;

	/*
	 * Prepare a Constraint Delaunay Triangulation
	 */
	ConstraintDelaunayTriangulation cdt;

	/*
	 * find polygon plane
	 */
	Kernel::Plane_3 polygonPlane = algorithm::plane3D< Kernel >( polygon, false ) ;
	if ( polygonPlane.is_degenerate() ){
		BOOST_THROW_EXCEPTION( Exception(
			( boost::format( "can't find plane for polygon %s" ) % polygon.asText() ).str()
		) );
	}
	cdt.setProjectionPlane( polygonPlane );

	/*
	 * insert each ring in the triangulation
	 */
	for ( size_t i = 0; i < polygon.numRings(); i++ ){
		const LineString & ring  = polygon.ringN( i );

		Vertex_handle v_prev ;
		for ( size_t j = 0; j < ring.numPoints(); j++ ) {
			Vertex_handle vh = cdt.addVertex( ring.pointN( j ).coordinate() );
			if ( j != 0 ){
				cdt.addConstraint(vh, v_prev);
			}
			v_prev = vh;
		}
	}

	/*
	 * Mark facets that are inside the domain bounded by the polygon
	 */
	cdt.markDomains() ;
	cdt.getTriangles( triangulatedSurface, true ) ;
}

///
///
///
void triangulatePolygon3D(
	const PolyhedralSurface & g,
	TriangulatedSurface & triangulatedSurface
)
{
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		triangulatePolygon3D( g.polygonN(i), triangulatedSurface );
	}
}


///
///
///
void triangulatePolygon3D(
	const Solid & g,
	TriangulatedSurface & triangulatedSurface
)
{
	for ( size_t i = 0; i < g.numShells(); i++ ){
		triangulatePolygon3D( g.shellN(i), triangulatedSurface );
	}
}


}//triangulate
}//SFCGAL



