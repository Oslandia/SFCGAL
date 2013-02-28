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

#include <SFCGAL/Kernel.h>
#include <SFCGAL/algorithm/intersects.h>

#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polyhedron_3.h>

#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>
#include <CGAL/Boolean_set_operations_2.h>

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_polyhedron_triangle_primitive.h>

#include <CGAL/box_intersection_d.h>

#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/all.h>
#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/algorithm/detail/intersects.h>
#include <SFCGAL/algorithm/covers.h>

typedef CGAL::Point_3< SFCGAL::Kernel >          Point_3;
typedef CGAL::Segment_3< SFCGAL::Kernel >        Segment_3;
typedef CGAL::Triangle_3< SFCGAL::Kernel >       Triangle_3;
typedef CGAL::Polyhedron_3< SFCGAL::Kernel >     Polyhedron;
typedef CGAL::Nef_polyhedron_3< SFCGAL::Kernel > Nef_polyhedron;

//#define CACHE_TRIANGULATION

typedef std::vector<Segment_3> Segments;
typedef CGAL::Box_intersection_d::Box_with_handle_d<double, 3, Segments::const_iterator> SegmentBox;
	
namespace SFCGAL {
namespace algorithm
{
	bool intersects3D( const Geometry& ga, const Geometry& gb )
	{
		SFCGAL::detail::GeometrySet<3> gsa( ga );
		SFCGAL::detail::GeometrySet<3> gsb( gb );

		return algorithm::detail::intersects( gsa, gsb );
	}
}
}
