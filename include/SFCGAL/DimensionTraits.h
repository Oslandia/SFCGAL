#ifndef SFCGAL_DIMENSION_TRAITS_H
#define SFCGAL_DIMENSION_TRAITS_H

#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Triangle_3.h>

namespace SFCGAL
{
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
    template <typename K, int Dim>
    struct TypeForKernel
    {
    };

    ///
    /// Specialization for dimension = 2
    template <typename K>
    struct TypeForKernel<K, 2>
    {
	typedef CGAL::Point_2<K> Point;
	typedef CGAL::Segment_2<K> Segment;
	typedef CGAL::Triangle_2<K> Triangle;
    };
    
    ///
    /// Specialization for dimension = 2
    template <typename K>
    struct TypeForKernel<K, 3>
    {
	typedef CGAL::Point_3<K> Point;
	typedef CGAL::Segment_3<K> Segment;
	typedef CGAL::Triangle_3<K> Triangle;
    };
    
    template <int Dim>
    struct TypeForDimension
    {	
    };

    template <>
    struct TypeForDimension<2>
    {
	typedef CGAL::Bbox_2 Bbox;
    };
	
    template <>
    struct TypeForDimension<3>
    {
	typedef CGAL::Bbox_3 Bbox;
    };
}

#endif
