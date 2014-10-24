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

#include <SFCGAL/algorithm/difference.h>
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
#include <CGAL/corefinement_operations.h>

//
// Intersection kernel

using namespace SFCGAL::detail;

namespace SFCGAL {

typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Triangle_2<Kernel> Triangle_2;

namespace algorithm {

template < typename SegmentType , typename SegmentOrSurfaceType, typename SegmentOutputIteratorType>
SegmentOutputIteratorType difference( const SegmentType & a, const SegmentOrSurfaceType & b, SegmentOutputIteratorType out)
{
    CGAL::Object inter = CGAL::intersection( a, b );
    const SegmentType* s = CGAL::object_cast< SegmentType >(&inter);
    if ( s ){ // there maybe zero, one or two segments as a result
        if (CGAL::squared_distance( a.source(), s->source() ) < CGAL::squared_distance( a.source(), s->target() )){
            if ( a.source() != s->source() ) *out++ = SegmentType( a.source(), s->source() );
            if ( s->target() != a.target() ) *out++ = SegmentType( s->target(), a.target() );
        }
        else{
            if ( a.source() != s->target() ) *out++ = SegmentType( a.source(), s->target() );
            if ( s->source() != a.target() ) *out++ = SegmentType( s->source(), a.target() );
        }
    }
    else { // intersection is a point or nothing, leave a unchanged
        *out++ = a;
    }
    return out;
}
template < typename SegmentOutputIteratorType>
SegmentOutputIteratorType difference( const CGAL::Segment_2<Kernel> & , const CGAL::Polygon_with_holes_2<Kernel> & , SegmentOutputIteratorType out)
{
    // we triangulate the polygon and substract each triangle

    BOOST_ASSERT(false);

    return out;
}

template < typename VolumeOutputIteratorType>
VolumeOutputIteratorType difference( const MarkedPolyhedron & a, const MarkedPolyhedron & b, VolumeOutputIteratorType out)
{
    MarkedPolyhedron& p = const_cast<MarkedPolyhedron&>( a );
    MarkedPolyhedron& q = const_cast<MarkedPolyhedron&>( b );
    typedef CGAL::Polyhedron_corefinement<MarkedPolyhedron> Corefinement;
    Corefinement coref;
    CGAL::Emptyset_iterator no_polylines;
    typedef std::vector<std::pair<MarkedPolyhedron*, int> >  ResultType;
    ResultType result;
    coref( p, q, no_polylines, std::back_inserter( result ), Corefinement::P_minus_Q_tag );

    for ( ResultType::iterator it = result.begin(); it != result.end(); it++){
        *out++ = *it->first;
        delete it->first;
    }
    return out;
}
    
bool do_intersect( const CGAL::Point_2<Kernel>& point, const CGAL::Polygon_with_holes_2<Kernel> & polygon )
{
    // point intersects if it's inside the ext ring and outside all holes

    if ( CGAL::bounded_side_2(polygon.outer_boundary().vertices_begin(), 
                polygon.outer_boundary().vertices_end(), point, Kernel() ) 
            ==  CGAL::ON_UNBOUNDED_SIDE) return false;

    for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator hit = polygon.holes_begin();
            hit != polygon.holes_end();
            ++hit ) {
        if ( CGAL::bounded_side_2(hit->vertices_begin(), 
                    hit->vertices_end(), point, Kernel() ) 
                !=  CGAL::ON_UNBOUNDED_SIDE) return false;
    }

    return true;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const CGAL::Point_2<Kernel> & primitive, const PrimitiveHandle<2>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        if ( primitive != *pb.as< CGAL::Point_2<Kernel> >() ) *out++ = primitive;
        break;
    case PrimitiveSegment:
        if ( ! CGAL::do_intersect( primitive, *pb.as< CGAL::Segment_2<Kernel> >() ) ) *out++ = primitive;
        break;
    case PrimitiveSurface:
        if ( ! do_intersect( primitive, *pb.as< CGAL::Polygon_with_holes_2<Kernel> >()  ) ) *out++=primitive;
        break;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const CGAL::Segment_2<Kernel> & primitive, const PrimitiveHandle<2>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        *out++ = primitive;
        break;
    case PrimitiveSegment:
        difference( primitive, *pb.as< CGAL::Segment_2<Kernel> >(), out);
        break;
    case PrimitiveSurface:
        difference( primitive, *pb.as< CGAL::Polygon_with_holes_2<Kernel> >(), out);
        break;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const CGAL::Polygon_with_holes_2<Kernel> & primitive, const PrimitiveHandle<2>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        *out++ = primitive;
        break;
    case PrimitiveSegment:
        *out++ = primitive;
        break;
    case PrimitiveSurface:
        CGAL::difference( primitive, *pb.as< CGAL::Polygon_with_holes_2<Kernel> >(), out);
        break;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const CGAL::Point_3<Kernel> & primitive, const PrimitiveHandle<3>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        if ( primitive != *pb.as< CGAL::Point_3<Kernel> >() ) *out++ = primitive; 
        break;
    case PrimitiveSegment:
        // CGAL::do_intersect( CGAL::Point_3<Kernel>, CGAL::Segment_3<Kernel> ) does not exist 
        BOOST_ASSERT(false);
        break;
    case PrimitiveSurface:
        BOOST_ASSERT(false);
        break;
    case PrimitiveVolume:
        BOOST_ASSERT(false);
        break;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const CGAL::Segment_3<Kernel> & primitive, const PrimitiveHandle<3>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        *out++ = primitive;
        break;
    case PrimitiveSegment:
        difference( primitive, *pb.as< CGAL::Segment_3<Kernel> >(), out);
        break;
    case PrimitiveSurface:
        difference( primitive, *pb.as< CGAL::Triangle_3<Kernel> >(), out);
        break;
    case PrimitiveVolume:
        BOOST_ASSERT(false);
        break;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const CGAL::Triangle_3<Kernel> & primitive, const PrimitiveHandle<3>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        *out++ = primitive;
        break;
    case PrimitiveSegment:
        *out++ = primitive;
        break;
    case PrimitiveSurface:
        BOOST_ASSERT(false);
        break;
    case PrimitiveVolume:
        BOOST_ASSERT(false);
        break;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const MarkedPolyhedron & primitive, const PrimitiveHandle<3>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
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
        difference( primitive, *pb.as< MarkedPolyhedron >(), out);
        break;
    }
    return out;
}


template <typename Primitive, typename PrimitiveHandleConstIterator>
std::vector< Primitive >
difference( const Primitive & primitive, PrimitiveHandleConstIterator begin, PrimitiveHandleConstIterator end )
{
    std::vector< Primitive > primitives; 
    primitives.push_back( primitive );
    for (PrimitiveHandleConstIterator b = begin; b != end; ++b){
        std::vector< Primitive > new_primitives; 
        for ( typename std::vector< Primitive >::const_iterator a = primitives.begin(); 
           a != primitives.end(); ++a ){
            difference( *a, *(*b), std::back_inserter( new_primitives ) );
        }
        primitives.swap(new_primitives);
    }
    return primitives;
}

template <int Dim>
struct difference_cb {
    typedef std::vector< PrimitiveHandle<Dim>* > PrimitiveHandleSet;
    typedef std::map< PrimitiveHandle<Dim>*, PrimitiveHandleSet > Map;
    difference_cb( Map & map ) : _map(map) {};
    void operator()( const typename PrimitiveBox<Dim>::Type& a,
                     const typename PrimitiveBox<Dim>::Type& b ) {
        _map[a.handle()].push_back( b.handle() );
    }

private:
    Map & _map;
};


// just performs the type switch for the primitive to substract from
//
void appendDifference( const PrimitiveHandle<2>& pa, 
        difference_cb<2>::PrimitiveHandleSet::const_iterator begin, 
        difference_cb<2>::PrimitiveHandleSet::const_iterator end, 
        GeometrySet<2>& output)
{
    switch ( pa.handle.which() ){
    case PrimitivePoint:
        {
        std::vector< CGAL::Point_2<Kernel> > res = difference( 
                *pa.as< CGAL::Point_2<Kernel> >(), begin, end );
        output.addPoints( res.begin(), res.end() );
        return;
        }
    case PrimitiveSegment:
        {
        std::vector< CGAL::Segment_2<Kernel> > res = difference( 
                *pa.as< CGAL::Segment_2<Kernel> >(), begin, end );
        output.addSegments( res.begin(), res.end() );
        return;
        }
    case PrimitiveSurface:
        {
        std::vector< CGAL::Polygon_with_holes_2<Kernel> > res = difference( 
                *pa.as< CGAL::Polygon_with_holes_2<Kernel> >(), begin, end );
        output.addSurfaces( res.begin(), res.end() );
        return;
        }
    }
}

void appendDifference( const PrimitiveHandle<3>& pa, 
        difference_cb<3>::PrimitiveHandleSet::const_iterator begin, 
        difference_cb<3>::PrimitiveHandleSet::const_iterator end, 
        GeometrySet<3>& output)
{
    switch ( pa.handle.which() ){
    case PrimitivePoint:
        {
        std::vector< CGAL::Point_3<Kernel> > res = difference( 
                *pa.as< CGAL::Point_3<Kernel> >(), begin, end );
        output.addPoints( res.begin(), res.end() );
        return;
        }
    case PrimitiveSegment:
        {
        std::vector< CGAL::Segment_3<Kernel> > res = difference( 
                *pa.as< CGAL::Segment_3<Kernel> >(), begin, end );
        output.addSegments( res.begin(), res.end() );
        break;
        }
    case PrimitiveSurface:
        {
        std::vector< CGAL::Triangle_3<Kernel> > res = difference( 
                *pa.as< CGAL::Triangle_3<Kernel> >(), begin, end );
        output.addSurfaces( res.begin(), res.end() );
        break;
        }
    case PrimitiveVolume:
        {
        std::vector< MarkedPolyhedron > res = difference( 
                *pa.as< MarkedPolyhedron >(), begin, end );
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
    typename difference_cb<Dim>::Map map;
    difference_cb<Dim> cb(map);
    CGAL::box_intersection_d( aboxes.begin(), aboxes.end(),
                              bboxes.begin(), bboxes.end(),
                              cb );

    // now we have in cb a map of operations to perform
    // we can put in the result right away all the primitives
    // that are not keys in this map
    {
        typename BoxCollection::const_iterator ait = aboxes.begin();
        const typename BoxCollection::const_iterator end = aboxes.end();
        for ( ; ait != end; ++ait ){
            if ( map.find( ait->handle() ) == map.end() ) {
                temp.addPrimitive( *ait->handle() );
            }
        }
    }

    // then we delegate the operations according to type
    {
        typename difference_cb<Dim>::Map::const_iterator cbit = map.begin();
        const typename difference_cb<Dim>::Map::const_iterator end = map.end();
        for ( ; cbit != end; ++cbit ){
            appendDifference( *cbit->first, cbit->second.begin(), cbit->second.end(), temp);
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
