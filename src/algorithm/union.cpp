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
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/union.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/detail/triangulate/ConstraintDelaunayTriangulation.h>

#include <cstdio>
#include <algorithm>

#define DEBUG_OUT if (0) std::cerr << __FILE__ << ":" << __LINE__ << " debug: "


namespace SFCGAL {

namespace algorithm {

struct EmptyPrimitive {};

enum PrimitiveType {
    PrimitivePoint = 0,
    PrimitiveSegment = 1,
    PrimitiveSurface = 2,
    PrimitiveVolume = 3,
    PrimitiveEmpty = 4
};

template <int Dim>
struct Segment_d: detail::Segment_d<Dim>::Type {
    typedef typename detail::Point_d<Dim>::Type PointType;
    typedef typename detail::Segment_d<Dim>::Type SegmentType;
    typedef typename std::vector< PointType > PointVector;
    typedef typename std::vector< SegmentType > SegmentVector;

    Segment_d( const SegmentType& s ): SegmentType( s ) {}
    void splitAt( const PointType& p ) {
        _split.push_back( p );
    }
    void remove( const SegmentType& s ) {
        _split.push_back( s.source() );
        _split.push_back( s.target() );
        _remove.push_back( s );
    }

    template < class OutputIterator >
    OutputIterator pieces( OutputIterator out ) const {
        PointVector points( 1, this->source() );
        points.insert( points.end(), _split.begin(), _split.end() );
        points.push_back( this->target() );
        std::sort( points.begin()+1, points.end()-1, Nearer< PointType >( this->source() ) );

        for ( typename PointVector::const_iterator p = points.begin(), q = p+1;
                q != points.end(); ++p, q++ ) {
            if ( *p != *q ) {
                PointType m = CGAL::midpoint( *p, *q );
                typename SegmentVector::const_iterator r = _remove.begin();

                for ( ; r != _remove.end() && !r->has_on( m ); ++r );

                if ( r == _remove.end() ) {
                    *out++ = SegmentType( *p, *q );
                }
            }
        }

        return out;
    }

    SegmentVector pieces() const {
        SegmentVector result;
        this->pieces( std::back_inserter( result ) );
        return result;
    }

private:
    PointVector _split;
    SegmentVector _remove;
};

template <int Dim> struct Surface_d {};

template <>
struct Surface_d<3>: Triangle_3 {
    typedef std::vector< algorithm::Point_2 > PointVector;
    typedef std::vector< Segment_2 > SegmentVector;
    typedef std::vector< PointVector > SurfaceVector;

    Surface_d( const Triangle_3& s ): Triangle_3( s ), _plane( s.supporting_plane() ) {
        this->splitAt( s );
    }

    void splitAt( const algorithm::Point_3& p ) {
        //@note this is a degenerated segment, but works anyway
        _split.push_back( Segment_2( _plane.to_2d( p ), _plane.to_2d( p ) ) );
    }

    void splitAt( const Segment_3& s ) {
        _split.push_back( Segment_2( _plane.to_2d( s.source() ), _plane.to_2d( s.target() ) ) );
    }

    template <typename Point3Iterator>
    void splitAt( Point3Iterator begin, Point3Iterator end ) { // polygon with unclosed ring
        if ( begin == end ) {
            return;
        }

        Point3Iterator s = begin, t = s+1;

        for ( ; t != end; ++t, ++s ) {
            _split.push_back( Segment_2( _plane.to_2d( *s ), _plane.to_2d( *t ) ) );
        }

        _split.push_back( Segment_2( _plane.to_2d( *s ), _plane.to_2d( *begin ) ) );
    }

    void splitAt( const Triangle_3& t ) {
        const algorithm::Point_3 v[3] = { t.vertex( 0 ), t.vertex( 1 ), t.vertex( 2 ) };
        this->splitAt( v, v+3 );
    }

    void splitAt( const std::vector< algorithm::Point_3 >& p ) { // polygon with unclosed ring
        this->splitAt( p.begin(), p.end() );
    }

    template <typename Point3Iterator> // polygon with unclosed ring
    void remove( Point3Iterator begin, Point3Iterator end ) {
        if ( begin == end ) {
            return;
        }

        this->splitAt( begin, end );
        PointVector v;

        for ( Point3Iterator i = begin; i != end; ++i ) {
            v.push_back( _plane.to_2d( *i ) );
        }

        _remove.push_back( v );
    }

    void remove( const std::vector< algorithm::Point_3 >& p ) {
        this->remove( p.begin(), p.end() );
    }

    void remove( const Triangle_3& t ) {
        const algorithm::Point_3 v[3] = { t.vertex( 0 ), t.vertex( 1 ), t.vertex( 2 ) };
        this->remove( v, v+3 );
    }

    std::vector< Triangle_3 > pieces() {
        // we need to process the split lines because there may be several lines at the same place
        // and this won't play nice with the triangulation, the same stands for the lines lying
        // on the triangle edges
        //
        // after that we just check, for each triangle, if a point fall in a removed part and remove it
        // we can do that by pairwise union of all segments
        std::vector< Segment_2 > filtered;
        {
            std::vector< Segment_d<2> > lines( _split.begin(), _split.begin()+3 );

            for ( typename SegmentVector::const_iterator c = _split.begin()+3; c != _split.end(); ++c ) {
                Segment_d<2> current( *c );

                for ( std::vector< Segment_d<2> >::iterator l = lines.begin(); l != lines.end(); ++l ) {
                    CGAL::Object inter = CGAL::intersection( *l, current );
                    const Point_2* p = CGAL::object_cast< Point_2 >( &inter );
                    const Segment_2* s = CGAL::object_cast< Segment_2 >( &inter );

                    if ( p ) {
                        current.splitAt( *p );
                        l->splitAt( *p );
                    }
                    else if ( s ) {
                        current.remove( *s );
                        l->splitAt( s->source() );
                        l->splitAt( s->target() );
                    }
                }

                lines.push_back( current );
            }

            for ( std::vector< Segment_d<2> >::const_iterator l = lines.begin(); l != lines.end(); ++l ) {
                l->pieces( std::back_inserter( filtered ) );
            }
        }

        DEBUG_OUT << "triangulating " << filtered.size() << " lines\n";

        // now we want to triangulate
        TriangulatedSurface ts;
        {
            typedef triangulate::ConstraintDelaunayTriangulation::Vertex_handle Vertex_handle;
            triangulate::ConstraintDelaunayTriangulation cdt;

            for ( std::vector< Segment_2 >::const_iterator f = filtered.begin(); f != filtered.end(); ++f ) {
                Vertex_handle s = cdt.addVertex( f->source() );
                Vertex_handle t = cdt.addVertex( f->target() );
                cdt.addConstraint( s, t ) ;
            }

            cdt.getTriangles( ts );
        }

        // filter removed triangles
        std::vector< Triangle_3 > res;

        for ( TriangulatedSurface::iterator t = ts.begin(); t != ts.end(); ++t ) {
            // define a point inside triangle
            const Point_2 a( t->vertex( 0 ).toPoint_2<Kernel>() );
            const Point_2 b( t->vertex( 1 ).toPoint_2<Kernel>() );
            const Point_2 c( t->vertex( 2 ).toPoint_2<Kernel>() );
            const Point_2 point( a + ( Vector_2( a, b ) + Vector_2( a, c ) )/3 );

            // find if triangle is in a removed spot
            SurfaceVector::const_iterator r = _remove.begin();

            for ( ; r != _remove.end()
                    && CGAL::bounded_side_2( r->begin(), r->end(), point, Kernel() )
                    == CGAL::ON_UNBOUNDED_SIDE; ++r );

            if ( r == _remove.end() ) {
                res.push_back( Triangle_3( _plane.to_3d( a ), _plane.to_3d( b ), _plane.to_3d( c ) ) );
            }
        }

        DEBUG_OUT << "generated " << res.size() << " triangles\n";

        return res;
    }

private:
    algorithm::Plane_3 _plane;
    SegmentVector _split;
    SurfaceVector _remove;
};

template <>
struct Surface_d<2>: PolygonWH_2 {
    typedef std::vector< Point_2 > PointVector;
    typedef std::vector< Segment_2 > SegmentVector;
    typedef std::vector< PointVector > SurfaceVector;

    Surface_d( const PolygonWH_2& s ): PolygonWH_2( s ) {}

    void splitAt( const Segment_2& s ) {
        _split.push_back( s );
    }

    void addSplitsFrom( const Surface_d<2>& other ) {
        _split.insert( _split.end(), other._split.begin(), other._split.end() );
    }

    std::vector< PolygonWH_2 > pieces() const {
        std::vector< PolygonWH_2 > res;
        fix_cgal_valid_polygon( *this, std::back_inserter( res ) );
        return res;
    }

private:
    SegmentVector _split;
};

// for debug prints
template <typename T >
std::ostream& operator<<( std::ostream& out, std::set< T* >& obs )
{
    for ( typename std::set< T* >::iterator h = obs.begin(); h != obs.end(); ++h ) {
        out << *h << "\n";
    }

    return out;
}


// takes care of RAII of primitives

template <int Dim>
class Handle {
    struct ObservablePrimitive: boost::variant<
            typename detail::Point_d<Dim>::Type,
            Segment_d<Dim>,
            Surface_d<Dim>,
            typename detail::Volume_d<Dim>::Type,
            EmptyPrimitive > {
        template <class T>
        ObservablePrimitive( const T& p ) :  boost::variant<
            typename detail::Point_d<Dim>::Type,
            Segment_d<Dim>,
            Surface_d<Dim>,
            typename detail::Volume_d<Dim>::Type,
            EmptyPrimitive >( p ) {}

        template <class T>
        inline T& as() {
            return boost::get<T&>( *this );
        }

        std::set< ObservablePrimitive** > _observers;  // this is for ref counting and handle updating

    private:
        // non copyable
        ObservablePrimitive( const ObservablePrimitive& );
        ObservablePrimitive& operator=( const ObservablePrimitive& );
    };

public:


    Handle(): _p( new ObservablePrimitive*( new ObservablePrimitive( EmptyPrimitive() ) ) ) {
        ( *_p )->_observers.insert( _p );
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
    }

    ~Handle() {
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        ( *_p )->_observers.erase( _p );

        if ( ( *_p )->_observers.empty() ) {
            delete ( *_p );
        }

        delete _p;
    }

    Handle( const Handle& other ):_p( new ObservablePrimitive*( *other._p ) ) {
        ( *_p )->_observers.insert( _p );
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
    }

    template < class PrimitiveType >
    explicit Handle( const PrimitiveType& primitive ): _p( new ObservablePrimitive*(  new ObservablePrimitive( primitive ) ) ) {
        ( *_p )->_observers.insert( _p );
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
    }

    void swap( Handle& other ) {
        ( *_p )->_observers.erase( _p );
        ( *other._p )->_observers.erase( other._p );
        std::swap( _p, other._p );
        ( *_p )->_observers.insert( _p );
        ( *other._p )->_observers.insert( other._p );
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        BOOST_ASSERT( ( *other._p )->_observers.count( other._p ) );
    }

    Handle& operator=( Handle other ) {
        this->swap( other );
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        return *this;
    }

    const ObservablePrimitive& operator*() const {
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        return *( *_p );
    }
    ObservablePrimitive& operator*() {
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        return *( *_p );
    }
    const ObservablePrimitive* operator->() const {
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        return ( *_p );
    }
    ObservablePrimitive* operator->() {
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        return ( *_p );
    }

    typename detail::Point_d<Dim>::Type& asPoint() {
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        BOOST_ASSERT( which() == PrimitivePoint );
        return boost::get<typename detail::Point_d<Dim>::Type& >( *( *_p ) );
    }

    Segment_d<Dim>& asSegment() {
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        BOOST_ASSERT( which() == PrimitiveSegment );
        return boost::get<Segment_d<Dim> & >( *( *_p ) );
    }

    Surface_d<Dim>& asSurface() {
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        BOOST_ASSERT( which() == PrimitiveSurface );
        return boost::get<Surface_d<Dim> & >( *( *_p ) );
    }

    typename detail::Volume_d<Dim>::Type& asVolume() {
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
        BOOST_ASSERT( which() == PrimitiveVolume );
        return boost::get<typename detail::Volume_d<Dim>::Type& >( *( *_p ) );
    }

    PrimitiveType which() {
        return PrimitiveType( ( *_p )->which() );
    }

    bool empty() {
        return which() == PrimitiveEmpty;
    }

    // makes all handles observing a observe 'this' instead
    void registerObservers( Handle a ) {
        if ( *a._p == *_p ) {
            return;    // both aready observing the same primitive
        }

        ObservablePrimitive* observed = *( a._p );
        std::vector< ObservablePrimitive** > observers( observed->_observers.begin(), observed->_observers.end() );

        for ( typename std::vector< ObservablePrimitive** >::iterator h = observers.begin(); h != observers.end(); ++h ) {
            *( *h ) = *_p;
            ( *( *h ) )->_observers.insert( *h );
        }

        BOOST_ASSERT( *a._p == *_p );

        delete observed; // we removed all observers
        BOOST_ASSERT( ( *_p )->_observers.count( _p ) );
#ifdef DEBUG

        for ( typename std::vector< ObservablePrimitive** >::iterator h = observers.begin(); h != observers.end(); ++h ) {
            BOOST_ASSERT( ( *( *h ) )->_observers.count( *h ) );
        }

#endif

    }

private:
    ObservablePrimitive** _p;  // mutable because no non const cpy ctor en template classes
};

template <int Dim>
struct HandledBox {
    typedef CGAL::Box_intersection_d::Box_with_handle_d<double, Dim, Handle<Dim>, CGAL::Box_intersection_d::ID_EXPLICIT > Type;
    typedef std::vector< Type > Vector;
};

template <int Dim, class OutputIterator>
OutputIterator compute_bboxes( const detail::GeometrySet<Dim>& gs, OutputIterator out )
{
    typename HandledBox<Dim>::Vector bboxes;

    for ( typename detail::GeometrySet<Dim>::PointCollection::const_iterator it = gs.points().begin();
            it != gs.points().end(); ++it ) {
        *out++ = typename HandledBox<Dim>::Type( it->primitive().bbox(), Handle<Dim>( it->primitive() ) );
    }

    for ( typename detail::GeometrySet<Dim>::SegmentCollection::const_iterator it = gs.segments().begin();
            it != gs.segments().end(); ++it ) {
        *out++ = typename HandledBox<Dim>::Type( it->primitive().bbox(), Handle<Dim>( it->primitive() ) );
    }

    for ( typename detail::GeometrySet<Dim>::SurfaceCollection::const_iterator it = gs.surfaces().begin();
            it != gs.surfaces().end(); ++it ) {
        DEBUG_OUT << "Adding surface " << it->primitive() << "\n";
        DEBUG_OUT << "       surface box " << it->primitive().bbox() << "\n";
        *out++ = typename HandledBox<Dim>::Type( it->primitive().bbox(), Handle<Dim>( it->primitive() ) );
    }

    for ( typename detail::GeometrySet<Dim>::VolumeCollection::const_iterator it = gs.volumes().begin();
            it != gs.volumes().end(); ++it ) {
        *out++ = typename HandledBox<Dim>::Type( compute_solid_bbox( it->primitive(), detail::dim_t<Dim>() ), Handle<Dim>( it->primitive() ) );
    }

    return out;
}

template <class Handle>
void union_point_point( Handle a, Handle b )
{
    if ( a.asPoint() == b.asPoint() ) {
        a.registerObservers( b );
    }
}

template <class Handle>
void union_point_segment( Handle a, Handle b )
{
    if ( b.asSegment().has_on( a.asPoint() ) ) {
        b.asSegment().splitAt( a.asPoint() );
        b.registerObservers( a );
    }
}

void union_point_surface( Handle<2> a,Handle<2> b )
{
    if ( do_intersect( a.asPoint(), b.asSurface() ) ) {
        b.registerObservers( a );
    }
}

void union_point_surface( Handle<3> a,Handle<3> b )
{
    if ( b.asSurface().has_on( a.asPoint() ) ) {
        b.registerObservers( a );
    }
}

void union_point_volume( Handle<2> ,Handle<2>  )
{
    BOOST_ASSERT( false ); // there shouldn't be any volume in 2D
}

void union_point_volume( Handle<3> a,Handle<3> b )
{
    //@todo put is in poly in a struct derived from MarkedPolyhedron to avoid rebuilding point inside every time
    Point_inside_polyhedron<MarkedPolyhedron, Kernel> is_in_poly( b.asVolume() );

    if ( CGAL::ON_UNBOUNDED_SIDE != is_in_poly( a.asPoint() ) ) {
        b.registerObservers( a );
    }
}



template <int Dim>
void union_segment_segment( Handle<Dim> a,Handle<Dim> b )
{
    typedef typename detail::TypeForDimension<Dim>::Segment SegmentType;
    typedef typename detail::TypeForDimension<Dim>::Point PointType;

    CGAL::Object inter = CGAL::intersection( a.asSegment(), b.asSegment() );
    const PointType* p = CGAL::object_cast< PointType >( &inter );
    const SegmentType* s = CGAL::object_cast< SegmentType >( &inter );

    if ( p ) {
        b.asSegment().splitAt( *p );
        a.asSegment().splitAt( *p );
    }
    else if ( s ) {
        b.asSegment().remove( *s );
        a.asSegment().splitAt( s->source() );
        a.asSegment().splitAt( s->target() );
    }
}

void union_segment_segment( Handle<2> a,Handle<2> b )
{
    union_segment_segment<2>( a, b );
}

void union_segment_segment( Handle<3> a,Handle<3> b )
{
    union_segment_segment<3>( a, b );
}

void union_segment_surface( Handle<2> a,Handle<2> b )
{
    std::vector< Polygon_2 > rings( 1, b.asSurface().outer_boundary() );
    rings.insert( rings.end(), b.asSurface().holes_begin(), b.asSurface().holes_end() );

    std::vector< Point_2 > points( 1, a.asSegment().source() );

    for ( std::vector< Polygon_2 >::iterator ring = rings.begin(); ring != rings.end(); ++ring ) {
        for ( Polygon_2::Vertex_const_iterator target = ring->vertices_begin();
                target != ring->vertices_end(); ++target ) {
            const Segment_2 sc( target == ring->vertices_begin()
                                ? *( ring->vertices_end() - 1 )
                                : *( target - 1 )
                                ,
                                *target );

            CGAL::Object inter = CGAL::intersection( a.asSegment(), sc );
            const Point_2* p = CGAL::object_cast< Point_2 >( &inter );
            const Segment_2* s = CGAL::object_cast< Segment_2 >( &inter );

            if ( p ) {
                points.push_back( *p );
            }
            else if ( s ) {
                a.asSegment().remove( *s );
            }
        }
    }

    points.push_back( a.asSegment().target() );

    // order point according to the distance from source
    std::sort( points.begin()+1, points.end()-1, Nearer<Point_2>( points[0] ) );

    // cut segment with pieces that have length and wich midpoint is inside polygon
    for ( std::vector< Point_2 >::const_iterator p = points.begin(), q = p+1; q != points.end(); ++p, ++q ) {
        if ( *p != *q && do_intersect( CGAL::midpoint( *p,*q ), b.asSurface() ) ) {
            const Segment_2 s( *p, *q );
            a.asSegment().remove( s );
            b.asSurface().splitAt( s );
        }
    }
}

void union_segment_surface( Handle<3> a,Handle<3> b )
{
    CGAL::Object inter = CGAL::intersection( a.asSegment(), b.asSurface() );
    const Segment_3* s = CGAL::object_cast< Segment_3 >( &inter );

    if ( s ) {
        a.asSegment().remove( *s );
        b.asSurface().splitAt( *s );
    }
}

void union_segment_volume( Handle<2> ,Handle<2> )
{
    BOOST_ASSERT( false ); // there shouldn't be any volume in 2D
}

void union_segment_volume( Handle<3> a,Handle<3> b )
{
    const Segment_3& segment = a.asSegment();
    const MarkedPolyhedron& polyhedron = b.asVolume();

    std::vector< FaceBbox > bboxes( polyhedron.facets_begin(), polyhedron.facets_end() );
    std::vector< FaceBboxBase > bbox( 1, FaceBboxBase( segment.bbox(),polyhedron.facets_begin()->facet_begin() ) ); // nevermind the facet handle, it's not used anyway
    FaceSegmentCollide::CollisionVector collisions;
    FaceSegmentCollide cb( collisions );
    CGAL::box_intersection_d( bbox.begin(), bbox.end(),
                              bboxes.begin(), bboxes.end(),
                              cb );

    Point_inside_polyhedron<MarkedPolyhedron, Kernel> is_in_poly( polyhedron );

    if ( !collisions.size() ) {
        // completely in or out, we just test one point

        if ( CGAL::ON_UNBOUNDED_SIDE != is_in_poly( segment.source() ) ) {
            a.asSegment().remove( a.asSegment() );
        }
    }
    else {
        std::vector< Triangle_3 > triangles;
        collidingTriangles( collisions, std::back_inserter( triangles ) );

        // first step, substract faces
        for ( std::vector< Triangle_3 >::const_iterator tri=triangles.begin();
                tri != triangles.end(); ++tri ) {
            Handle<3> h( *tri );
            union_segment_surface( a, h );
        }

        // second step, for each segment, add intersection points and test each middle point
        // to know if it's in or out
        std::vector< Point_3 > points;

        for ( std::vector< Triangle_3 >::const_iterator tri=triangles.begin();
                tri != triangles.end(); ++tri ) {
            CGAL::Object inter = CGAL::intersection( segment, *tri );
            const Point_3* p = CGAL::object_cast< Point_3 >( &inter );

            if ( p ) {
                points.push_back( *p );
            }
        }

        if ( points.size() ) {
            std::sort( points.begin(), points.end(), Nearer<Point_3>( segment.source() ) );

            // mark segments pieces that have length and wich midpoint is inside polyhedron
            for ( std::vector< Point_3 >::const_iterator p = points.begin(), q = p+1;
                    q != points.end(); ++p, ++q ) {
                if ( *p != *q && CGAL::ON_UNBOUNDED_SIDE != is_in_poly( CGAL::midpoint( *p,*q ) ) ) {
                    a.asSegment().remove( Segment_3( *p, *q ) );
                }
            }
        }
    }
}


void union_surface_surface( Handle<2> a,Handle<2> b )
{
    PolygonWH_2 res;

    if ( CGAL::join( fix_sfs_valid_polygon( a.asSurface() ),
                     fix_sfs_valid_polygon( b.asSurface() ), res ) ) {
        DEBUG_OUT << "merged " << a.asSurface() << " and " << b.asSurface() << "\n";
        Handle<2> h( res );
        h.asSurface().addSplitsFrom( a.asSurface() );
        h.asSurface().addSplitsFrom( b.asSurface() );
        h.registerObservers( a );
        h.registerObservers( b );
    }
}

void union_surface_surface( Handle<3> a,Handle<3> b )
{
    CGAL::Object inter = intersection( a.asSurface(), b.asSurface() );
    const Point_3* p = CGAL::object_cast< Point_3 >( &inter );
    const Segment_3* s = CGAL::object_cast< Segment_3 >( &inter );
    const Triangle_3* t = CGAL::object_cast< Triangle_3 >( &inter );
    const std::vector<Point_3>* v = CGAL::object_cast< std::vector<Point_3> >( &inter );

    if ( p ) {
        a.asSurface().splitAt( *p );
        b.asSurface().splitAt( *p );
    }
    else if ( s ) {
        a.asSurface().splitAt( *s );
        b.asSurface().splitAt( *s );
    }
    else if ( t ) {
        a.asSurface().splitAt( *t );
        b.asSurface().remove( *t );
    }
    else if ( v ) {
        a.asSurface().splitAt( *v );
        b.asSurface().remove( *v );
    }
}

void union_surface_volume( Handle<2> ,Handle<2> )
{
    BOOST_ASSERT( false ); // there shouldn't be any volume in 2D
}

void union_surface_volume( Handle<3> a,Handle<3> b )
{
    detail::GeometrySet<3> res;
    _intersection_solid_triangle( b.asVolume(), a.asSurface(), res );

    for ( detail::GeometrySet<3>::SurfaceCollection::const_iterator it = res.surfaces().begin();
            it != res.surfaces().end(); ++it ) {
        a.asSurface().remove( it->primitive() );
    }
}

void union_volume_volume( Handle<2> ,Handle<2> )
{
    BOOST_ASSERT( false ); // there shouldn't be any volume in 2D
}

void union_volume_volume( Handle<3> a,Handle<3> b )
{
    MarkedPolyhedron& p = const_cast<MarkedPolyhedron&>( a.asVolume() );
    MarkedPolyhedron& q = const_cast<MarkedPolyhedron&>( b.asVolume() );
    typedef CGAL::Polyhedron_corefinement<MarkedPolyhedron> Corefinement;
    Corefinement coref;
    CGAL::Emptyset_iterator no_polylines;

    // volumes must at least share a face, if they share only a point, this will cause
    // an invalid geometry, if they only share an egde it will cause the CGAL algo to
    // throw
    //
    // for the moment we use hte intersection algo and test the result
    detail::GeometrySet<3> inter;
    intersection( detail::GeometrySet<3>( a.asVolume() ), detail::GeometrySet<3>( b.asVolume() ), inter );

    if ( inter.volumes().size() || inter.surfaces().size() ) {
        typedef std::vector<std::pair<MarkedPolyhedron*, int> >  ResultType;
        ResultType result;
        coref( p, q, no_polylines, std::back_inserter( result ), Corefinement::Join_tag );

        if ( result.size() == 1 ) {
            Handle<3> h( *result[0].first );
            // @todo check that the volume is valid (connection on one point isn't)
            h.registerObservers( a );
            h.registerObservers( b );
        }

        for ( ResultType::iterator it = result.begin(); it != result.end(); it++ ) {
            delete it->first;
        }
    }

}



template <int Dim>
struct UnionOnBoxCollision {
    void operator()( typename HandledBox<Dim>::Type& a,
                     typename HandledBox<Dim>::Type& b ) {
        DEBUG_OUT << "collision of boxes\n";

        switch ( a.handle().which() ) {
        case PrimitivePoint:
            switch ( b.handle().which() ) {
            case PrimitivePoint:
                union_point_point( a.handle(), b.handle() );
                break;

            case PrimitiveSegment:
                union_point_segment( a.handle(), b.handle() );
                break;

            case PrimitiveSurface:
                union_point_surface( a.handle(), b.handle() );
                break;

            case PrimitiveVolume:
                union_point_volume( a.handle(), b.handle() );
                break;

            case PrimitiveEmpty:
                break;
            }

            break;

        case PrimitiveSegment:
            switch ( b.handle().which() ) {
            case PrimitivePoint:
                union_point_segment( b.handle(), a.handle() );
                break;

            case PrimitiveSegment:
                union_segment_segment( a.handle(), b.handle() );
                break;

            case PrimitiveSurface:
                union_segment_surface( a.handle(), b.handle() );
                break;

            case PrimitiveVolume:
                union_segment_volume( a.handle(), b.handle() );
                break;

            case PrimitiveEmpty:
                break;
            }

            break;

        case PrimitiveSurface:
            switch ( b.handle().which() ) {
            case PrimitivePoint:
                union_point_surface( b.handle(), a.handle() );
                break;

            case PrimitiveSegment:
                union_segment_surface( b.handle(), a.handle() );
                break;

            case PrimitiveSurface:
                union_surface_surface( a.handle(), b.handle() );
                break;

            case PrimitiveVolume:
                union_surface_volume( a.handle(), b.handle() );
                break;

            case PrimitiveEmpty:
                break;
            }

            break;

        case PrimitiveVolume:
            switch ( b.handle().which() ) {
            case PrimitivePoint:
                union_point_volume( b.handle(), a.handle() );
                break;

            case PrimitiveSegment:
                union_segment_volume( b.handle(), a.handle() );
                break;

            case PrimitiveSurface:
                union_surface_volume( b.handle(), a.handle() );
                break;

            case PrimitiveVolume:
                union_volume_volume( a.handle(), b.handle() );
                break;

            case PrimitiveEmpty:
                break;
            }

            break;

        case PrimitiveEmpty:
            break;
        }
    }
};

template <int Dim>
void collectPrimitives( const typename HandledBox<Dim>::Vector& boxes, detail::GeometrySet<Dim>& output )
{
    Handle<Dim> empty;

    for ( typename  HandledBox<Dim>::Vector::const_iterator bit = boxes.begin();
            bit != boxes.end(); ++bit ) {
        switch ( bit->handle().which() ) {
        case PrimitivePoint :
            output.addPrimitive( bit->handle().asPoint() );
            empty.registerObservers( bit->handle() );
            break;

        case PrimitiveSegment : {
            typename std::vector< typename detail::Segment_d<Dim>::Type > pieces( bit->handle().asSegment().pieces() );
            output.addSegments( pieces.begin(), pieces.end() );
            empty.registerObservers( bit->handle() );
        }
        break;

        case PrimitiveSurface : {
            typename std::vector< typename detail::Surface_d<Dim>::Type > pieces( bit->handle().asSurface().pieces() );
            output.addSurfaces( pieces.begin(), pieces.end() );
            empty.registerObservers( bit->handle() );
        }
        break;

        case PrimitiveVolume :
            output.addPrimitive( bit->handle().asVolume() );
            empty.registerObservers( bit->handle() );
            break;

        case PrimitiveEmpty :
            break;
        }
    }
}

std::auto_ptr<Geometry> union_( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    HandledBox<2>::Vector boxes;
    compute_bboxes( detail::GeometrySet<2>( ga ), std::back_inserter( boxes ) );
    const unsigned numBoxA = boxes.size();
    compute_bboxes( detail::GeometrySet<2>( gb ), std::back_inserter( boxes ) );

    CGAL::box_intersection_d( boxes.begin(), boxes.begin() + numBoxA,
                              boxes.begin() + numBoxA, boxes.end(),
                              UnionOnBoxCollision<2>() );

    detail::GeometrySet<2> output;
    collectPrimitives( boxes, output );
    return output.recompose();
}

std::auto_ptr<Geometry> union_( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( gb );
    std::auto_ptr<Geometry>  result( union_( ga, gb, NoValidityCheck() ) );
    return result;
}

std::auto_ptr<Geometry> union3D( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    HandledBox<3>::Vector boxes;
    compute_bboxes( detail::GeometrySet<3>( ga ), std::back_inserter( boxes ) );
    const unsigned numBoxA = boxes.size();
    compute_bboxes( detail::GeometrySet<3>( gb ), std::back_inserter( boxes ) );

    CGAL::box_intersection_d( boxes.begin(), boxes.begin() + numBoxA,
                              boxes.begin() + numBoxA, boxes.end(),
                              UnionOnBoxCollision<3>() );

    detail::GeometrySet<3> output;
    collectPrimitives( boxes, output );
    return output.recompose();
}


std::auto_ptr<Geometry> union3D( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( gb );
    std::auto_ptr<Geometry>  result( union3D( ga, gb, NoValidityCheck() ) );
    return result;
}

void handleLeakTest()
{
    Handle<2> h0( Point_2( 0,0 ) );
    Handle<2> h1( Point_2( 1,1 ) );
    Handle<2> empty;
    empty.registerObservers( empty );
    empty.registerObservers( h0 );
    h1.registerObservers( h0 );
}
}
}
