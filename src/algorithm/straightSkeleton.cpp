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
#include <SFCGAL/algorithm/translate.h>

#include <CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>
#include <CGAL/Straight_skeleton_converter_2.h>

namespace SFCGAL {
namespace algorithm {

typedef Kernel::Point_2                    Point_2 ;
typedef CGAL::Polygon_2<Kernel>            Polygon_2 ;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2 ;
typedef CGAL::Straight_skeleton_2<Kernel>  Straight_skeleton_2 ;



template<class K>
void straightSkeletonToMultiLineString(
    const CGAL::Straight_skeleton_2<K>& ss,
    MultiLineString& result,
    bool innerOnly,
    Kernel::Vector_2& translate
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

        std::auto_ptr<LineString> ls ( new LineString(
                                Point( it->opposite()->vertex()->point() ),
                                Point( it->vertex()->point() ) )
                         );
        algorithm::translate( *ls, translate );
        result.addGeometry( ls.release() );
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
  boost::shared_ptr< Straight_skeleton_2 > ret;
  if ( sk ) ret = CGAL::convert_straight_skeleton_2< Straight_skeleton_2 > ( *sk ) ;
  return ret;
}

// Throw an exception if any two polygon rings touch,
// since CGAL segfaults in that case.
// @todo find upstream reference to find out exact case
// See https://github.com/Oslandia/SFCGAL/issues/75
void
checkNoTouchingHoles( const Polygon& g )
{
    const size_t numRings =  g.numRings();

    for ( size_t ri=0; ri < numRings-1; ++ri ) {
        for ( size_t rj=ri+1; rj < numRings; ++rj ) {
            std::auto_ptr<Geometry> inter = g.is3D()
                                            ? intersection3D( g.ringN( ri ), g.ringN( rj ) )
                                            : intersection( g.ringN( ri ), g.ringN( rj ) );

            // @note this check would accept rings touching at
            //       more than a single point, which may be
            //       still dangerous. @todo find out !
            if ( ! inter->isEmpty() && inter->is< Point >() ) {
                BOOST_THROW_EXCEPTION( NotImplementedException(
                                           "straight skeleton of Polygon with touching interior rings is not implemented"
                                       ) );
            }
        }
    }
}

Polygon_with_holes_2
preparePolygon( const Polygon& poly, Kernel::Vector_2& trans )
{
  checkNoTouchingHoles( poly );
  Envelope env = poly.envelope();
  trans = Kernel::Vector_2(-env.xMin(), -env.yMin());

  // @todo: avoid cloning !
  std::auto_ptr<Polygon> cloned ( poly.clone() );
  algorithm::translate( *cloned, trans );
  Polygon_with_holes_2 ret = cloned->toPolygon_with_holes_2();
  trans = -trans;

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
        return std::auto_ptr< MultiLineString >( new MultiLineString );
    }
}

std::auto_ptr< MultiLineString > straightSkeleton( const Geometry& g, bool autoOrientation, bool innerOnly )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( g );

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
    Polygon_with_holes_2 polygon = preparePolygon( g, trans );
    boost::shared_ptr< Straight_skeleton_2 > skeleton =
        straightSkeleton( polygon );

    if ( !skeleton.get() ) {
        BOOST_THROW_EXCEPTION( Exception( "CGAL failed to create straightSkeleton" ) ) ;
    }

    straightSkeletonToMultiLineString( *skeleton, *result, innerOnly, trans ) ;
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
        Polygon_with_holes_2 polygon = preparePolygon( g.polygonN( i ), trans );
        boost::shared_ptr< Straight_skeleton_2 > skeleton = straightSkeleton( polygon ) ;

        if ( !skeleton.get() ) {
            BOOST_THROW_EXCEPTION( Exception( "CGAL failed to create straightSkeleton" ) ) ;
        }

        straightSkeletonToMultiLineString( *skeleton, *result, innerOnly, trans ) ;
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

