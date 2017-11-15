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

#include <SFCGAL/algorithm/differencePrimitives.h>
#include <SFCGAL/algorithm/difference.h>
#include <SFCGAL/Exception.h>
#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/triangulate/triangulatePolygon.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/TriangulatedSurface.h>

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// #include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/box_intersection_d.h>
#include <CGAL/corefinement_operations.h>

//
// Intersection kernel

using namespace SFCGAL::detail;

namespace SFCGAL {

namespace algorithm {

template <int Dim>
struct CollisionMapper {
    typedef std::vector< PrimitiveHandle<Dim>* > PrimitiveHandleSet;
    typedef std::map< PrimitiveHandle<Dim>*, PrimitiveHandleSet > Map;
    CollisionMapper( Map& map ) : _map( map ) {};
    void operator()( const typename PrimitiveBox<Dim>::Type& a,
                     const typename PrimitiveBox<Dim>::Type& b ) {
        _map[a.handle()].push_back( b.handle() );
    }

private:
    Map& _map;
};

template < typename OutputIteratorType >
OutputIteratorType difference( const Point_2& primitive, const PrimitiveHandle<2>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ) {
    case PrimitivePoint:
        difference( primitive, *pb.as< Point_2 >(), out );
        break;

    case PrimitiveSegment:
        difference( primitive, *pb.as< Segment_2 >(), out );
        break;

    case PrimitiveSurface:
        difference( primitive, *pb.as< PolygonWH_2 >(), out );
        break;
    }

    return out;
}


template < typename OutputIteratorType >
OutputIteratorType difference( const Segment_2& primitive, const PrimitiveHandle<2>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ) {
    case PrimitivePoint:
        *out++ = primitive;
        break;

    case PrimitiveSegment:
        difference( primitive, *pb.as< Segment_2 >(), out );
        break;

    case PrimitiveSurface:
        difference( primitive, *pb.as< PolygonWH_2 >(), out );
        break;
    }

    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const PolygonWH_2& primitive, const PrimitiveHandle<2>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ) {
    case PrimitivePoint:
        *out++ = primitive;
        break;

    case PrimitiveSegment:
        *out++ = primitive;
        break;

    case PrimitiveSurface:
        difference( primitive, *pb.as< PolygonWH_2 >(), out );
        break;
    }

    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const Point_3& primitive, const PrimitiveHandle<3>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ) {
    case PrimitivePoint:
        difference( primitive, *pb.as< Point_3 >(), out );
        break;

    case PrimitiveSegment:
        difference( primitive, *pb.as< Segment_3 >(), out );
        break;

    case PrimitiveSurface:
        difference( primitive, *pb.as< Triangle_3 >(), out );
        break;

    case PrimitiveVolume:
        difference( primitive, *pb.as< MarkedPolyhedron >(), out );
        break;
    }

    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const Segment_3& primitive, const PrimitiveHandle<3>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ) {
    case PrimitivePoint:
        *out++ = primitive;
        break;

    case PrimitiveSegment:
        difference( primitive, *pb.as< Segment_3 >(), out );
        break;

    case PrimitiveSurface:
        difference( primitive, *pb.as< Triangle_3 >(), out );
        break;

    case PrimitiveVolume:
        difference( primitive, *pb.as< MarkedPolyhedron >(), out );
        break;
    }

    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const Triangle_3& primitive, const PrimitiveHandle<3>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ) {
    case PrimitivePoint:
        *out++ = primitive;
        break;

    case PrimitiveSegment:
        *out++ = primitive;
        break;

    case PrimitiveSurface:
        difference( primitive, *pb.as< Triangle_3 >(), out );
        break;

    case PrimitiveVolume:
        difference( primitive, *pb.as< MarkedPolyhedron >(), out );
        break;
    }

    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const MarkedPolyhedron& primitive, const PrimitiveHandle<3>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ) {
    case PrimitivePoint:
        *out++ = primitive;
        break;

    case PrimitiveSegment:
        *out++ = primitive;
        break;

    case PrimitiveSurface:
        *out++ = primitive;
        break;

    case PrimitiveVolume:
        difference( primitive, *pb.as< MarkedPolyhedron >(), out );
        break;
    }

    return out;
}


template <typename Primitive, typename PrimitiveHandleConstIterator>
std::vector< Primitive >
difference( const Primitive& primitive, PrimitiveHandleConstIterator begin, PrimitiveHandleConstIterator end )
{
    std::vector< Primitive > primitives;
    primitives.push_back( primitive );

    for ( PrimitiveHandleConstIterator b = begin; b != end; ++b ) {
        std::vector< Primitive > new_primitives;

        for ( typename std::vector< Primitive >::const_iterator a = primitives.begin();
                a != primitives.end(); ++a ) {
            difference( *a, *( *b ), std::back_inserter( new_primitives ) );
        }

        primitives.swap( new_primitives );
    }

    return primitives;
}


// just performs the type switch for the primitive to substract from
//
void appendDifference( const PrimitiveHandle<2>& pa,
                       CollisionMapper<2>::PrimitiveHandleSet::const_iterator begin,
                       CollisionMapper<2>::PrimitiveHandleSet::const_iterator end,
                       GeometrySet<2>& output )
{
    switch ( pa.handle.which() ) {
    case PrimitivePoint: {
        std::vector< Point_2 > res = difference(
                                         *pa.as< Point_2 >(), begin, end );
        output.addPoints( res.begin(), res.end() );
        return;
    }

    case PrimitiveSegment: {
        std::vector< Segment_2 > res = difference(
                                           *pa.as< Segment_2 >(), begin, end );
        output.addSegments( res.begin(), res.end() );
        return;
    }

    case PrimitiveSurface: {
        std::vector< PolygonWH_2 > res = difference( *pa.as< PolygonWH_2 >(), begin, end );
        output.addSurfaces( res.begin(), res.end() );
        return;
    }
    }
}

void appendDifference( const PrimitiveHandle<3>& pa,
                       CollisionMapper<3>::PrimitiveHandleSet::const_iterator begin,
                       CollisionMapper<3>::PrimitiveHandleSet::const_iterator end,
                       GeometrySet<3>& output )
{
    switch ( pa.handle.which() ) {
    case PrimitivePoint: {
        std::vector< Point_3 > res = difference( *pa.as< Point_3 >(), begin, end );
        output.addPoints( res.begin(), res.end() );
        return;
    }

    case PrimitiveSegment: {
        std::vector< Segment_3 > res = difference( *pa.as< Segment_3 >(), begin, end );
        output.addSegments( res.begin(), res.end() );
        break;
    }

    case PrimitiveSurface: {
        std::vector< Triangle_3 > res = difference( *pa.as< Triangle_3 >(), begin, end );
        output.addSurfaces( res.begin(), res.end() );
        break;
    }

    case PrimitiveVolume: {
        std::vector< MarkedPolyhedron > res = difference( *pa.as< MarkedPolyhedron >(), begin, end );
        output.addVolumes( res.begin(), res.end() );
        break;
    }
    }
}

/**
 * difference post processing
 */
void post_difference( const GeometrySet<2>& input, GeometrySet<2>& output )
{
    //
    // reverse orientation of polygons if needed
    for ( GeometrySet<2>::SurfaceCollection::const_iterator it = input.surfaces().begin();
            it != input.surfaces().end();
            ++it ) {
        const PolygonWH_2& p = it->primitive();
        Polygon_2 outer = p.outer_boundary();

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

        output.surfaces().push_back( PolygonWH_2( outer, rings.begin(), rings.end() ) );
    }

    output.points() = input.points();
    output.segments() = input.segments();
    output.volumes() = input.volumes();
}

void post_difference( const GeometrySet<3>& input, GeometrySet<3>& output )
{
    // nothing special to do
    output = input;
}

template <int Dim>
void difference( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b, GeometrySet<Dim>& output )
{
    typedef typename SFCGAL::detail::BoxCollection<Dim>::Type BoxCollection;
    typename SFCGAL::detail::HandleCollection<Dim>::Type ahandles, bhandles;
    BoxCollection aboxes, bboxes;
    a.computeBoundingBoxes( ahandles, aboxes );
    b.computeBoundingBoxes( bhandles, bboxes );

    // here we use box_intersection_d to build the list of operations
    // that actually need to be performed
    GeometrySet<Dim> temp, temp2;
    typename CollisionMapper<Dim>::Map map;
    CollisionMapper<Dim> cb( map );
    CGAL::box_intersection_d( aboxes.begin(), aboxes.end(),
                              bboxes.begin(), bboxes.end(),
                              cb );

    // now we have in cb a map of operations to perform
    // we can put in the result right away all the primitives
    // that are not keys in this map
    {
        typename BoxCollection::const_iterator ait = aboxes.begin();
        const typename BoxCollection::const_iterator end = aboxes.end();

        for ( ; ait != end; ++ait ) {
            if ( map.find( ait->handle() ) == map.end() ) {
                temp.addPrimitive( *ait->handle() );
            }
        }
    }

    // then we delegate the operations according to type
    {
        typename CollisionMapper<Dim>::Map::const_iterator cbit = map.begin();
        const typename CollisionMapper<Dim>::Map::const_iterator end = map.end();

        for ( ; cbit != end; ++cbit ) {
            appendDifference( *cbit->first, cbit->second.begin(), cbit->second.end(), temp );
        }
    }

    post_difference( temp, temp2 );
    output.merge( temp2 );
}

template void difference<2>( const GeometrySet<2>& a, const GeometrySet<2>& b, GeometrySet<2>& );
template void difference<3>( const GeometrySet<3>& a, const GeometrySet<3>& b, GeometrySet<3>& );

std::auto_ptr<Geometry> difference( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    GeometrySet<2> gsa( ga ), gsb( gb ), output;
    algorithm::difference( gsa, gsb, output );

    GeometrySet<2> filtered;
    output.filterCovered( filtered );
    return filtered.recompose();
}

std::auto_ptr<Geometry> difference( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( gb );

    return difference( ga, gb, NoValidityCheck() );
}

std::auto_ptr<Geometry> difference3D( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    GeometrySet<3> gsa( ga ), gsb( gb ), output;
    algorithm::difference( gsa, gsb, output );

    GeometrySet<3> filtered;
    output.filterCovered( filtered );

    return filtered.recompose();
}

std::auto_ptr<Geometry> difference3D( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( gb );

    return difference3D( ga, gb, NoValidityCheck() );
}
}
}
