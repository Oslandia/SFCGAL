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
#ifndef _SFCGAL_ALGORITHM_DETAIL_INTERSECTION_
#define _SFCGAL_ALGORITHM_DETAIL_INTERSECTION_

#include <SFCGAL/GeometryCollection.h>

#include <SFCGAL/detail/Box_d.h>
#include <boost/shared_ptr.hpp>

namespace SFCGAL {
namespace algorithm {
namespace detail {

	///
	/// Callback functor used with box_intersection_d for 2d and 3d intersections
	///
	template <int Dim>
	struct intersection_cb_base
	{
		//
		// The resulting intersection geometry
		// A shared_ptr seems to be needed here, since intersection_cb is copied
		// a few times (and auto_ptr is not enough)
		boost::shared_ptr< std::list<Geometry*> > geometries;

		// convert to a geometrycollection
		// warning: it is destructive
		std::auto_ptr<GeometryCollection> geometryCollection() const;

		intersection_cb_base();
	};

	template <int TA, int TB, int Dim>
	struct intersection_cb
	{
		intersection_cb_base<Dim> base;
		void operator() ( const typename SFCGAL::detail::ObjectBox<Dim>::Type& a, const typename SFCGAL::detail::ObjectBox<Dim>::Type& b );
	};

} // detail
} // algorithm
} // SFCGAL

#endif
