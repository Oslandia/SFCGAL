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

#include <CGAL/intersections.h>

#include <CGAL/intersection_of_Polyhedra_3.h>
#include <CGAL/intersection_of_Polyhedra_3_refinement_visitor.h>
#include <CGAL/corefinement_operations.h>

#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/triangulate.h>

#include <CGAL/IO/Polyhedron_iostream.h>

namespace SFCGAL {
namespace algorithm {
    
	using namespace SFCGAL::detail;

	void _intersection_solid_segment( const PrimitiveHandle<3>& pa, const PrimitiveHandle<3>& pb, GeometrySet<3>& output )
	{
		typedef CGAL::Polyhedral_mesh_domain_3<MarkedPolyhedron, Kernel> Mesh_domain;

		const MarkedPolyhedron* ext_poly = pa.as<MarkedPolyhedron>();
		const CGAL::Segment_3<Kernel>* segment = pb.as<CGAL::Segment_3<Kernel> >();
		Mesh_domain ext_domain( *ext_poly );
		Mesh_domain::Is_in_domain is_in_ext( ext_domain );

		std::cout << "solid vs segment" << *segment << std::endl;
		bool source_inside = is_in_ext( segment->source() );
		bool target_inside = is_in_ext( segment->target() );
		std::cout << "source_inside: " << source_inside << std::endl;
		std::cout << "target_inside: " << target_inside << std::endl;

		if ( source_inside && target_inside ) {
			// the entire segment intersects the volume, return the segment
			std::cout << "add the segment in output" << std::endl;
			output.addPrimitive( pb );
		}
		else {
			GeometrySet<3> triangles, g;
			algorithm::triangulate( *ext_poly, triangles );
			g.addPrimitive( pb );
			GeometrySet<3> inter;

			// call recursively on triangulated polyhedron
			intersection( g, triangles, inter );
			if ( ! inter.points().empty() ) {
				// the intersection is a point, build a segment from that point to the other end
				if (!source_inside && target_inside) {
					CGAL::Segment_3<Kernel> interSeg( *inter.points().begin(), segment->target() );
					if ( interSeg.source() == interSeg.target() ) {
						output.addPrimitive( segment->target() );
					}
					else {
						output.addPrimitive( interSeg );
					}
				}
				else if ( source_inside && !target_inside ) {
					CGAL::Segment_3<Kernel> interSeg( segment->source(), *inter.points().begin() );
					if ( interSeg.source() == interSeg.target() ) {
						output.addPrimitive( segment->source() );
					}
					else
					{
						output.addPrimitive( interSeg );
					}
				}
				else { // !source_inside && !target_inside => intersection on a point
					output.addPrimitive( *inter.points().begin() );
				}
			}
			if ( ! inter.segments().empty() ) {
				// the intersection is a segment
				output.addPrimitive( *inter.segments().begin() );
			}
		}
		std::cout << "in intersection_solid_segment: " << output;
	}


	typedef CGAL::Node_visitor_refine_polyhedra<MarkedPolyhedron, Kernel, CGAL::Tag_true> Split_visitor;
	typedef std::vector<Kernel::Point_3> Polyline_3;

	typedef CGAL::Polyhedral_mesh_domain_3<MarkedPolyhedron, Kernel> Mesh_domain;
	
	struct Is_not_marked{
		bool operator()(MarkedPolyhedron::Halfedge_const_handle h) const{
			return !h->mark;
		}
	};
	
	void _intersection_solid_triangle( const MarkedPolyhedron& pa, const CGAL::Triangle_3<Kernel>& tri, GeometrySet<3>& output )
	{
		bool isVolume = false;
		Split_visitor visitor( NULL, true );

		MarkedPolyhedron polyb;
		polyb.make_triangle( tri.vertex(0), tri.vertex(1), tri.vertex(2) );

		MarkedPolyhedron& polya = const_cast<MarkedPolyhedron&>(pa);
		std::list<Polyline_3> polylines;
		CGAL::Intersection_of_Polyhedra_3<MarkedPolyhedron,Kernel, Split_visitor> intersect_polys(visitor);
		intersect_polys( polya, polyb, std::back_inserter(polylines) );
		std::cout << "# of polylines: " << polylines.size() << std::endl;
		#if 1
		for ( std::list<Polyline_3>::const_iterator it = polylines.begin(); it != polylines.end(); ++it ) {
			std::cout << "polyline: ";
			for ( size_t j = 0; j < it->size(); ++j ) {
				std::cout << (*it)[j] << ",";
			}
			std::cout << std::endl;
		}
		#endif

		std::ofstream fa("polya.off");
		std::ofstream fb("polyb.off");
		fa << polya;
		fb << polyb;

		if ( polylines.size() == 0 ) {
			// no intersection
			std::cout << "no intersection" << std::endl;
			return;
		}

		// triangle decomposition
		std::list<MarkedPolyhedron> decomposition;
		Is_not_marked criterion;
		CGAL::internal::extract_connected_components( polyb, criterion, std::back_inserter(decomposition));

		Mesh_domain ext_domain( polya );
		Mesh_domain::Is_in_domain point_inside_q( ext_domain );
		//		CGAL::Point_inside_polyhedron_3<Polyhedron, Kernel> point_inside_q( const_cast<Polyhedron&>(polyb) );

		std::cout << "# of decomposition: " << decomposition.size() << std::endl;
		for ( std::list<MarkedPolyhedron>::iterator it = decomposition.begin(); it != decomposition.end(); ++it ) {
			std::ofstream decompo("decompo.off");
			decompo << *it;

			// take a point on the component and tests if its inside the other polyhedron
			//
			CGAL::Point_3<Kernel> test_point;
			std::cout << "# of facets: " << it->size_of_facets() << std::endl;
			std::cout << "# of vertices: " << it->size_of_vertices() << std::endl;
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
						//						std::cout << "take the center of the edge as test_point " << test_point << std::endl;
						break;
					}
				}
			}
			
			std::cout << "test_point = " << test_point << std::endl;
			// point inside volume or on surface
			bool point_inside_volume =  point_inside_q( test_point );

			if ( point_inside_volume ) {
				//				if ( is_volume ) {
				//					return surface_polyhedron_to_geometry( *it );
				//				}
				//				return planar_polyhedron_to_geometry( *it );


				// we know it is a planar intersection
				it->set_is_planar( true );
				output.addPrimitive( &*it );
				return;
			}
		}

		for ( std::list<Polyline_3>::const_iterator lit = polylines.begin(); lit != polylines.end(); ++lit ) {
			if ( lit->size() == 1 ) {
				// it's a point
				output.addPrimitive( (*lit)[0] );
			}
			else {
				for ( size_t k = 1; k < lit->size(); ++k ) {
					CGAL::Segment_3<Kernel> seg( (*lit)[k-1], (*lit)[k] );
					output.addPrimitive( seg );
				}
			}
		}
	}
	//
	// must be called with pa's dimension larger than pb's
	void intersection( const PrimitiveHandle<3>& pa, const PrimitiveHandle<3>& pb,
			   GeometrySet<3>& output, dim_t<3> )
	{
		std::cout << "intersection " << pa.handle.which() << " vs. " << pb.handle.which() << std::endl;
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
				output.addPrimitive( interObj );
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
				// TODO
			}
			else if ( pb.handle.which() == PrimitiveVolume ) {
				// TODO
			}
		}
	}

} // algorithm
} // SFCGAL
