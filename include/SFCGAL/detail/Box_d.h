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

#ifndef _SFCGAL_DETAIL_BOX_D_H_
#define _SFCGAL_DETAIL_BOX_D_H_

#include <boost/ptr_container/ptr_vector.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/TypeForDimension.h>

#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Box_intersection_d/Box_with_handle_d.h>

namespace SFCGAL {
	class Geometry;
	class Point;
	class Triangle;
namespace detail {

	///
	/// Structure used to store handle to a segment or to a triangle
	///
	struct ObjectHandle
	{
		enum
		{
			Point,
			Segment,
			Triangle
		} ObjectType;
		int type;
		
		union
		{
			const SFCGAL::Point* point;

			struct
			{
				const SFCGAL::Point* start_point;
				const SFCGAL::Point* end_point;
			} segment;

			const SFCGAL::Triangle* triangle;
		};

		///
		/// Constructors
		ObjectHandle( const SFCGAL::Point* p ) :
			type(ObjectHandle::Point)
		{
			point = p;
		}

		ObjectHandle( const SFCGAL::Point* spoint, const SFCGAL::Point* epoint ) :
			type(ObjectHandle::Segment)
		{
			segment.start_point = spoint;
			segment.end_point = epoint;
		}

		ObjectHandle( const SFCGAL::Triangle* triangle ) :
			type(ObjectHandle::Triangle), triangle(triangle) {}

		///
		/// 2D Bounding box conversion
		CGAL::Bbox_2 bbox_2() const;

		///
		/// 3D Bounding box conversion
		CGAL::Bbox_3 bbox_3() const;

		///
		/// Generic way to call bbox_2 or bbox_3
		template <int Dim>
		typename TypeForDimension<Dim>::Bbox bbox_d() const;
	};

	typedef CGAL::Box_intersection_d::Box_with_handle_d<double, 2, ObjectHandle*> Object2Box;
	typedef CGAL::Box_intersection_d::Box_with_handle_d<double, 3, ObjectHandle*> Object3Box;

	template <int Dim>
	struct ObjectBox
	{
		typedef void Type;
	};
	template <>
	struct ObjectBox<2>
	{
		typedef detail::Object2Box Type;
	};
	template <>
	struct ObjectBox<3>
	{
		typedef detail::Object3Box Type;
	};

	///
	/// Auxiliary function used to fill up vectors of handle and boxes for segments, triangle and triangulated surfaces
	///
	template <int Dim> // 2 or 3
	void to_boxes( const Geometry& g, std::list<ObjectHandle>& handles, std::vector<typename ObjectBox<Dim>::Type>& boxes );


}
}

#endif
