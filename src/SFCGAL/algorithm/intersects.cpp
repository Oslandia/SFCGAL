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
#include <map>
#include <sstream>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/Envelope.h>

#include <CGAL/box_intersection_d.h>

//#define CACHE_TRIANGULATION

namespace SFCGAL {
namespace algorithm
{
	using namespace SFCGAL::detail;
    
	//
	// Type of pa must be of larger dimension than type of pb
	bool _intersects( const PrimitiveHandle<2>& pa, const PrimitiveHandle<2>& pb )
	{
		//
		// Point vs. Point
		//

		if ( pa.handle.which() == PrimitivePoint && pb.handle.which() == PrimitivePoint ) {
			return *boost::get<const CGAL::Point_2<Kernel>* >( pa.handle )
				== *boost::get<const CGAL::Point_2<Kernel>* >( pb.handle );
		}

		//
		// Segment vs. Point
		//

		else if ( pa.handle.which() == PrimitiveSegment && pb.handle.which() == PrimitivePoint ) {
			const CGAL::Segment_2<Kernel>* seg = pa.as<CGAL::Segment_2<Kernel> >();
			const CGAL::Point_2<Kernel>* pt = pb.as<CGAL::Point_2<Kernel> >();
			return seg->has_on( *pt );
		}

		//
		// Segment vs. Segment
		//

		else if ( pa.handle.which() == PrimitiveSegment && pb.handle.which() == PrimitiveSegment ) {
			const CGAL::Segment_2<Kernel>* seg1 = pa.as<CGAL::Segment_2<Kernel> >();
			const CGAL::Segment_2<Kernel>* seg2 = pb.as<CGAL::Segment_2<Kernel> >();
			return CGAL::do_intersect( *seg1, *seg2 );
		}
		//
		// Polygon vs. Point
		//

		else if ( pa.handle.which() == PrimitiveSurface && pb.handle.which() == PrimitivePoint ) {
			// Polygon versus Point
			const CGAL::Polygon_with_holes_2<Kernel> *poly = pa.as<CGAL::Polygon_with_holes_2<Kernel> >();
			const CGAL::Point_2<Kernel> *pt = pb.as<CGAL::Point_2<Kernel> >();

			int b1 = poly->outer_boundary().bounded_side( *pt );
			if ( b1 == CGAL::ON_BOUNDARY ) {
				return true;
			}
			if ( b1 == CGAL::ON_BOUNDED_SIDE ) {
				CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator it;
				for ( it = poly->holes_begin(); it != poly->holes_end(); ++it ) {
					int b = it->bounded_side( *pt );
					if ( b == CGAL::ON_BOUNDED_SIDE ) {
						return false;
					}
				}
			}
			else {
				return false;
			}
			return true;
		}

		//
		// Polygon vs. Segment
		//

		else if ( pa.handle.which() == PrimitiveSurface && pb.handle.which() == PrimitiveSegment ) {
			const CGAL::Polygon_with_holes_2<Kernel> *poly = pa.as<CGAL::Polygon_with_holes_2<Kernel> >();
			const CGAL::Segment_2<Kernel> *seg = pb.as<CGAL::Segment_2<Kernel> >();
			
			// 1. if the segment intersects a boundary of the polygon, returns true
			// 2. else, if one of the point of the segment intersects the polygon, returns true

			GeometrySet<2> segment;
			segment.addSegments( seg, seg+1 );

			// 1.
			GeometrySet<2> boundary;
			boundary.addSegments( poly->outer_boundary().edges_begin(),
			 		      poly->outer_boundary().edges_end() );

			// recurse call
			if ( intersects( boundary, segment ) ) {
			 	return true;
			}
			for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator it = poly->holes_begin();
			      it != poly->holes_end();
			      ++it ) {
				GeometrySet<2> hole;
				hole.addSegments( it->edges_begin(), it->edges_end() );

				// recurse call
				if ( intersects( hole, segment ) ) {
					return true;
				}
			}

			// 2. call the polygon, point version
			CGAL::Point_2<Kernel> pt = seg->source();
			PrimitiveHandle<2> ppoly( poly );
			PrimitiveHandle<2> ppt( &pt );
			return intersects( ppoly, ppt );
		}

		//
		// Polygon vs. Polygon
		//

		else if ( pa.handle.which() == PrimitiveSurface && pb.handle.which() == PrimitiveSurface ) {
			const CGAL::Polygon_with_holes_2<Kernel> *poly1 = pa.as<CGAL::Polygon_with_holes_2<Kernel> >();
			const CGAL::Polygon_with_holes_2<Kernel> *poly2 = pb.as<CGAL::Polygon_with_holes_2<Kernel> >();

			// 1. if rings intersects, returns true
			// 2. else, if poly1 is inside poly2 or poly1 inside poly2 (but not in holes), returns true

			GeometrySet<2> rings1, rings2;
			rings1.addSegments( poly1->outer_boundary().edges_begin(),
					    poly1->outer_boundary().edges_end() );
			for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator it = poly1->holes_begin();
			      it != poly1->holes_end();
			      ++it ) {
				rings1.addSegments( it->edges_begin(), it->edges_end() );
			}
			rings2.addSegments( poly2->outer_boundary().edges_begin(),
					    poly2->outer_boundary().edges_end() );
			for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator it = poly2->holes_begin();
			      it != poly2->holes_end();
			      ++it ) {
				rings2.addSegments( it->edges_begin(), it->edges_end() );
			}

			// 1.
			if ( intersects( rings1, rings2 ) ) {
				return true;
			}

			// 2.
			CGAL::Bbox_2 box1, box2;
			box1 = poly1->bbox();
			box2 = poly2->bbox();
			Envelope e1( box1.xmin(), box1.xmax(), box1.ymin(), box1.ymax() );
			Envelope e2( box2.xmin(), box2.xmax(), box2.ymin(), box2.ymax() );

			// if pa is inside pb
			if ( Envelope::contains( e2, e1 ) ) {
				// is pa inside one of pb's holes ?
				CGAL::Point_2<Kernel> pt = *poly1->outer_boundary().vertices_begin();
				for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator it = poly2->holes_begin();
				      it != poly2->holes_end();
				      ++it ) {
					CGAL::Bounded_side b2 = it->bounded_side( pt );
					if ( b2 == CGAL::ON_BOUNDED_SIDE ) {
						return false;
					}
				}
				return true;
			}
			// if pb is inside pa
			if ( Envelope::contains( e1, e2 ) ) {
				// is pa inside one of pb's holes ?
				CGAL::Point_2<Kernel> pt = *poly2->outer_boundary().vertices_begin();
				for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator it = poly1->holes_begin();
				      it != poly1->holes_end();
				      ++it ) {
					CGAL::Bounded_side b2 = it->bounded_side( pt );
					if ( b2 == CGAL::ON_BOUNDED_SIDE ) {
						return false;
					}
				}
				return true;
			}
			return false;
		}
		return false;
	}


	//
	// intersects of a volume with any other type
	struct intersects_volume_x : public boost::static_visitor<bool>
	{
		const CGAL::Polyhedron_3<Kernel> *polyhedron;

		intersects_volume_x( const CGAL::Polyhedron_3<Kernel>* vol ) : polyhedron(vol) {}

		template <class T>
		bool operator() ( const T *geometry ) const
		{
			// intersection between a solid and a geometry
			// 1. either one of the geometry' point lies inside the solid
			// 2. or the geometry intersects one of the surfaces

			// 1.
#if 0
			SFCGAL::detail::GetPointsVisitor get_point_visitor;
			ga.accept( get_point_visitor );
			for ( size_t i = 0; i < get_point_visitor.points.size(); ++i ) {
				const Point *pt = get_point_visitor.points[i];
				if ( covers3D( solid, *pt )) {
					return true;
				}
			}
#endif
			
			// 2.

			// triangulate the polyhedron_3
			GeometrySet<3> triangles;
			algorithm::triangulate( *polyhedron, triangles );

			GeometrySet<3> g;
			g.addPrimitive( *geometry );

			return intersects( g, triangles );
		}
	};

	//
	// Type of pa must be of larger dimension than type of pb
	bool _intersects( const PrimitiveHandle<3>& pa, const PrimitiveHandle<3>& pb )
	{
		if ( pa.handle.which() == PrimitivePoint && pb.handle.which() == PrimitivePoint ) {
			return *boost::get<const CGAL::Point_3<Kernel>* >( pa.handle )
				== *boost::get<const CGAL::Point_3<Kernel>* >( pb.handle );
		}
		else if ( pa.handle.which() == PrimitiveSegment && pb.handle.which() == PrimitivePoint ) {
			const CGAL::Segment_3<Kernel>* seg = pa.as<CGAL::Segment_3<Kernel> >();
			const CGAL::Point_3<Kernel>* pt = pb.as<CGAL::Point_3<Kernel> >();
			return seg->has_on( *pt );
		}
		if ( pa.handle.which() == PrimitiveVolume ) {
			intersects_volume_x visitor( pa.as<CGAL::Polyhedron_3<Kernel> >() );
			return boost::apply_visitor( visitor, pb.handle );
		}
		if ( pa.handle.which() == PrimitiveSurface && pb.handle.which() == PrimitivePoint ) {
			const CGAL::Triangle_3<Kernel> *tri = pa.as<CGAL::Triangle_3<Kernel> >();
			const CGAL::Point_3<Kernel> *pt = pb.as<CGAL::Point_3<Kernel> >();
			return tri->has_on( *pt );
		}
		if ( pa.handle.which() == PrimitiveSurface && pb.handle.which() == PrimitiveSegment ) {
			const CGAL::Triangle_3<Kernel> *tri = pa.as<CGAL::Triangle_3<Kernel> >();
			const CGAL::Segment_3<Kernel> *seg = pb.as<CGAL::Segment_3<Kernel> >();
			return CGAL::do_intersect( *tri, *seg );
		}
		if ( pa.handle.which() == PrimitiveSurface && pb.handle.which() == PrimitiveSurface ) {
			const CGAL::Triangle_3<Kernel> *tri1 = pa.as<CGAL::Triangle_3<Kernel> >();
			const CGAL::Triangle_3<Kernel> *tri2 = pb.as<CGAL::Triangle_3<Kernel> >();
			return CGAL::do_intersect( *tri1, *tri2 );
		}
		return false;
	}

	template <int Dim>
	bool intersects( const PrimitiveHandle<Dim>& pa, const PrimitiveHandle<Dim>& pb )
	{
		return _intersects( pa, pb );
	}

	//
	// We deal here with symmetric call
	template <int Dim>
	bool dispatch_intersects_sym( const PrimitiveHandle<Dim>& pa, const PrimitiveHandle<Dim>& pb )
	{
		// assume types are ordered by dimension within the boost::variant
		if ( pa.handle.which() >= pb.handle.which() ) {
			return _intersects( pa, pb );
		}
		else {
			return _intersects( pb, pa );
		}
	}

	struct found_an_intersection{};

	template <int Dim>
	struct intersects_cb
	{
		void operator()( const typename PrimitiveBox<Dim>::Type& a,
				 const typename PrimitiveBox<Dim>::Type& b )
		{
			if ( dispatch_intersects_sym( *a.handle(), *b.handle() ) ) {
				throw found_an_intersection();
			}
		}
	};

	template <int Dim>
	bool intersects( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b )
	{
		typename SFCGAL::detail::HandleCollection<Dim>::Type ahandles, bhandles;
		typename SFCGAL::detail::BoxCollection<Dim>::Type aboxes, bboxes;
		a.compute_bboxes( ahandles, aboxes );
		b.compute_bboxes( bhandles, bboxes );

		try {
			intersects_cb<Dim> cb;
			CGAL::box_intersection_d( aboxes.begin(), aboxes.end(),
						  bboxes.begin(), bboxes.end(),
						  cb );
		}
		catch ( found_an_intersection& e ) {
			return true;
		}
		return false;
	}

	template bool intersects<2>( const GeometrySet<2>& a, const GeometrySet<2>& b );
	template bool intersects<3>( const GeometrySet<3>& a, const GeometrySet<3>& b );

	template bool intersects<2>( const PrimitiveHandle<2>& a, const PrimitiveHandle<2>& b );
	template bool intersects<3>( const PrimitiveHandle<3>& a, const PrimitiveHandle<3>& b );

	bool intersects( const Geometry& ga, const Geometry& gb )
	{
		GeometrySet<2> gsa( ga );
		GeometrySet<2> gsb( gb );

		return intersects( gsa, gsb );
	}

	bool intersects3D( const Geometry& ga, const Geometry& gb )
	{
		SFCGAL::detail::GeometrySet<3> gsa( ga );
		SFCGAL::detail::GeometrySet<3> gsb( gb );

		return intersects( gsa, gsb );
	}

}
}
