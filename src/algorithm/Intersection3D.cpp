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


#include <CGAL/intersections.h>

#include <CGAL/intersection_of_Polyhedra_3.h>
#include <CGAL/intersection_of_Polyhedra_3_refinement_visitor.h>
#include <CGAL/corefinement_operations.h>

#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/detail/triangulate/triangulateInGeometrySet.h>

#include <CGAL/IO/Polyhedron_iostream.h>

#include <CGAL/Point_inside_polyhedron_3.h>

using namespace SFCGAL::detail;

namespace SFCGAL {
namespace algorithm {

void _intersection_solid_segment( const PrimitiveHandle<3>& pa, const PrimitiveHandle<3>& pb, GeometrySet<3>& output )
{
    //typedef CGAL::Polyhedral_mesh_domain_3<MarkedPolyhedron, Kernel> Mesh_domain;

    const MarkedPolyhedron* ext_poly = pa.as<MarkedPolyhedron>();
    BOOST_ASSERT( ext_poly->is_closed() );
    const CGAL::Segment_3<Kernel>* segment = pb.as<CGAL::Segment_3<Kernel> >();

    MarkedPolyhedron* ext_poly_nc = const_cast<MarkedPolyhedron*>( ext_poly );
    CGAL::Point_inside_polyhedron_3<MarkedPolyhedron, Kernel> is_in_ext( *ext_poly_nc );

    GeometrySet<3> triangles;
    GeometrySet<3> spoint( segment->source() );
    GeometrySet<3> tpoint( segment->target() );
    triangulate::triangulate( *ext_poly, triangles );

    bool source_inside = ( is_in_ext( segment->source() ) != CGAL::ON_UNBOUNDED_SIDE ) || intersects( triangles, spoint );
    bool target_inside = ( is_in_ext( segment->target() ) != CGAL::ON_UNBOUNDED_SIDE ) || intersects( triangles, tpoint );

    if ( source_inside && target_inside ) {
        // the entire segment intersects the volume, return the segment
        output.addPrimitive( pb );
    }
    else {
        GeometrySet<3> triangles, g;
        triangulate::triangulate( *ext_poly, triangles );
        g.addPrimitive( pb );
        GeometrySet<3> inter;

        // call recursively on triangulated polyhedron
        intersection( g, triangles, inter );

        if ( ! inter.points().empty() ) {
            // the intersection is a point, build a segment from that point to the other end
            if ( !source_inside && target_inside ) {
                CGAL::Segment_3<Kernel> interSeg( inter.points().begin()->primitive(),
                                                  segment->target() );

                if ( interSeg.source() == interSeg.target() ) {
                    output.addPrimitive( segment->target() );
                }
                else {
                    output.addPrimitive( interSeg );
                }
            }
            else if ( source_inside && !target_inside ) {
                CGAL::Segment_3<Kernel> interSeg( segment->source(),
                                                  inter.points().begin()->primitive() );

                if ( interSeg.source() == interSeg.target() ) {
                    output.addPrimitive( segment->source() );
                }
                else {
                    output.addPrimitive( interSeg );
                }
            }
            else { // !source_inside && !target_inside => intersection on a point
                output.addPrimitive( inter.points().begin()->primitive() );
            }
        }

        if ( ! inter.segments().empty() ) {
            // the intersection is a segment
            output.addPrimitive( inter.segments().begin()->primitive() );
        }
    }
}


#if CGAL_VERSION_NR < 1040301000 // version 4.3
// Before 4.3, we pass CGAL::Tag_true to mark boundary halfedges
typedef CGAL::Node_visitor_refine_polyhedra<MarkedPolyhedron, Kernel, CGAL::Tag_true> Split_visitor;
#else
// Starting with 4.3, we must now pass a property_map
template<class Polyhedron>
struct Edge_mark_property_map{
  typedef bool value_type;
  typedef value_type reference;
  typedef std::pair<typename Polyhedron::Halfedge_handle,Polyhedron*> key_type;
  typedef boost::read_write_property_map_tag category;

  friend reference get(Edge_mark_property_map,const key_type& key) {return key.first->mark;}
  friend void put(Edge_mark_property_map,key_type key,value_type v) {key.first->mark=v;}
};
typedef CGAL::Node_visitor_refine_polyhedra<MarkedPolyhedron,Kernel,Edge_mark_property_map<MarkedPolyhedron> > Split_visitor;
#endif

typedef std::vector<Kernel::Point_3> Polyline_3;

struct Is_not_marked {
    bool operator()( MarkedPolyhedron::Halfedge_const_handle h ) const {
        return !h->mark;
    }
};

void _intersection_solid_triangle( const MarkedPolyhedron& pa, const CGAL::Triangle_3<Kernel>& tri, GeometrySet<3>& output )
{
    BOOST_ASSERT( pa.is_closed() );
    Split_visitor visitor( NULL, true );

    MarkedPolyhedron polyb;
    polyb.make_triangle( tri.vertex( 0 ), tri.vertex( 1 ), tri.vertex( 2 ) );

    MarkedPolyhedron& polya = const_cast<MarkedPolyhedron&>( pa );
    std::list<Polyline_3> polylines;
    CGAL::Intersection_of_Polyhedra_3<MarkedPolyhedron,Kernel, Split_visitor> intersect_polys( visitor );
    intersect_polys( polya, polyb, std::back_inserter( polylines ) );

    CGAL::Point_inside_polyhedron_3<MarkedPolyhedron, Kernel> point_inside_q( polya );

    if ( polylines.size() == 0 ) {
        // no surface intersection
        // if one of the point of the triangle is inside the polyhedron,
        // the triangle is inside
        if ( point_inside_q( tri.vertex( 0 ) ) != CGAL::ON_UNBOUNDED_SIDE ) {
            output.addPrimitive( tri );
            return;
        }

        return;
    }

    // triangle decomposition
    std::list<MarkedPolyhedron> decomposition;
    Is_not_marked criterion;
    CGAL::internal::extract_connected_components( polyb, criterion, std::back_inserter( decomposition ) );

    bool hasSurface = false;

    for ( std::list<MarkedPolyhedron>::iterator it = decomposition.begin(); it != decomposition.end(); ++it ) {
        // char fname[256];
        // sprintf(fname, "decompo%d.off", k++);
        // std::ofstream decompo(fname);
        // decompo << *it;

        // take a point on the component and tests if its inside the other polyhedron
        //
        CGAL::Point_3<Kernel> test_point;

        if ( it->size_of_facets() == 1 ) {
            //
            // If we only have a facet, take the centroid (cannot take a point on the intersection)
            CGAL::Point_3<Kernel> q[3];
            MarkedPolyhedron::Point_iterator pit = it->points_begin();

            for ( size_t i = 0; i < 3; ++i, ++pit ) {
                q[i] = *pit;
            }

            test_point = CGAL::centroid( q[0], q[1], q[2] );
        }
        else {
            //
            // Take a point not an the border
            for ( MarkedPolyhedron::Halfedge_iterator hit = it->halfedges_begin(); hit != it->halfedges_end(); ++hit ) {
                if ( !hit->is_border_edge() ) {
                    // take the center of the edge
                    test_point = CGAL::midpoint( hit->vertex()->point(), hit->prev()->vertex()->point() );
                    break;
                }
            }
        }

        // point inside volume or on surface
        bool point_is_inside = false;
        // on surface test
        {
            // triangulate the polyhedron
            GeometrySet<3> triangles;
            GeometrySet<3> point( test_point );

            triangulate::triangulate( polya, triangles );
            bool point_on_surface = intersects( triangles, point );

            if ( point_on_surface ) {
                point_is_inside = true;
            }
            else {
                // strictly inside test
                bool point_inside_volume = point_inside_q( test_point ) != CGAL::ON_UNBOUNDED_SIDE;

                if ( point_inside_volume ) {
                    point_is_inside = true;
                }
            }
        }

        if ( point_is_inside ) {
            // we know it is a planar intersection
            hasSurface = true;
            output.addPrimitive( *it, FLAG_IS_PLANAR );
        }
    }

    if ( hasSurface ) {
        return;
    }

    for ( std::list<Polyline_3>::const_iterator lit = polylines.begin(); lit != polylines.end(); ++lit ) {
        if ( lit->size() == 1 ) {
            // it's a point
            output.addPrimitive( ( *lit )[0] );
        }
        else {
            for ( size_t k = 1; k < lit->size(); ++k ) {
                CGAL::Segment_3<Kernel> seg( ( *lit )[k-1], ( *lit )[k] );
                output.addPrimitive( seg );
            }
        }
    }
}

void _intersection_solid_solid( const MarkedPolyhedron& pa, const MarkedPolyhedron& pb, GeometrySet<3>& output )
{
    // 1. find intersections on surfaces
    // CGAL corefinement or polyhedra_intersection do not return polygon intersections between two solids
    // they only return points, lines and volumes (but no surfaces ...)
    {
        // call CGAL::intersection on triangles
        GeometrySet<3> gsa, gsb;
        // convert polyhedra to geometry sets
        // (no actual triangulation is done if the polyhedra are pure_triangle()
        triangulate::triangulate( pa, gsa );
        triangulate::triangulate( pb, gsb );
        // "recurse" call on triangles
        algorithm::intersection( gsa, gsb, output );
    }

    // 2. find intersections in volumes
    {
        typedef CGAL::Polyhedron_corefinement<MarkedPolyhedron> Corefinement;
        MarkedPolyhedron& polya = const_cast<MarkedPolyhedron&>( pa );
        MarkedPolyhedron& polyb = const_cast<MarkedPolyhedron&>( pb );

        Corefinement coref;
        CGAL::Emptyset_iterator no_polylines;
        // vector of <Polyhedron, tag>
        typedef std::vector<std::pair<MarkedPolyhedron*, int> > Decomposition;
        Decomposition result;
        coref( polya, polyb, no_polylines, std::back_inserter( result ), Corefinement::Intersection_tag );

        // empty intersection
        if ( result.size() == 0 ) {
            return ;
        }

        // else, we have an intersection
        MarkedPolyhedron* res_poly = result[0].first;
        output.addPrimitive( *res_poly );
        delete res_poly;
    }
}

//
// must be called with pa's dimension larger than pb's
void intersection( const PrimitiveHandle<3>& pa, const PrimitiveHandle<3>& pb,
                   GeometrySet<3>& output, dim_t<3> )
{
    // everything vs a point
    if ( pb.handle.which() == PrimitivePoint ) {
        if ( algorithm::intersects( pa, pb ) ) {
            output.addPrimitive( *boost::get<const TypeForDimension<3>::Point*>( pb.handle ) );
        }
    }
    else if ( pa.handle.which() == PrimitiveSegment && pb.handle.which() == PrimitiveSegment ) {
        const CGAL::Segment_3<Kernel> *seg1 = pa.as<CGAL::Segment_3<Kernel> >();
        const CGAL::Segment_3<Kernel> *seg2 = pb.as<CGAL::Segment_3<Kernel> >();
        CGAL::Object interObj = CGAL::intersection( *seg1, *seg2 );
        output.addPrimitive( interObj );
    }
    else if ( pa.handle.which() == PrimitiveSurface ) {
        const CGAL::Triangle_3<Kernel> *tri1 = pa.as<CGAL::Triangle_3<Kernel> >();

        if ( pb.handle.which() == PrimitiveSegment ) {
            const CGAL::Segment_3<Kernel> *seg2 = pb.as<CGAL::Segment_3<Kernel> >();
            CGAL::Object interObj = CGAL::intersection( *tri1, *seg2 );
            output.addPrimitive( interObj );
        }
        else if ( pb.handle.which() == PrimitiveSurface ) {
            const CGAL::Triangle_3<Kernel> *tri2 = pb.as<CGAL::Triangle_3<Kernel> >();
            CGAL::Object interObj = CGAL::intersection( *tri1, *tri2 );
            output.addPrimitive( interObj, /* pointsAsRing */ true );
        }
    }
    else if ( pa.handle.which() == PrimitiveVolume ) {
        if ( pb.handle.which() == PrimitiveSegment ) {
            _intersection_solid_segment( pa, pb, output );
        }
        else if ( pb.handle.which() == PrimitiveSurface ) {
            _intersection_solid_triangle( *pa.as<MarkedPolyhedron>(),
                                          *pb.as<CGAL::Triangle_3<Kernel> >(),
                                          output );
        }
        else if ( pb.handle.which() == PrimitiveVolume ) {
            const MarkedPolyhedron& sa = *pa.as<MarkedPolyhedron>();
            const MarkedPolyhedron& sb = *pb.as<MarkedPolyhedron>();
            BOOST_ASSERT( sa.is_closed() );
            BOOST_ASSERT( sb.is_closed() );
            _intersection_solid_solid( sa, sb, output );
        }
    }
}

} // algorithm
} // SFCGAL
