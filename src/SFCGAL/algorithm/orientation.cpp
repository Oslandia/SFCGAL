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
#include <SFCGAL/algorithm/orientation.h>

#include <SFCGAL/algorithm/ConsistentOrientationBuilder.h>

#include <SFCGAL/graph/GeometryGraph.h>
#include <SFCGAL/graph/GeometryGraphBuilder.h>
#include <SFCGAL/graph/algorithm/isHalfEdge.h>

#include <SFCGAL/graph/algorithm/orientation.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
bool hasConsistentOrientation3D( const TriangulatedSurface & g )
{
	using namespace graph ;

	if ( g.isEmpty() )
		return true ;


	GeometryGraph graph ;
	GeometryGraphBuilder graphBuilder( graph ) ;
	graphBuilder.addTriangulatedSurface( g );
	return graph::algorithm::isHalfEdge( graph ) ;
}


///
///
///
bool hasConsistentOrientation3D( const PolyhedralSurface & g )
{
	using namespace graph ;

	if ( g.isEmpty() )
		return true ;

	GeometryGraph graph ;
	GeometryGraphBuilder graphBuilder( graph ) ;
	graphBuilder.addPolyhedralSurface( g );
	return graph::algorithm::isHalfEdge( graph ) ;
}


///
///
///
void makeConsistentOrientation3D( TriangulatedSurface & g )
{
	ConsistentOrientationBuilder builder ;
	builder.addTriangulatedSurface(g);
	g = builder.buildTriangulatedSurface() ;
}

///
///
///
bool isCounterClockWiseOriented( const LineString& ls )
{
	// Compute the 'z' part of the Newell's formula
	// and test against 0
	Kernel::FT z = 0 ;
	for ( size_t i = 0; i < ls.numPoints() - 1; ++i )
	{
		const Point& pi = ls.pointN(i);
		const Point& pj = ls.pointN( (i+1) % ls.numPoints() );
		z += ( pi.x() - pj.x() ) * ( pi.y() + pj.y() );
	}
	return z > 0;
}

///
///
///
bool isCounterClockWiseOriented( const Triangle& tri )
{
	// Compute the 'z' part of the cross product

	return (tri.vertex(2).x() - tri.vertex(1).x()) * (tri.vertex(0).y() - tri.vertex(1).y()) -
		(tri.vertex(2).y() - tri.vertex(1).y()) * (tri.vertex(0).x() - tri.vertex(1).x()) > 0;
}

///
///
///
bool isCounterClockWiseOriented( const Polygon& poly )
{
	return isCounterClockWiseOriented( poly.exteriorRing() );
}

}//algorithm
}//SFCGAL

