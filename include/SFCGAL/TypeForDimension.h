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
