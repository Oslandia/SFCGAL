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
#include <SFCGAL/algorithm/straightSkeleton.h>
#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/orientation.h>
#include <SFCGAL/algorithm/isValid.h>

#include <CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>

namespace SFCGAL {
namespace algorithm {

typedef Kernel::Point_2                    Point_2 ;
typedef CGAL::Polygon_2<Kernel>            Polygon_2 ;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2 ;
typedef CGAL::Straight_skeleton_2<Kernel>  Straight_skeleton_2 ;


template<class K>
void straightSkeletonToMultiLineString(
	const CGAL::Straight_skeleton_2<K> & ss,
	MultiLineString& result
)
{
	typedef CGAL::Straight_skeleton_2<K> Ss ;

	typedef typename Ss::Vertex_const_handle     Vertex_const_handle ;
	typedef typename Ss::Halfedge_const_handle   Halfedge_const_handle ;
	typedef typename Ss::Halfedge_const_iterator Halfedge_const_iterator ;

	Halfedge_const_handle null_halfedge ;
	Vertex_const_handle   null_vertex ;

	for ( Halfedge_const_iterator it = ss.halfedges_begin(); it != ss.halfedges_end(); ++it ){
		// skip contour edge
		if ( ! it->is_bisector() ){
			continue ;
		}

		// avoid duplicates
		if ( it->opposite() < it ){
			continue ;
		}

		result.addGeometry( new LineString(
			Point( it->opposite()->vertex()->point() ),
			Point( it->vertex()->point() )
		) );
	}
}



///
///
///
std::auto_ptr< MultiLineString > straightSkeleton( const Geometry& g, bool autoOrientation )
{
	SFCGAL_ASSERT_GEOMETRY_VALIDITY( g );

	switch ( g.geometryTypeId() ){
	case TYPE_TRIANGLE:
		return straightSkeleton( g.as< Triangle >().toPolygon(), autoOrientation ) ;
	case TYPE_POLYGON:
		return straightSkeleton( g.as< Polygon >(), autoOrientation ) ;
	case TYPE_MULTIPOLYGON:
		return straightSkeleton( g.as< MultiPolygon >(), autoOrientation ) ;
	default:
		return std::auto_ptr< MultiLineString >( new MultiLineString );
	}
}

///
///
///
std::auto_ptr< MultiLineString > straightSkeleton( const Polygon& g, bool autoOrientation )
{
	std::auto_ptr< MultiLineString > result( new MultiLineString );
	if ( g.isEmpty() ){
		return result ;
	}

	Polygon_with_holes_2 polygon = g.toPolygon_with_holes_2() ;
	boost::shared_ptr< Straight_skeleton_2 > skeleton = CGAL::create_interior_straight_skeleton_2( polygon ) ;
	straightSkeletonToMultiLineString( *skeleton, *result ) ;
	return result ;
}


///
///
///
std::auto_ptr< MultiLineString > straightSkeleton( const MultiPolygon& g, bool autoOrientation )
{
	std::auto_ptr< MultiLineString > result( new MultiLineString );
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		Polygon_with_holes_2 polygon = g.polygonN(i).toPolygon_with_holes_2() ;
		boost::shared_ptr< Straight_skeleton_2 > skeleton = CGAL::create_interior_straight_skeleton_2( polygon ) ;
		straightSkeletonToMultiLineString( *skeleton, *result ) ;
	}
	return result ;
}


}//namespace algorithm
}//namespace SFCGAL

