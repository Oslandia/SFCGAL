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
#include <SFCGAL/algorithm/detail/intersection.h>
#include <SFCGAL/algorithm/triangulate.h>

namespace SFCGAL {
namespace algorithm {
namespace detail {
    
	using namespace SFCGAL::detail;

	void _intersection_solid_segment( const PrimitiveHandle<3>& pa, const PrimitiveHandle<3>& pb, GeometrySet<3>& output )
	{
		typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
		typedef CGAL::Polyhedral_mesh_domain_3<Polyhedron, Kernel> Mesh_domain;

		const Polyhedron* ext_poly = pa.as<Polyhedron>();
		const CGAL::Segment_3<Kernel>* segment = pb.as<CGAL::Segment_3<Kernel> >();
		Mesh_domain ext_domain( *ext_poly );
		Mesh_domain::Is_in_domain is_in_ext( ext_domain );

		bool source_inside = is_in_ext( segment->source() );
		bool target_inside = is_in_ext( segment->target() );

		if ( source_inside && target_inside ) {
			// the entire segment intersects the volume, return the segment
			output.addPrimitive( pa );
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
					output.addPrimitive( interSeg );
				}
				else if ( source_inside && !target_inside ) {
					CGAL::Segment_3<Kernel> interSeg( segment->source(), *inter.points().begin() );
					output.addPrimitive( interSeg );
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
				output.addPrimitive( interObj );
			}
		}
		else if ( pa.handle.which() == PrimitiveVolume ) {
			if ( pb.handle.which() == PrimitiveSegment ) {
				_intersection_solid_segment( pa, pb, output );
			}
		}
	}

} // detail
} // algorithm
} // SFCGAL
