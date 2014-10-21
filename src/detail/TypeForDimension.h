/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SFCGAL_DETAIL_TYPE_FOR_DIMENSION_H
#define SFCGAL_DETAIL_TYPE_FOR_DIMENSION_H

#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polyhedron_3.h>

namespace SFCGAL {
namespace detail {
///
/// Type traits for CGAL types.
///
/// CGAL types cannot be directly parametrized by their dimension.
/// For instance, there are both a Triangle_2<K> and a Triangle_3<K> type
///
/// TypeForKernel<K, 2>::Point is equivalent to CGAL::Point_2<K> for instance
/// TypeForDimension<2>::Bbox is equivalent to CGAL::Bbox_2

struct SFCGAL_API NoVolume {};

///
/// Generic traits, default dimension is 2
template <int Dim>
struct TypeForDimension {
    typedef CGAL::Bbox_2                        Bbox;
    typedef Kernel::Point_2                     Point;
    typedef Kernel::Segment_2                   Segment;
    typedef Kernel::Triangle_2                  Triangle;
    typedef CGAL::Polygon_with_holes_2<Kernel>  Surface;
    typedef NoVolume                            Volume;
};


///
/// Extended polyhedron_3 type with a boolean marker on halfedges
/// This is used internally for polyhedra boolean operations
template <class Refs>
struct Halfedge_with_mark : public CGAL::HalfedgeDS_halfedge_base<Refs> {
    Halfedge_with_mark()
        : CGAL::HalfedgeDS_halfedge_base<Refs>(), mark( false )
    {}

    bool mark;   // A boundary marker for faces with different status
    void set_mark() { // this will be called by Intersection_of_Polyhedra_3
        mark = true;
    }
};

// An items type using my halfedge.
struct SFCGAL_API Items_with_mark_on_hedge : public CGAL::Polyhedron_items_3 {
    template <class Refs, class Traits>
    struct Halfedge_wrapper {
        typedef Halfedge_with_mark<Refs> Halfedge;
    };
};


typedef CGAL::Polyhedron_3<Kernel, Items_with_mark_on_hedge> MarkedPolyhedron;

///
/// Specialization for dimension = 3
template <>
struct TypeForDimension<3> {
    typedef CGAL::Bbox_3               Bbox;
    typedef Kernel::Point_3            Point;
    typedef Kernel::Segment_3          Segment;
    typedef Kernel::Triangle_3         Triangle;
    typedef Kernel::Triangle_3         Surface;
    typedef MarkedPolyhedron           Volume;
};

///
/// Another way of looking at TypeForDimension<Dim>::Point
template <int Dim>
struct Point_d {
    typedef typename TypeForDimension<Dim>::Point Type;
};
///
/// Another way of looking at TypeForDimension<Dim>::Segment
template <int Dim>
struct Segment_d {
    typedef typename TypeForDimension<Dim>::Segment Type;
};
///
/// Another way of looking at TypeForDimension<Dim>::Surface
template <int Dim>
struct Surface_d {
    typedef typename TypeForDimension<Dim>::Surface Type;
};
///
/// Another way of looking at TypeForDimension<Dim>::Volume
template <int Dim>
struct Volume_d {
    typedef typename TypeForDimension<Dim>::Volume Type;
};

///
/// Create a distinct type for each dimension
template <int N>
struct dim_t {
    enum { v = N };
};

///
/// Get a primitive dimension (0: point, 1: line, 2: surface, 3: volume) from a type
template <class T>
struct PrimitiveDimension {
    static const int value = 0;
};

template <>
struct PrimitiveDimension<TypeForDimension<2>::Segment> {
    static const int value = 1;
};
template <>
struct PrimitiveDimension<TypeForDimension<3>::Segment> {
    static const int value = 1;
};
template <>
struct PrimitiveDimension<TypeForDimension<2>::Surface> {
    static const int value = 2;
};
template <>
struct PrimitiveDimension<TypeForDimension<3>::Surface> {
    static const int value = 2;
};
template <>
struct PrimitiveDimension<TypeForDimension<2>::Volume> {
    static const int value = 3;
};
template <>
struct PrimitiveDimension<TypeForDimension<3>::Volume> {
    static const int value = 3;
};

///
/// Tests if a primitive type has a larger dimension than another one
template <class X, class Y>
struct IsPrimitiveLarger {
    static const bool value = PrimitiveDimension<X>::value > PrimitiveDimension<Y>::value;
};

} // namespace detail
} // namespace SFCGAL

#endif
