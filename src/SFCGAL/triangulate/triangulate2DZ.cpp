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
#include <SFCGAL/triangulate/triangulate2DZ.h>
#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/isValid.h>

namespace SFCGAL {
namespace triangulate {

typedef ConstraintDelaunayTriangulation::Vertex_handle Vertex_handle ;

///
///
///
void triangulate2DZ( const Point & g, ConstraintDelaunayTriangulation & triangulation ){
	triangulation.addVertex( g.coordinate() );
}
///
///
///
void triangulate2DZ( const LineString & g, ConstraintDelaunayTriangulation & triangulation ){
	Vertex_handle last ;
	for ( size_t i = 0; i < g.numPoints(); i++ ){
		Vertex_handle vertex = triangulation.addVertex( g.pointN(i).coordinate() );
		if ( i != 0 ){
			triangulation.addConstraint( last, vertex ) ;
		}
		last = vertex ;
	}
}
///
///
///
void triangulate2DZ( const Polygon & g, ConstraintDelaunayTriangulation & triangulation ){
	for ( size_t i = 0; i < g.numRings(); i++ ){
		triangulate2DZ( g.ringN(i), triangulation ) ;
	}
}
///
///
///
void triangulate2DZ( const Triangle & g, ConstraintDelaunayTriangulation & triangulation ){
	Vertex_handle last ;
	for ( size_t i = 0; i < 4; i++ ){
		Vertex_handle vertex = triangulation.addVertex( g.vertex(i).coordinate() );
		if ( i != 0 ){
			triangulation.addConstraint( last, vertex ) ;
		}
		last = vertex ;
	}
}
///
///
///
void triangulateCollection2DZ( const Geometry & g, ConstraintDelaunayTriangulation & triangulation ){
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		triangulate2DZ( g.geometryN(i), triangulation ) ;
	}
}



///
///
///
void triangulate2DZ( const Geometry & g, ConstraintDelaunayTriangulation & triangulation ){

    if (g.isEmpty() ) return;
  
    if (triangulation.hasProjectionPlane()) {
        SFCGAL_ASSERT_GEOMETRY_VALIDITY_ON_PLANE(g, triangulation.projectionPlane() );
    }
    else {
        SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( g );
    }


	switch ( g.geometryTypeId() ){
	case TYPE_POINT:
		triangulate2DZ( g.as< Point >(), triangulation );
		return ;
	case TYPE_LINESTRING:
		triangulate2DZ( g.as< LineString >(), triangulation );
		return ;
	case TYPE_POLYGON:
		triangulate2DZ( g.as< Polygon >(), triangulation );
		return ;
	case TYPE_TRIANGLE:
		triangulate2DZ( g.as< Triangle >(), triangulation );
		return ;
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_MULTIPOLYGON:
	case TYPE_POLYHEDRALSURFACE:
	case TYPE_TRIANGULATEDSURFACE:
	case TYPE_GEOMETRYCOLLECTION:
		triangulateCollection2DZ( g, triangulation );
		return ;
	case TYPE_SOLID:
	case TYPE_MULTISOLID:
        // note: we can't have à valid geom in 2D that comes from à solid 
        // since a solid closed and thus self-intersects once projected
		BOOST_THROW_EXCEPTION(
				      InappropriateGeometryException(
						( boost::format( "can't process 2DZ triangulation for type '%1%'" ) % g.geometryType() ).str()
						)
				      );
	}
}


///
///
///
ConstraintDelaunayTriangulation triangulate2DZ( const Geometry & g )
{
	ConstraintDelaunayTriangulation triangulation ;
	triangulate2DZ(g,triangulation);
	return triangulation ;
}


}//triangulate
}//SFCGAL


