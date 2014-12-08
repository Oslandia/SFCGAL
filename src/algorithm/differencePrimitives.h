#include <SFCGAL/Exception.h>
#include <SFCGAL/triangulate/triangulatePolygon.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/detail/GeometrySet.h>

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/box_intersection_d.h>
#include <CGAL/corefinement_operations.h>
#include <CGAL/Point_inside_polyhedron_3.h>

using namespace SFCGAL::detail;

namespace SFCGAL {

namespace algorithm {

typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Triangle_2<Kernel> Triangle_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel> PolygonWH_2;

typedef CGAL::Vector_3<Kernel> Vector_3;
typedef CGAL::Point_3<Kernel> Point_3;
typedef CGAL::Segment_3<Kernel> Segment_3;
typedef CGAL::Triangle_3<Kernel> Triangle_3;
typedef CGAL::Plane_3<Kernel> Plane_3;

inline
bool do_intersect( const Point_2& point, const PolygonWH_2& polygon )
{
    // point intersects if it's inside the ext ring and outside all holes

    if ( CGAL::bounded_side_2( polygon.outer_boundary().vertices_begin(),
                               polygon.outer_boundary().vertices_end(), point, Kernel() )
            == CGAL::ON_UNBOUNDED_SIDE ) {
        return false;
    }

    for ( PolygonWH_2::Hole_const_iterator hit = polygon.holes_begin();
            hit != polygon.holes_end();
            ++hit ) {
        if ( CGAL::bounded_side_2( hit->vertices_begin(),
                                   hit->vertices_end(), point, Kernel() )
                !=  CGAL::ON_UNBOUNDED_SIDE ) {
            return false;
        }
    }

    return true;
}



template < typename PointOutputIteratorType>
PointOutputIteratorType difference( const Point_2& a, const Point_2& b, PointOutputIteratorType out )
{
    if ( a != b ) {
        *out++ = a;
    }

    return out;
}

template < typename PointOutputIteratorType>
PointOutputIteratorType difference( const Point_2& a, const Segment_2& b, PointOutputIteratorType out )
{
    if ( ! CGAL::do_intersect( a, b ) ) {
        *out++ = a;
    }

    return out;
}

template < typename PointOutputIteratorType>
PointOutputIteratorType difference( const Point_2& a, const PolygonWH_2& b, PointOutputIteratorType out )
{
    if ( ! do_intersect( a, b ) ) {
        *out++ = a;
    }

    return out;
}

template < typename PointOutputIteratorType>
PointOutputIteratorType difference( const Point_2&, const NoVolume& , PointOutputIteratorType out )
{
    BOOST_ASSERT( false );
    return out;
}

template < typename SegmentOutputIteratorType>
SegmentOutputIteratorType difference( const Segment_2&, const NoVolume& , SegmentOutputIteratorType out )
{
    BOOST_ASSERT( false );
    return out;
}

template < typename SurfaceOutputIteratorType>
SurfaceOutputIteratorType difference( const PolygonWH_2&, const NoVolume& , SurfaceOutputIteratorType out )
{
    BOOST_ASSERT( false );
    return out;
}


template < typename PointOutputIteratorType>
PointOutputIteratorType difference( const Point_3& a, const Point_3& b, PointOutputIteratorType out )
{
    if ( a != b ) {
        *out++ = a;
    }

    return out;
}

template < typename PointOutputIteratorType>
PointOutputIteratorType difference( const Point_3& a, const Segment_3& b, PointOutputIteratorType out )
{
    if ( ! b.has_on( a ) ) {
        *out++ = a;
    }

    return out;
}

template < typename PointOutputIteratorType>
PointOutputIteratorType difference( const Point_3& a, const Triangle_3& b, PointOutputIteratorType out )
{
    if ( ! b.has_on( a ) ) {
        *out++ = a;
    }

    return out;
}

template < typename PointOutputIteratorType>
PointOutputIteratorType difference( const Point_3& a, const MarkedPolyhedron& b, PointOutputIteratorType out )
{
    CGAL::Point_inside_polyhedron_3<MarkedPolyhedron, Kernel> is_in_poly( b );

    if ( CGAL::ON_UNBOUNDED_SIDE == is_in_poly( a ) ) {
        *out++ = a;
    }

    return out;
}


template < typename SegmentType , typename SegmentOrSurfaceType, typename SegmentOutputIteratorType>
SegmentOutputIteratorType difference( const SegmentType& a, const SegmentOrSurfaceType& b, SegmentOutputIteratorType out )
{
    CGAL::Object inter = CGAL::intersection( a, b );
    const SegmentType* s = CGAL::object_cast< SegmentType >( &inter );

    if ( s ) { // there maybe zero, one or two segments as a result
        if ( CGAL::squared_distance( a.source(), s->source() ) < CGAL::squared_distance( a.source(), s->target() ) ) {
            if ( a.source() != s->source() ) {
                *out++ = SegmentType( a.source(), s->source() );
            }

            if ( s->target() != a.target() ) {
                *out++ = SegmentType( s->target(), a.target() );
            }
        }
        else {
            if ( a.source() != s->target() ) {
                *out++ = SegmentType( a.source(), s->target() );
            }

            if ( s->source() != a.target() ) {
                *out++ = SegmentType( s->source(), a.target() );
            }
        }
    }
    else { // intersection is a point or nothing, leave a unchanged
        *out++ = a;
    }

    return out;
}

template<typename PointType>
struct Nearer {
    Nearer( const PointType& reference ) :_ref( reference ) {}
    bool operator()( const PointType& lhs, const PointType& rhs ) const {
        return CGAL::squared_distance( _ref, lhs ) < CGAL::squared_distance( _ref, rhs );
    }
private:
    const PointType _ref;
};

template < typename SegmentOutputIteratorType>
SegmentOutputIteratorType difference( const Segment_2& segment, const PolygonWH_2& polygon, SegmentOutputIteratorType out )
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

    std::vector< Segment_2 > result( 1, segment );
    std::vector< Polygon_2 > rings( 1, polygon.outer_boundary() );
    rings.insert( rings.end(), polygon.holes_begin(), polygon.holes_end() );

    for ( std::vector< Polygon_2 >::iterator ring = rings.begin(); ring != rings.end(); ++ring ) {
        for ( Polygon_2::Vertex_const_iterator target = ring->vertices_begin();
                target != ring->vertices_end(); ++target ) {
            const Segment_2 sc( target == ring->vertices_begin()
                                ? *( ring->vertices_end() - 1 )
                                : *( target - 1 )
                                ,
                                *target );
            std::vector< Segment_2 > tmp;

            for ( std::vector< Segment_2 >::const_iterator s = result.begin(); s != result.end(); ++s ) {
                difference( *s, sc, std::back_inserter( tmp ) );
            }

            tmp.swap( result );
        }
    }

    for ( std::vector< Segment_2 >::const_iterator s = result.begin(); s != result.end(); ++s ) {
        std::vector< Point_2 > points( 1, s->source() );

        for ( std::vector< Polygon_2 >::iterator ring = rings.begin(); ring != rings.end(); ++ring ) {
            for ( Polygon_2::Vertex_const_iterator target = ring->vertices_begin();
                    target != ring->vertices_end(); ++target ) {
                Segment_2 sc( target == ring->vertices_begin()
                              ? *( ring->vertices_end() - 1 )
                              : *( target - 1 )
                              ,
                              *target );
                CGAL::Object inter = CGAL::intersection( *s, sc );
                const Point_2* p = CGAL::object_cast< Point_2 >( &inter );

                if ( p ) {
                    points.push_back( *p );
                }
            }
        }

        points.push_back( s->target() );
        // order point according to the distance from source
        const Nearer<Point_2> nearer( s->source() );
        std::sort( points.begin()+1, points.end()-1, nearer );

        // append segments that has length and wich midpoint is outside polygon to result
        for ( std::vector< Point_2 >::const_iterator p = points.begin(); p != points.end()-1; ++p ) {
            std::vector< Point_2 >::const_iterator q = p+1;

            if ( *p != *q && !do_intersect( CGAL::midpoint( *p,*q ), polygon ) ) {
                *out++ = Segment_2( *p, *q );
            }
        }
    }

    return out;
}

inline
PolygonWH_2
fix_cgal_valid_polygon( const PolygonWH_2& p )
{
    const Polygon_2& outer = p.outer_boundary();

    for ( Polygon_2::Vertex_const_iterator v = outer.vertices_begin();
            v != outer.vertices_end(); ++v ) {
        for ( Polygon_2::Vertex_const_iterator o = v+1; o != outer.vertices_end(); ++o ) {
            if ( *o == *v ) {
                BOOST_THROW_EXCEPTION( NotImplementedException( "Fixing a polygon which exterior ring self intersects is not implemented" ) );
            }
        }
    }

    return p;
}

inline
PolygonWH_2
fix_sfs_valid_polygon( const PolygonWH_2& p )
{
    // a polygon is valid for sfs and invalid for CGAL when two rings intersect
    // on a point that is not a ring vertex
    // we add this vertex to fix the polygon
    // for each ring segment
    //    for every other ring point
    //        add point to segment

    // put all rings in a vector to avoid distinction between outer and holes
    std::vector< Polygon_2 > rings( 1, p.outer_boundary() );
    rings.insert( rings.end(), p.holes_begin(), p.holes_end() );

    std::vector< Polygon_2 > out;

    for ( std::vector< Polygon_2 >::iterator ring = rings.begin(); ring != rings.end(); ++ring ) {
        out.push_back( Polygon_2() );

        for ( Polygon_2::Vertex_const_iterator target = ring->vertices_begin(); target != ring->vertices_end(); ++target ) {
            Segment_2 segment(
                target == ring->vertices_begin()
                ? *( ring->vertices_end() - 1 )
                : *( target - 1 )
                ,
                *target );

            // for every other ring
            for ( std::vector< Polygon_2 >::const_iterator other = rings.begin(); other != rings.end(); ++other ) {
                if ( ring == other ) {
                    continue;
                }

                for ( Polygon_2::Vertex_const_iterator vertex = other->vertices_begin();
                        vertex != other->vertices_end(); ++vertex ) {
                    if ( CGAL::do_intersect( *vertex, segment ) ) {
                        out.back().push_back( *vertex );
                    }
                }
            }

            out.back().push_back( *target );
        }
    }

    return PolygonWH_2( out[0], out.begin()+1, out.end() );
}

template < typename OutputIteratorType >
OutputIteratorType difference( const Triangle_3& p, const Triangle_3& q, OutputIteratorType out )
{

    const Plane_3 plane = p.supporting_plane();

    if ( plane != q.supporting_plane() || !CGAL::do_intersect( p,q ) ) {
        *out++ = p;
    }
    else {
        // project on plane
        // difference between polygons
        // triangulate the result

        PolygonWH_2 pProj, qProj;

        for ( unsigned i=0; i<3; i++ ) {
            pProj.outer_boundary().push_back( plane.to_2d( p.vertex( i ) ) );
            qProj.outer_boundary().push_back( plane.to_2d( q.vertex( i ) ) );
        }

        std::vector< PolygonWH_2 > res;
        difference( pProj, qProj, std::back_inserter( res ) );


        for ( std::vector< PolygonWH_2 >::const_iterator i = res.begin(); i != res.end(); ++i ) {
            const Polygon poly( *i );
            TriangulatedSurface ts;
            triangulate::triangulatePolygon3D( poly, ts );

            for ( TriangulatedSurface::iterator t = ts.begin(); t != ts.end(); ++t ) {
                *out++ = Triangle_3( plane.to_3d( t->vertex( 0 ).toPoint_2() ),
                                     plane.to_3d( t->vertex( 1 ).toPoint_2() ),
                                     plane.to_3d( t->vertex( 2 ).toPoint_2() ) ) ;
            }
        }
    }

    return out;
}

template < typename VolumeOutputIteratorType>
VolumeOutputIteratorType difference( const MarkedPolyhedron& a, const MarkedPolyhedron& b, VolumeOutputIteratorType out )
{
    MarkedPolyhedron& p = const_cast<MarkedPolyhedron&>( a );
    MarkedPolyhedron& q = const_cast<MarkedPolyhedron&>( b );
    typedef CGAL::Polyhedron_corefinement<MarkedPolyhedron> Corefinement;
    Corefinement coref;
    CGAL::Emptyset_iterator no_polylines;
    typedef std::vector<std::pair<MarkedPolyhedron*, int> >  ResultType;
    ResultType result;
    coref( p, q, no_polylines, std::back_inserter( result ), Corefinement::P_minus_Q_tag );

    for ( ResultType::iterator it = result.begin(); it != result.end(); it++ ) {
        *out++ = *it->first;
        delete it->first;
    }

    return out;
}

typedef  CGAL::Box_intersection_d::Box_with_handle_d<double,3,MarkedPolyhedron::Halfedge_around_facet_const_circulator> FaceBboxBase;


struct FaceBbox: FaceBboxBase {
    struct Bbox: CGAL::Bbox_3 {
        Bbox( MarkedPolyhedron::Halfedge_around_facet_const_circulator handle )
            : CGAL::Bbox_3( handle->vertex()->point().bbox() ) {
            const MarkedPolyhedron::Halfedge_around_facet_const_circulator end = handle;

            do {
                // @note with CGAL 4.5 you would write simply
                // *this += (++handle)->vertex()->point().bbox();
                this->CGAL::Bbox_3::operator=( *this + ( ++handle )->vertex()->point().bbox() );
            }
            while ( handle != end );
        }
    };

    FaceBbox( const MarkedPolyhedron::Facet& facet )
        : FaceBboxBase( Bbox( facet.facet_begin() ), facet.facet_begin() ) {
    }
};

struct FaceSegmentCollide {
    typedef std::vector< MarkedPolyhedron::Halfedge_around_facet_const_circulator > CollisionVector;
    FaceSegmentCollide( CollisionVector& list ): _list( list ) {}
    void operator()( const FaceBboxBase&, const FaceBboxBase& face ) {
        _list.push_back( face.handle() );
    }
private:
    CollisionVector& _list;
};

template < typename TriangleOutputIteratorType>
TriangleOutputIteratorType collidingTriangles( const FaceSegmentCollide::CollisionVector& collisions, TriangleOutputIteratorType out )
{
    for ( FaceSegmentCollide::CollisionVector::const_iterator cit = collisions.begin();
            cit != collisions.end(); ++cit ) {
        MarkedPolyhedron::Halfedge_around_facet_const_circulator it = *cit;
        std::vector< Point > points( 1, it->vertex()->point() );

        do {
            points.push_back( ( ++it )->vertex()->point() );
        }
        while ( it != *cit );

        if ( points.size() == 3 ) {
            *out++ = Triangle_3( points[0].toPoint_3(), points[1].toPoint_3(), points[2].toPoint_3() ) ;
        }
        else {
            const Polygon poly( points );
            TriangulatedSurface ts;
            triangulate::triangulatePolygon3D( poly, ts );

            for ( TriangulatedSurface::iterator t = ts.begin(); t != ts.end(); ++t ) {
                *out++ = Triangle_3( t->vertex( 0 ).toPoint_3(),
                                     t->vertex( 1 ).toPoint_3(),
                                     t->vertex( 2 ).toPoint_3() );
            }
        }
    }

    return out;
}


template < typename SegmentOutputIteratorType>
SegmentOutputIteratorType difference( const Segment_3& segment, const MarkedPolyhedron& polyhedron, SegmentOutputIteratorType out )
{
    // this is a bit of a pain
    // the algo should follow the same lines as the Segment_2 - PolygonWH_2
    // namely, remove the pieces of the segment were it touches facets,
    // then compute the intersections with facets to cut the segments and
    // create segments for output were the middle point is inside
    //
    // to speed thing up we put facets in AABB-Tree

    std::vector< FaceBbox > bboxes( polyhedron.facets_begin(), polyhedron.facets_end() );
    std::vector< FaceBboxBase > bbox( 1, FaceBboxBase( segment.bbox(),polyhedron.facets_begin()->facet_begin() ) ); // nevermind the facet handle, it's not used anyway
    FaceSegmentCollide::CollisionVector collisions;
    FaceSegmentCollide cb( collisions );
    CGAL::box_intersection_d( bbox.begin(), bbox.end(),
                              bboxes.begin(), bboxes.end(),
                              cb );

    if ( !collisions.size() ) {
        // completely in or out, we just test one point
        CGAL::Point_inside_polyhedron_3<MarkedPolyhedron, Kernel> is_in_poly( polyhedron );

        if ( CGAL::ON_UNBOUNDED_SIDE == is_in_poly( segment.source() ) ) {
            *out++ = segment;
        }
    }
    else {
        std::vector< Triangle_3 > triangles;
        collidingTriangles( collisions, std::back_inserter( triangles ) );

        // first step, substract faces
        std::vector< Segment_3 > res1( 1, segment );

        for ( std::vector< Triangle_3 >::const_iterator tri=triangles.begin();
                tri != triangles.end(); ++tri ) {
            std::vector< Segment_3 > tmp;

            for ( std::vector< Segment_3 >::const_iterator seg = res1.begin();
                    seg != res1.end(); ++seg ) {
                difference( *seg, *tri, std::back_inserter( tmp ) );
            }

            res1.swap( tmp );
        }

        // second step, for each segment, add intersection points and test each middle point
        // to know if it's in or out
        for ( std::vector< Segment_3 >::const_iterator seg = res1.begin();
                seg != res1.end(); ++seg ) {
            std::vector< Point_3 > points( 1, seg->source() );

            for ( std::vector< Triangle_3 >::const_iterator tri=triangles.begin();
                    tri != triangles.end(); ++tri ) {
                CGAL::Object inter = CGAL::intersection( *seg, *tri );
                const Point_3* p = CGAL::object_cast< Point_3 >( &inter );

                if ( p ) {
                    points.push_back( *p );
                }
            }

            points.push_back( seg->target() );
            // order point according to the distance from source

            const Nearer<Point_3> nearer( seg->source() );
            std::sort( points.begin()+1, points.end()-1, nearer );

            CGAL::Point_inside_polyhedron_3<MarkedPolyhedron, Kernel> is_in_poly( polyhedron );

            // append segments that has length and wich midpoint is outside polyhedron to result
            for ( std::vector< Point_3 >::const_iterator p = points.begin(); p != points.end()-1; ++p ) {
                std::vector< Point_3 >::const_iterator q = p+1;

                if ( *p != *q && CGAL::ON_UNBOUNDED_SIDE == is_in_poly( CGAL::midpoint( *p,*q ) ) ) {
                    *out++ = Segment_3( *p, *q );
                }
            }
        }
    }

    return out;
}

// @TODO put that in a proper header
void _intersection_solid_triangle( const MarkedPolyhedron& pa, const Triangle_3& tri, GeometrySet<3>& output );

template < typename TriangleOutputIteratorType>
TriangleOutputIteratorType intersection( const Triangle_3& triangle, const MarkedPolyhedron& polyhedron, TriangleOutputIteratorType out )
{
    // call _intersection_solid_triangle
    GeometrySet<3> res;
    _intersection_solid_triangle( polyhedron, triangle, res );

    for ( GeometrySet<3>::SurfaceCollection::const_iterator it = res.surfaces().begin();
            it != res.surfaces().end(); ++it ) {
        *out++ = it->primitive();
    }

    return out;
}

template < typename TriangleOutputIteratorType>
TriangleOutputIteratorType difference( const Triangle_3& triangle, const MarkedPolyhedron& polyhedron, TriangleOutputIteratorType out )
{
    std::vector< Triangle_3 > inter;
    intersection( triangle, polyhedron, std::back_inserter( inter ) );

    std::vector< Triangle_3 > res( 1, triangle );

    for ( std::vector< Triangle_3 >::const_iterator it = inter.begin(); it != inter.end(); ++it ) {
        std::vector< Triangle_3 > tmp;

        for ( std::vector< Triangle_3 >::const_iterator tri = res.begin(); tri != res.end(); ++tri ) {
            difference( *tri, *it, std::back_inserter( tmp ) );
        }

        tmp.swap( res );
    }

    for ( std::vector< Triangle_3 >::const_iterator tri = res.begin(); tri != res.end(); ++tri ) {
        *out++ = *tri;
    }

    return out;

    /*
        std::vector< FaceBbox > bboxes(polyhedron.facets_begin(), polyhedron.facets_end() );
        std::vector< FaceBboxBase > bbox( 1, FaceBboxBase(triangle.bbox(),polyhedron.facets_begin()->facet_begin()) ); // nevermind the facet handle, it's not used anyway
        FaceSegmentCollide::CollisionVector collisions;
        FaceSegmentCollide cb(collisions);
        CGAL::box_intersection_d( bbox.begin(), bbox.end(),
                                  bboxes.begin(), bboxes.end(),
                                  cb );

        if ( !collisions.size() ){
            // completely in or out, we just test one point
            CGAL::Point_inside_polyhedron_3<MarkedPolyhedron, Kernel> is_in_poly( polyhedron );
            if ( CGAL::ON_UNBOUNDED_SIDE == is_in_poly( triangle.vertex(0) ) ) *out++ = triangle;
        }
        else {
            // now we first transform bboxes colliding faces into triangles
            // then we test for intersection and store resulting segments in a vector
            // we also store resulting polygons as segments
            //
            // we need to convert the resulting segments to a multipolygon of sort
            //
            // finally we triangulate the result and substract those triangles
            //
            std::vector< Triangle_3 > interTriangles;
            collidingTriangles( collisions, std::back_inserter( interTriangles ) );

            std::vector< Segment_3 > intersectionCountours;

            BOOST_THROW_EXCEPTION(NotImplementedException("Triangle_3 - Volume is not implemented") );
        }
        return out;
    */
}

template < typename PolygonOutputIteratorType>
PolygonOutputIteratorType difference( const PolygonWH_2& a, const PolygonWH_2& b, PolygonOutputIteratorType out )
{
    CGAL::Gps_segment_traits_2<Kernel> traits;

    std::vector< PolygonWH_2 > temp;
    CGAL::difference(
        are_holes_and_boundary_pairwise_disjoint( a, traits ) ? a : fix_sfs_valid_polygon( a ),
        are_holes_and_boundary_pairwise_disjoint( b, traits ) ? b : fix_sfs_valid_polygon( b ),
        std::back_inserter( temp ) );

    // polygon outer rings from difference can self intersect at points
    // therefore we need to split the generated polygons so that they are valid for SFS
    for ( std::vector< PolygonWH_2 >::const_iterator poly=temp.begin(); poly!=temp.end(); ++poly  ) {
        *out++ = fix_cgal_valid_polygon( *poly );
    }

    return out;
}

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

}
}

