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

#include <SFCGAL/algorithm/straightSkeleton.h>

#include <SFCGAL/Envelope.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>

#include <SFCGAL/Exception.h>

#include <SFCGAL/algorithm/orientation.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/differencePrimitives.h>
#include <SFCGAL/algorithm/plane.h>

#include <CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>
#include <CGAL/Straight_skeleton_converter_2.h>

#define DEBUG_OUT if (1) std::cerr << __FILE__ << ":" << __LINE__ << " debug: "

namespace SFCGAL {
namespace algorithm {

typedef Kernel::Point_2                    Point_2 ;
typedef CGAL::Polygon_2<Kernel>            Polygon_2 ;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2 ;
typedef CGAL::Straight_skeleton_2<Kernel>  Straight_skeleton_2 ;

Kernel::Plane_3 xy_plane(Point_3(0,0,0), Kernel::Vector_3(0,0,1));


template<class K>
void straightSkeletonToMultiLineString(
    const CGAL::Straight_skeleton_2<K>& ss,
    MultiLineString& result,
    bool innerOnly,
    const Kernel::Vector_2& translate,
    const Kernel::Plane_3& plane
)
{
    typedef CGAL::Straight_skeleton_2<K> Ss ;

    typedef typename Ss::Vertex_const_handle     Vertex_const_handle ;
    typedef typename Ss::Halfedge_const_handle   Halfedge_const_handle ;
    typedef typename Ss::Halfedge_const_iterator Halfedge_const_iterator ;

    Halfedge_const_handle null_halfedge ;
    Vertex_const_handle   null_vertex ;

    for ( Halfedge_const_iterator it = ss.halfedges_begin(); it != ss.halfedges_end(); ++it ) {
        // skip contour edge
        if ( ! it->is_bisector() ) {
            continue ;
        }

        // Skip non-inner edges if requested
        if ( innerOnly && ! it->is_inner_bisector() ) {
            continue ;
        }

        // avoid duplicates
        if ( it->opposite() < it ) {
            continue ;
        }

        // transform the pair of points
        if ( plane == xy_plane ) {
            result.addGeometry( new LineString(
                it->opposite()->vertex()->point() - translate,
                it->vertex()->point() - translate ));
        }
        else {
            result.addGeometry( new LineString(
                plane.to_3d( it->opposite()->vertex()->point() - translate ),
                plane.to_3d( it->vertex()->point() - translate ) ) );
        }
    }
}

boost::shared_ptr< Straight_skeleton_2 >
straightSkeleton(const Polygon_with_holes_2& poly)
{
  boost::shared_ptr< CGAL::Straight_skeleton_2<CGAL::Epick> > sk  =
  CGAL::create_interior_straight_skeleton_2(
    poly.outer_boundary().vertices_begin(),
    poly.outer_boundary().vertices_end  (),
    poly.holes_begin   (),
    poly.holes_end     (),
    CGAL::Epick()
  );
  return CGAL::convert_straight_skeleton_2< Straight_skeleton_2 > ( *sk ) ;
}

// Throw an exception if holes touch, since CGAL segfaults in that case.
// See https://github.com/Oslandia/SFCGAL/issues/75
void
checkNoTouchingHoles( const Polygon& g )
{
    BOOST_ASSERT(!g.is3D());
    const size_t numRings =  g.numRings();

    for ( size_t ri=0; ri < numRings-1; ++ri ) {
        for ( size_t rj=ri+1; rj < numRings; ++rj ) {
            std::auto_ptr<Geometry> inter = intersection( g.ringN( ri ), g.ringN( rj ) );

            if ( ! inter->isEmpty() && inter->is< Point >() ) {
                BOOST_THROW_EXCEPTION( NotImplementedException(
                                           "straight skeleton of Polygon with touching interior rings is not implemented"
                                       ) );
            }
        }
    }
}

Polygon_with_holes_2
preparePolygon( const Polygon& poly, Kernel::Vector_2& trans, Kernel::Plane_3& polygonPlane )
{
  Envelope env = poly.envelope();
  trans = Kernel::Vector_2(-env.xMin(), -env.yMin());

  // put the polygon in the plane and create Polygon_with_holes_2
  Polygon_2 outer;
  std::vector< Polygon_2 > holes(poly.numInteriorRings());
  const bool is3D = poly.is3D();
  polygonPlane = is3D ? plane3D< Kernel >( poly, false ) : xy_plane;
  for (LineString::const_iterator vtx=poly.exteriorRing().begin();
          vtx + 1 != poly.exteriorRing().end(); vtx++) {
      if ( is3D ) outer.push_back( polygonPlane.to_2d( vtx->toPoint_3() ) + trans );
      else outer.push_back( vtx->toPoint_2() + trans );
  }
  for (size_t i=0; i<holes.size(); i++) {
      for (LineString::const_iterator vtx=poly.interiorRingN(i).begin();
              vtx + 1 != poly.interiorRingN(i).end(); vtx++) {
          if ( is3D ) holes[i].push_back( polygonPlane.to_2d( vtx->toPoint_3() ) + trans );
          else holes[i].push_back( vtx->toPoint_2() + trans );
      }
  }

  DEBUG_OUT << outer << "\n";
  Polygon_with_holes_2 ret = Polygon_with_holes_2(outer, holes.begin(), holes.end());
  CGAL::Gps_segment_traits_2<Kernel> traits; // need to be an lvalue
  if ( !are_holes_and_boundary_pairwise_disjoint( ret, traits ) ) {
      fix_sfs_valid_polygon( ret );
  }
  return ret;
}


///
///
///
std::auto_ptr< MultiLineString > straightSkeleton( const Geometry& g, bool autoOrientation, NoValidityCheck, bool innerOnly )
{
    switch ( g.geometryTypeId() ) {
    case TYPE_TRIANGLE:
        return straightSkeleton( g.as< Triangle >().toPolygon(), autoOrientation, innerOnly ) ;

    case TYPE_POLYGON:
        return straightSkeleton( g.as< Polygon >(), autoOrientation, innerOnly ) ;

    case TYPE_MULTIPOLYGON:
        return straightSkeleton( g.as< MultiPolygon >(), autoOrientation, innerOnly ) ;

    default:
        BOOST_THROW_EXCEPTION( InappropriateGeometryException(
               ( boost::format( "Straight Skeleton of %s is not supported" ) % g.geometryType() ).str() ) );
        //return std::auto_ptr< MultiLineString >( new MultiLineString );
    }
}

std::auto_ptr< MultiLineString > straightSkeleton( const Geometry& g, bool autoOrientation, bool innerOnly )
{
    // we rotate the geometry in the plane if needed first if 3D
    // note that by doing that, we loose the absolute precision
    SFCGAL_ASSERT_GEOMETRY_VALIDITY( g );

    return straightSkeleton( g, autoOrientation, NoValidityCheck(), innerOnly );
}
///
///
///
std::auto_ptr< MultiLineString > straightSkeleton( const Polygon& g, bool /*autoOrientation*/, bool innerOnly )
{
    std::auto_ptr< MultiLineString > result( new MultiLineString );

    if ( g.isEmpty() ) {
        return result ;
    }

    Kernel::Vector_2 trans;
    Kernel::Plane_3 plane;
    Polygon_with_holes_2 polygon = preparePolygon( g, trans, plane );
    boost::shared_ptr< Straight_skeleton_2 > skeleton =
        straightSkeleton( polygon );

    if ( !skeleton.get() ) {
        BOOST_THROW_EXCEPTION( Exception( "CGAL failed to create straightSkeleton" ) ) ;
    }

    straightSkeletonToMultiLineString( *skeleton, *result, innerOnly, trans, plane ) ;
    return result ;
}


///
///
///
std::auto_ptr< MultiLineString > straightSkeleton( const MultiPolygon& g, bool /*autoOrientation*/, bool innerOnly )
{
    std::auto_ptr< MultiLineString > result( new MultiLineString );

    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        Kernel::Vector_2 trans;
        Kernel::Plane_3 plane;
        Polygon_with_holes_2 polygon = preparePolygon( g.polygonN( i ), trans, plane );
        boost::shared_ptr< Straight_skeleton_2 > skeleton = straightSkeleton( polygon ) ;

        if ( !skeleton.get() ) {
            BOOST_THROW_EXCEPTION( Exception( "CGAL failed to create straightSkeleton" ) ) ;
        }

        straightSkeletonToMultiLineString( *skeleton, *result, innerOnly, trans, plane ) ;
    }

    return result ;
}

std::auto_ptr< MultiLineString > approximateMedialAxis( const Geometry& g )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( g );

    std::auto_ptr< MultiLineString > mx =
        straightSkeleton( g, false, NoValidityCheck(), true );

    return mx;
}

}//namespace algorithm
}//namespace SFCGAL

