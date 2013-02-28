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
#include <CGAL/box_intersection_d.h>
#include <CGAL/intersections.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Triangle_2.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Envelope.h>
#include <SFCGAL/algorithm/detail/intersects.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/length.h>
#include <SFCGAL/algorithm/area.h>
#include <SFCGAL/algorithm/triangulate.h>

namespace SFCGAL {
namespace algorithm {
namespace detail {

	using namespace SFCGAL::detail;
    
	//
	// main intersects() code
	//
	// template can be used to factor processing on types
	// These functors are always called with the first argument of a greater geometry dimension
	// than the second one
	// That reduces the list of types to handle
	template <int Dim>
	struct _do_intersect_asym
	{
		// Point versus Point
		bool operator()( const typename TypeForDimension<Dim>::Point* pt1,
				 const typename TypeForDimension<Dim>::Point* pt2 )
		{
			return *pt1 == *pt2;
		}

		// Segment versus Point
		bool operator() ( const typename TypeForDimension<Dim>::Segment* seg,
				  const typename TypeForDimension<Dim>::Point* pt )
		{
			return seg->has_on( *pt );
		}

		// Polygon versus Point
		bool operator() ( const CGAL::Polygon_with_holes_2<Kernel>*poly,
				  const CGAL::Point_2<Kernel>* pt )
		{
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

		// Polygon versus Segment
		bool operator() ( const CGAL::Polygon_with_holes_2<Kernel> *poly,
				  const CGAL::Segment_2<Kernel> *seg )
		{
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
			return  operator()( poly, &pt );
		}

		// Polygon versus Polygon
		bool operator() ( const CGAL::Polygon_with_holes_2<Kernel> *poly1,
				  const CGAL::Polygon_with_holes_2<Kernel> *poly2 )
		{
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

		// Only here to please the compiler
		template <class T>
		bool operator() ( const NoVolume*, const T* )
		{
			return false;
		}

		// Polyhedron versus any other geometry types
		template <class T>
		bool operator() ( const CGAL::Polyhedron_3<Kernel>* polyhedron, const T* geometry )
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

		// For every other types, call CGAL::do_intersect
		template <class T, class U>
		bool operator()( const T* a, const U* b )
		{
			return CGAL::do_intersect( *a, *b );
		}
	};
	
	//
	// This is the boost::variant visitor that deals with symmetric calls
	// It then calls _do_intersect_asym
	template <int Dim>
	struct _do_intersect_sym : public boost::static_visitor<bool>
	{
		template <class X, class Y>
		bool operator() ( const X* a, const Y*b,
				  typename boost::enable_if<IsPrimitiveLarger<X,Y> >::type* = 0 ) const
		{
			// call the aux function
			return _do_intersect_asym<Dim>()( a, b );
		}

		template <class X, class Y>
		bool operator() ( const X* a, const Y*b,
				  typename boost::disable_if<IsPrimitiveLarger<X,Y> >::type* = 0 ) const
		{
			// call the aux function with swapped parameters
			return _do_intersect_asym<Dim>()( b, a );
		}
	};

	struct found_an_intersection{};

	template <int Dim>
	struct intersects_cb
	{
		void operator()( const typename PrimitiveBox<Dim>::Type& a,
				 const typename PrimitiveBox<Dim>::Type& b )
		{
			if ( boost::apply_visitor(_do_intersect_sym<Dim>(), a.handle()->handle, b.handle()->handle) ) {
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
		catch ( algorithm::detail::found_an_intersection& e ) {
			return true;
		}
		return false;
	}

	template bool intersects<2>( const GeometrySet<2>& a, const GeometrySet<2>& b );
	template bool intersects<3>( const GeometrySet<3>& a, const GeometrySet<3>& b );

} // detail
} // algorithm
} // SFCGAL
