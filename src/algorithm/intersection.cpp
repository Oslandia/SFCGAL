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

#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/Exception.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/collect.h>
#include <SFCGAL/algorithm/collectionHomogenize.h>
#include <SFCGAL/detail/tools/Registry.h>
#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/algorithm/isValid.h>

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/box_intersection_d.h>

//
// Intersection kernel

using namespace SFCGAL::detail;

namespace SFCGAL {

typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Triangle_2<Kernel> Triangle_2;

namespace algorithm {
// see Intersection3D.cpp
void intersection( const PrimitiveHandle<3>& pa, const PrimitiveHandle<3>& pb,
                   GeometrySet<3>& output, dim_t<3> );
// see Intersection2D.cpp
void intersection( const PrimitiveHandle<2>& pa, const PrimitiveHandle<2>& pb,
                   GeometrySet<2>& output, dim_t<2> );
//
// We deal here with symmetric call
template <int Dim>
void dispatch_intersection_sym( const PrimitiveHandle<Dim>& pa, const PrimitiveHandle<Dim>& pb,
                                GeometrySet<Dim>& output )
{
    // assume types are ordered by dimension within the boost::variant
    if ( pa.handle.which() >= pb.handle.which() ) {
        intersection( pa, pb, output, dim_t<Dim>() );
    }
    else {
        intersection( pb, pa, output, dim_t<Dim>() );
    }
}

template <int Dim>
void intersection( const PrimitiveHandle<Dim>& pa, const PrimitiveHandle<Dim>& pb, GeometrySet<Dim>& output )
{
    dispatch_intersection_sym( pa, pb, output );
}

template void intersection<2>( const PrimitiveHandle<2>& a, const PrimitiveHandle<2>& b, GeometrySet<2>& );
template void intersection<3>( const PrimitiveHandle<3>& a, const PrimitiveHandle<3>& b, GeometrySet<3>& );

template <int Dim>
struct intersection_cb {
    intersection_cb( GeometrySet<Dim>& out ) : output( out ) {}

    void operator()( const typename PrimitiveBox<Dim>::Type& a,
                     const typename PrimitiveBox<Dim>::Type& b ) {
        dispatch_intersection_sym<Dim>( *a.handle(), *b.handle(), output );
    }

    GeometrySet<Dim>& output;
};

/**
 * intersection post processing
 */
void post_intersection( const GeometrySet<2>& input, GeometrySet<2>& output )
{
    //
    // reverse orientation of polygons if needed
    for ( GeometrySet<2>::SurfaceCollection::const_iterator it = input.surfaces().begin();
            it != input.surfaces().end();
            ++it ) {
        const CGAL::Polygon_with_holes_2<Kernel>& p = it->primitive();
        CGAL::Polygon_2<Kernel> outer = p.outer_boundary();

        if ( outer.orientation() == CGAL::CLOCKWISE ) {
            outer.reverse_orientation();
        }

        std::list<CGAL::Polygon_2<Kernel> > rings;

        for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator hit = p.holes_begin();
                hit != p.holes_end();
                ++hit ) {
            rings.push_back( *hit );

            if ( hit->orientation() == CGAL::COUNTERCLOCKWISE ) {
                rings.back().reverse_orientation();
            }
        }

        output.surfaces().push_back( CGAL::Polygon_with_holes_2<Kernel>( outer, rings.begin(), rings.end() ) );
    }

    output.points() = input.points();
    output.segments() = input.segments();
    output.volumes() = input.volumes();
}

void post_intersection( const GeometrySet<3>& input, GeometrySet<3>& output )
{
    // nothing special to do
    output = input;
}

template <int Dim>
void intersection( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b, GeometrySet<Dim>& output )
{
    typename SFCGAL::detail::HandleCollection<Dim>::Type ahandles, bhandles;
    typename SFCGAL::detail::BoxCollection<Dim>::Type aboxes, bboxes;
    a.computeBoundingBoxes( ahandles, aboxes );
    b.computeBoundingBoxes( bhandles, bboxes );

    GeometrySet<Dim> temp, temp2;
    intersection_cb<Dim> cb( temp );
    CGAL::box_intersection_d( aboxes.begin(), aboxes.end(),
                              bboxes.begin(), bboxes.end(),
                              cb );

    post_intersection( temp, temp2 );
    output.merge( temp2 );
}

template void intersection<2>( const GeometrySet<2>& a, const GeometrySet<2>& b, GeometrySet<2>& );
template void intersection<3>( const GeometrySet<3>& a, const GeometrySet<3>& b, GeometrySet<3>& );

std::auto_ptr<Geometry> intersection( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    GeometrySet<2> gsa( ga ), gsb( gb ), output;
    algorithm::intersection( gsa, gsb, output );

    GeometrySet<2> filtered;
    output.filterCovered( filtered );
    return filtered.recompose();
}

std::auto_ptr<Geometry> intersection( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( gb );

    return intersection( ga, gb, NoValidityCheck() );
}

std::auto_ptr<Geometry> intersection3D( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    GeometrySet<3> gsa( ga ), gsb( gb ), output;
    algorithm::intersection( gsa, gsb, output );

    GeometrySet<3> filtered;
    output.filterCovered( filtered );

    return filtered.recompose();
}

std::auto_ptr<Geometry> intersection3D( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( gb );

    return intersection3D( ga, gb, NoValidityCheck() );
}
}
}
