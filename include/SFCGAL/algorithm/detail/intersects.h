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
#ifndef _SFCGAL_ALGORITHM_DETAIL_INTERSECTS_
#define _SFCGAL_ALGORITHM_DETAIL_INTERSECTS_

#include <SFCGAL/detail/Box_d.h>
#include <boost/shared_ptr.hpp>

namespace SFCGAL {
namespace algorithm {
namespace detail {

	struct found_intersection {};
	struct found_point_triangle_intersection : public found_intersection {};
	struct found_point_segment_intersection : public found_intersection {};
	struct found_segment_segment_intersection : public found_intersection {};
	struct found_segment_triangle_intersection : public found_intersection {};
	struct found_triangle_triangle_intersection : public found_intersection {};
	
	///
	/// Callback function used with box_intersection_d for 2d and 3d intersection test
	/// Throws an exception if an intersection has been found
	///
	template <int TA, int TB, int Dim>
	class intersects_cb
	{
	public:
		void operator()( const typename SFCGAL::detail::ObjectBox<Dim>::Type& a,
				 const typename SFCGAL::detail::ObjectBox<Dim>::Type& b );
	};

} // detail
} // algorithm
} // SFCGAL

#endif
