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
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel> PolygonWH_2;

typedef CGAL::Point_3<Kernel> Point_3;
typedef CGAL::Segment_3<Kernel> Segment_3;
typedef CGAL::Triangle_3<Kernel> Triangle_3;

namespace algorithm {

bool do_intersect( const Point_2& point, const PolygonWH_2 & polygon )
{
    // point intersects if it's inside the ext ring and outside all holes

    if ( CGAL::bounded_side_2(polygon.outer_boundary().vertices_begin(), 
                polygon.outer_boundary().vertices_end(), point, Kernel() ) 
            == CGAL::ON_UNBOUNDED_SIDE) return false;

    for ( PolygonWH_2::Hole_const_iterator hit = polygon.holes_begin();
            hit != polygon.holes_end();
            ++hit ) {
        if ( CGAL::bounded_side_2(hit->vertices_begin(), 
                    hit->vertices_end(), point, Kernel() ) 
                !=  CGAL::ON_UNBOUNDED_SIDE) return false;
    }

    return true;
}

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

struct Nearer
{
    Nearer( const Point_2 & reference ) :_ref( reference ) {}
    bool operator()( const Point_2 & lhs, const Point_2 & rhs ) const
    {
        return CGAL::squared_distance(_ref, lhs) < CGAL::squared_distance(_ref, rhs);
    }
private:
    const Point_2 _ref;
};

template < typename SegmentOutputIteratorType>
SegmentOutputIteratorType difference( const Segment_2 & segment, const PolygonWH_2 & polygon, SegmentOutputIteratorType out)
{
    // we could triangulate the polygon and substract each triangle
    //
    // we could also cut the line by polygon contours and test if the middle of the segment is inside
    // but if the segment lies on the contour it's a special case
    // we first substract the contours to take care of this
    // special case, we obtain a vector of segments,
    // for each segment of this vector, we subdivide it with the intersection
    // points with the rings
    // once done, we check, for each subdivision that has distinct end-points
    // if the middle is in or out.

    std::vector< Segment_2 > result(1, segment);
    std::vector< Polygon_2 > rings(1, polygon.outer_boundary());
    rings.insert(rings.end(), polygon.holes_begin(), polygon.holes_end());

    for ( std::vector< Polygon_2 >::iterator ring = rings.begin(); ring != rings.end(); ++ring ){
        for ( Polygon_2::Vertex_const_iterator target = ring->vertices_begin(); 
                target != ring->vertices_end(); ++target ){
            const Segment_2 sc( target == ring->vertices_begin() 
                    ? *(ring->vertices_end() - 1) 
                    : *(target - 1)
                    , 
                    *target );
            std::vector< Segment_2 > tmp;
            for ( std::vector< Segment_2 >::const_iterator s = result.begin(); s != result.end(); ++s ){
                difference( *s, sc, std::back_inserter(tmp) );
            }
            tmp.swap( result );
        }
    }

    for ( std::vector< Segment_2 >::const_iterator s = result.begin(); s != result.end(); ++s ){
        std::vector< Point_2 > points;
        points.push_back( s->source() );
        for ( std::vector< Polygon_2 >::iterator ring = rings.begin(); ring != rings.end(); ++ring ){
            for ( Polygon_2::Vertex_const_iterator target = ring->vertices_begin(); 
                    target != ring->vertices_end(); ++target ){
                Segment_2 sc( target == ring->vertices_begin() 
                        ? *(ring->vertices_end() - 1) 
                        : *(target - 1)
                        , 
                        *target );
                CGAL::Object inter = CGAL::intersection( *s, sc );
                const Point_2 * p = CGAL::object_cast< Point_2 >(&inter);
                if (p) points.push_back( *p );
            }
        }
        points.push_back( s->target() );
        // order point according to the distance from source
        const Nearer nearer( s->source() );
        std::sort( points.begin()+1, points.end()-1, nearer );

        // append segments that has length and wich midpoint is outside polygon to result
        for ( std::vector< Point_2 >::const_iterator p = points.begin(); p != points.end()-1; ++p ){
            std::vector< Point_2 >::const_iterator q = p+1;
            if ( *p != *q && !do_intersect( CGAL::midpoint(*p,*q), polygon ) ){
                *out++ = Segment_2( *p, *q );
            }
        }
    }

    return out;
}

PolygonWH_2
fix_sfs_valid_polygon(const PolygonWH_2 & p )
{
    // a polygon is valid for sfs and invalid for CGAL when two rings intersect
    // on a point that is not a ring vertex
    // we add this vertex to fix the polygon
    // for each ring segment
    //    for every other ring point
    //        add point to segment

    // put all rings in a vector to avoid distinction between outer and holes
    std::vector< Polygon_2 > rings(1, p.outer_boundary());
    rings.insert(rings.end(), p.holes_begin(), p.holes_end());

    std::vector< Polygon_2 > out;
    for ( std::vector< Polygon_2 >::iterator ring = rings.begin(); ring != rings.end(); ++ring ){
        out.push_back( Polygon_2() );
        for ( Polygon_2::Vertex_const_iterator target = ring->vertices_begin(); target != ring->vertices_end(); ++target ){
            Segment_2 segment( 
                    target == ring->vertices_begin() 
                    ? *(ring->vertices_end() - 1) 
                    : *(target - 1)
                    , 
                    *target );
            // for every other ring
            for ( std::vector< Polygon_2 >::const_iterator other = rings.begin(); other != rings.end(); ++other ){
                if ( ring == other ) continue;
                for ( Polygon_2::Vertex_const_iterator vertex = other->vertices_begin(); 
                        vertex != other->vertices_end(); ++vertex ){
                    if ( CGAL::do_intersect( *vertex, segment ) ){
                        out.back().push_back( *vertex );
                    }
                }
            }
            out.back().push_back( *target );
        }
    }

    return PolygonWH_2(out[0], out.begin()+1, out.end());
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

template < typename PolygonOutputIteratorType>
PolygonOutputIteratorType difference( const PolygonWH_2 & a, const PolygonWH_2 & b, PolygonOutputIteratorType out)
{
    CGAL::Gps_segment_traits_2<Kernel> traits;
    
    std::vector< PolygonWH_2 > temp;
    CGAL::difference( 
            are_holes_and_boundary_pairwise_disjoint( a, traits ) ? a : fix_sfs_valid_polygon( a ), 
            are_holes_and_boundary_pairwise_disjoint( b, traits ) ? b : fix_sfs_valid_polygon( b ),
            std::back_inserter( temp ) );

    // polygon outer rings from difference can self intersect at points
    // therefore we need to split the generated polygons so that they are valid for SFS
    for ( std::vector< PolygonWH_2 >::const_iterator poly=temp.begin(); poly!=temp.end(); ++poly  ){
        const Polygon_2 & outer = poly->outer_boundary();
        for ( Polygon_2::Vertex_const_iterator v = outer.vertices_begin(); 
                v != outer.vertices_end(); ++v ){
            for ( Polygon_2::Vertex_const_iterator o = v+1; o != outer.vertices_end(); ++o ){
                if ( *o == *v ){
                    BOOST_THROW_EXCEPTION(NotImplementedException("Difference yelding a polygon which exterior ring self intersect is not implemented") );
                }
            }
        }
        *out++ = *poly;
    }
    
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const Point_2 & primitive, const PrimitiveHandle<2>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        if ( primitive != *pb.as< Point_2 >() ) *out++ = primitive;
        break;
    case PrimitiveSegment:
        if ( ! CGAL::do_intersect( primitive, *pb.as< Segment_2 >() ) ) *out++ = primitive;
        break;
    case PrimitiveSurface:
        if ( ! do_intersect( primitive, *pb.as< PolygonWH_2 >()  ) ) *out++=primitive;
        break;
    }
    return out;
}



template < typename OutputIteratorType >
OutputIteratorType difference( const Segment_2 & primitive, const PrimitiveHandle<2>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        *out++ = primitive;
        break;
    case PrimitiveSegment:
        difference( primitive, *pb.as< Segment_2 >(), out);
        break;
    case PrimitiveSurface:
        difference( primitive, *pb.as< PolygonWH_2 >(), out);
        break;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const PolygonWH_2 & primitive, const PrimitiveHandle<2>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
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
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        if ( primitive != *pb.as< Point_3 >() ) *out++ = primitive; 
        break;
    case PrimitiveSegment:
        BOOST_THROW_EXCEPTION( NotImplementedException("Point_3 - Segment_3 is not implemented") );
        break;
    case PrimitiveSurface:
        BOOST_THROW_EXCEPTION( NotImplementedException("Point_3 - Triangle_3 is not implemented") );
        break;
    case PrimitiveVolume:
        BOOST_THROW_EXCEPTION( NotImplementedException("Point_3 - MarkedPolyhedron is not implemented") );
        break;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const Segment_3 & primitive, const PrimitiveHandle<3>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        *out++ = primitive;
        break;
    case PrimitiveSegment:
        difference( primitive, *pb.as< Segment_3 >(), out);
        break;
    case PrimitiveSurface:
        difference( primitive, *pb.as< Triangle_3 >(), out);
        break;
    case PrimitiveVolume:
        BOOST_THROW_EXCEPTION( NotImplementedException("Segment_3 - MarkedPolyhedron is not implemented") );
        break;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const Triangle_3 & primitive, const PrimitiveHandle<3>& pb, OutputIteratorType out )
{
    switch ( pb.handle.which() ){
    case PrimitivePoint:
        *out++ = primitive;
        break;
    case PrimitiveSegment:
        *out++ = primitive;
        break;
    case PrimitiveSurface:
        BOOST_THROW_EXCEPTION( NotImplementedException("Triangle_3 - Triangle_3 is not implemented") );
        break;
    case PrimitiveVolume:
        BOOST_THROW_EXCEPTION( NotImplementedException("Triangle_3 - MarkedPolyhedron is not implemented") );
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
        std::vector< Point_2 > res = difference( 
                *pa.as< Point_2 >(), begin, end );
        output.addPoints( res.begin(), res.end() );
        return;
        }
    case PrimitiveSegment:
        {
        std::vector< Segment_2 > res = difference( 
                *pa.as< Segment_2 >(), begin, end );
        output.addSegments( res.begin(), res.end() );
        return;
        }
    case PrimitiveSurface:
        {
        std::vector< PolygonWH_2 > res = difference( *pa.as< PolygonWH_2 >(), begin, end );
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
        std::vector< Point_3 > res = difference( *pa.as< Point_3 >(), begin, end );
        output.addPoints( res.begin(), res.end() );
        return;
        }
    case PrimitiveSegment:
        {
        std::vector< Segment_3 > res = difference( *pa.as< Segment_3 >(), begin, end );
        output.addSegments( res.begin(), res.end() );
        break;
        }
    case PrimitiveSurface:
        {
        std::vector< Triangle_3 > res = difference( *pa.as< Triangle_3 >(), begin, end );
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
