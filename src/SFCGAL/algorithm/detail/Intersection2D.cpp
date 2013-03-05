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
#include <CGAL/Segment_2.h>
#include <CGAL/Triangle_2.h>

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/intersections.h>

#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/algorithm/detail/intersects.h>
#include <SFCGAL/algorithm/detail/intersection.h>
#include <SFCGAL/algorithm/triangulate.h>

namespace SFCGAL {
namespace algorithm {
namespace detail {
    
	using namespace SFCGAL::detail;

	//
	// must be called with pa's dimension larger than pb's
	void intersection( const PrimitiveHandle<2>& pa, const PrimitiveHandle<2>& pb,
			   GeometrySet<2>& output, dim_t<2> )
	{
		// everything vs a point
		if ( pb.handle.which() == PrimitivePoint ) {
			if ( algorithm::detail::intersects( pa, pb ) ) {
				output.addPrimitive( *pb.as<CGAL::Point_2<Kernel> >() );
			}
		}
		else if ( pa.handle.which() == PrimitiveSurface && pb.handle.which() == PrimitiveSurface ) {
			const CGAL::Polygon_with_holes_2<Kernel>* poly1 = pa.as<CGAL::Polygon_with_holes_2<Kernel> >();
			const CGAL::Polygon_with_holes_2<Kernel>* poly2 = pb.as<CGAL::Polygon_with_holes_2<Kernel> >();

			// CGAL::intersection does not work when the intersection is a point or a segment
			// We have to call intersection on boundaries first

			GeometrySet<2> gpoly1, gpoly2;
			gpoly1.addSegments( poly1->outer_boundary().edges_begin(), poly1->outer_boundary().edges_end() );
			gpoly2.addSegments( poly2->outer_boundary().edges_begin(), poly2->outer_boundary().edges_end() );
			for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator hit = poly1->holes_begin();
			      hit != poly1->holes_end();
			      ++hit ) {
				gpoly1.addSegments( hit->edges_begin(), hit->edges_end() );
			}
			for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator hit = poly2->holes_begin();
			      hit != poly2->holes_end();
			      ++hit ) {
				gpoly2.addSegments( hit->edges_begin(), hit->edges_end() );
			}
			algorithm::detail::intersection( gpoly1, gpoly2, output );

			// now call on polygon's interiors
			CGAL::intersection( *poly1,
					    *poly2,
					    std::back_inserter( output.surfaces() ) );
		}
		else if ( pa.handle.which() == PrimitiveSegment && pb.handle.which() == PrimitiveSegment ) {
			const CGAL::Segment_2<Kernel> *seg1 = pa.as<CGAL::Segment_2<Kernel> >();
			const CGAL::Segment_2<Kernel> *seg2 = pb.as<CGAL::Segment_2<Kernel> >();
			CGAL::Object interObj = CGAL::intersection( *seg1, *seg2 );
			CGAL::Segment_2<Kernel> interSeg;
			CGAL::Point_2<Kernel> interPoint;
			if ( CGAL::assign( interSeg, interObj ) ) {
				output.segments().push_back( interSeg );
			}
			else if ( CGAL::assign( interPoint, interObj ) ) {
				output.points().push_back( interPoint );
			}
		}
		else if ( pa.handle.which() == PrimitiveSurface && pb.handle.which() == PrimitiveSegment ) {
			const CGAL::Polygon_with_holes_2<Kernel> *poly = pa.as<CGAL::Polygon_with_holes_2<Kernel> >();
			const CGAL::Segment_2<Kernel> *seg = pb.as<CGAL::Segment_2<Kernel> >();

			// shortcut for triangle
			if ( poly->holes_begin() == poly->holes_end() && poly->outer_boundary().size() == 3 ) {
				// no holes and 3 vertices => it is a triangle
				CGAL::Polygon_2<Kernel>::Vertex_iterator vit = poly->outer_boundary().vertices_begin();
				CGAL::Point_2<Kernel> p1( *vit++ );
				CGAL::Point_2<Kernel> p2( *vit++ );
				CGAL::Point_2<Kernel> p3( *vit++ );
				CGAL::Triangle_2<Kernel> tri( p1, p2, p3 );
				CGAL::Object interObj = CGAL::intersection( tri, *seg );
				output.addPrimitive( interObj );
				return;
			} 

			// if it s a regulat polygon, triangulate it and recurse call
			GeometrySet<2> triangles, g;
			algorithm::triangulate( *poly, triangles );
			g.addPrimitive( pb );

			// recurse call
			algorithm::detail::intersection( triangles, g, output );
		}
	}
} // detail
} // algorithm
} // SFCGAL
