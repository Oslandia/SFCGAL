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

#ifndef _SFCGAL_DETAIL_GEOMETRY_SET_H_
#define _SFCGAL_DETAIL_GEOMETRY_SET_H_

#include <boost/ptr_container/ptr_vector.hpp>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/TypeForDimension.h>

#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Box_intersection_d/Box_with_handle_d.h>

namespace SFCGAL {
	class Geometry;
namespace detail {

	template <int Dim>
	class GeometrySet
	{
	public:
		typedef std::list<typename TypeForDimension<Dim>::Point>   PointCollection;
		typedef std::list<typename TypeForDimension<Dim>::Segment> SegmentCollection;
		typedef std::list<typename TypeForDimension<Dim>::Surface> SurfaceCollection;
		typedef std::list<typename TypeForDimension<Dim>::Volume>  VolumeCollection;
	private:
		///
		/// Given an input SFCGAL::Geometry, decompose it into CGAL primitives
		void _decompose( const Geometry& g );

		PointCollection _points;
		SegmentCollection _segments;
		SurfaceCollection _surfaces;
		VolumeCollection _volumes;
	};

}
}

#endif
