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
#ifndef SFCGAL_DETAIL_TYPE_FOR_DIMENSION_H
#define SFCGAL_DETAIL_TYPE_FOR_DIMENSION_H

#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Triangle_3.h>

namespace SFCGAL {
	///
    /// Type traits for CGAL types.
    ///
    /// CGAL types cannot be directly parametrized by their dimension.
    /// For instance, there are both a Triangle_2<K> and a Triangle_3<K> type
    ///
    /// TypeForKernel<K, 2>::Point is equivalent to CGAL::Point_2<K> for instance
    /// TypeForDimension<2>::Bbox is equivalent to CGAL::Bbox_2

    ///
    /// Generic traits, all types are defined to 'void'
    template <int Dim>
    struct TypeForDimension
    {
    };

    ///
    /// Specialization for dimension = 2
    template <>
    struct TypeForDimension<2>
    {
    	typedef CGAL::Bbox_2 Bbox;
		typedef Kernel::Point_2 Point;
		typedef Kernel::Segment_2 Segment;
		typedef Kernel::Triangle_2 Triangle;
    };
    
    ///
    /// Specialization for dimension = 2
    template <>
    struct TypeForDimension<3>
    {
    	typedef CGAL::Bbox_3 Bbox;
		typedef Kernel::Point_3 Point;
		typedef Kernel::Segment_3 Segment;
		typedef Kernel::Triangle_3 Triangle;
    };
    
}

#endif
