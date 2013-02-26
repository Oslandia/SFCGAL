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


namespace SFCGAL {
namespace algorithm {
namespace detail {
    
	using namespace SFCGAL::detail;

	template <int Dim>
	class intersects_cb<ObjectHandle::Point, ObjectHandle::Triangle, Dim>
	{
	public:
		void operator()( const typename ObjectBox<Dim>::Type& a,
				 const typename ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Point Point_d;
			typedef typename TypeForDimension<Dim>::Triangle Triangle_d;
			Point_d point = a.handle()->point->template toPoint_d<Dim>();
			Triangle_d tri = b.handle()->triangle->template toTriangle_d<Dim>();
			if ( CGAL::do_intersect( point, tri )) {
				throw found_point_triangle_intersection();
			}
		}
	};

	template <int Dim>
	class intersects_cb<ObjectHandle::Segment, ObjectHandle::Segment, Dim>
	{
	public:
		void operator()( const typename ObjectBox<Dim>::Type& a,
				 const typename ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Segment Segment_d;
			Segment_d sega( a.handle()->segment.start_point->template toPoint_d<Dim>(),
					a.handle()->segment.end_point->template toPoint_d<Dim>());
			Segment_d segb( b.handle()->segment.start_point->template toPoint_d<Dim>(),
					b.handle()->segment.end_point->template toPoint_d<Dim>());
			if ( CGAL::do_intersect( sega, segb )) {
				throw found_segment_segment_intersection();
			}
		}
	};

	template <int Dim>
	class intersects_cb<ObjectHandle::Segment, ObjectHandle::Triangle, Dim>
	{
	public:
		void operator()( const typename ObjectBox<Dim>::Type& a,
				 const typename ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Segment Segment_d;
			typedef typename TypeForDimension<Dim>::Triangle Triangle_d;
			Segment_d sega( a.handle()->segment.start_point->template toPoint_d<Dim>(),
					a.handle()->segment.end_point->template toPoint_d<Dim>());
			Triangle_d tri = b.handle()->triangle->template toTriangle_d<Dim>();
			if ( CGAL::do_intersect( sega, tri )) {
				throw found_segment_triangle_intersection();
			}
		}
	};

	template <int Dim>
	class intersects_cb<ObjectHandle::Triangle, ObjectHandle::Triangle, Dim>
	{
	public:
		void operator()( const typename ObjectBox<Dim>::Type& a,
				 const typename ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Triangle Triangle_d;
			Triangle_d tria( a.handle()->triangle->template toTriangle_d<Dim>() );
			Triangle_d trib( b.handle()->triangle->template toTriangle_d<Dim>() );
			if (CGAL::do_intersect( tria, trib )) {
				throw found_triangle_triangle_intersection();
			}

		}
	};
	/// template instanciations
	template class intersects_cb<ObjectHandle::Point, ObjectHandle::Triangle, 2>;
	template class intersects_cb<ObjectHandle::Point, ObjectHandle::Triangle, 3>;
	template class intersects_cb<ObjectHandle::Segment, ObjectHandle::Segment, 2>;
	template class intersects_cb<ObjectHandle::Segment, ObjectHandle::Segment, 3>;
	template class intersects_cb<ObjectHandle::Segment, ObjectHandle::Triangle, 2>;
	template class intersects_cb<ObjectHandle::Segment, ObjectHandle::Triangle, 3>;
	template class intersects_cb<ObjectHandle::Triangle, ObjectHandle::Triangle, 2>;
	template class intersects_cb<ObjectHandle::Triangle, ObjectHandle::Triangle, 3>;
} // detail
} // algorithm
} // SFCGAL
