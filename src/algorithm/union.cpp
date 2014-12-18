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
#include <SFCGAL/algorithm/union.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/triangulate/triangulate2DZ.h>

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
struct Segment_d: detail::Segment_d<Dim>::Type
{
    typedef typename detail::Point_d<Dim>::Type PointType;
    typedef typename detail::Segment_d<Dim>::Type SegmentType;
    typedef typename std::vector< PointType > PointVector;
    typedef typename std::vector< SegmentType > SegmentVector;

    Segment_d( const SegmentType & s ): SegmentType(s){}
    void splitAt( const PointType & p ) { _split.push_back( p ); }
    void remove( const SegmentType & s ) 
    { 
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
            if ( *p != *q ){
                PointType m = CGAL::midpoint( *p, *q );
                typename SegmentVector::const_iterator r = _remove.begin();
                for ( ; r != _remove.end() && !r->has_on( m ); ++r );
                if ( r == _remove.end() ){
                    *out++ = SegmentType( *p, *q );
                }
            }
        }
        return out;
    }

    SegmentVector pieces() const {
        SegmentVector result;
        this->pieces( std::back_inserter(result) );
        return result;
    }

private:
    PointVector _split;
    SegmentVector _remove;
};

template <int Dim> struct Surface_d {};

template <>
struct Surface_d<3>: Triangle_3
{
    typedef std::vector< algorithm::Point_2 > PointVector;
    typedef std::vector< Segment_2 > SegmentVector;
    typedef std::vector< PointVector > SurfaceVector;

    Surface_d( const Triangle_3 & s ): Triangle_3( s ), _plane( s.supporting_plane() ) 
    {
        this->splitAt( s );
    }

    void splitAt( const Segment_3 & s)
    { 
        _split.push_back(Segment_2( _plane.to_2d(s.source()), _plane.to_2d(s.target()) ));
    }

    template <typename Point3Iterator>
    void splitAt( Point3Iterator begin, Point3Iterator end ) // polygon with unclosed ring
    {
        if (begin == end) return;
        Point3Iterator s = begin, t = s+1;
        for ( ; t != end; ++t, ++s ){
            _split.push_back( Segment_2( _plane.to_2d(*s), _plane.to_2d(*t) ) );
        }
        _split.push_back( Segment_2( _plane.to_2d(*s), _plane.to_2d(*begin) ) );
    }

    void splitAt( const Triangle_3 & t)
    { 
        const algorithm::Point_3 v[3] = { t.vertex( 0 ), t.vertex( 1 ), t.vertex( 2 ) };
        this->splitAt( v, v+3 );
    }

    void splitAt( const std::vector< algorithm::Point_3 > & p ) // polygon with unclosed ring
    {
        this->splitAt( p.begin(), p.end() );
    }

    template <typename Point3Iterator> // polygon with unclosed ring
    void remove( Point3Iterator begin, Point3Iterator end )
    {
        if ( begin == end ) return;
        this->splitAt( begin, end );
        PointVector v;
        for ( Point3Iterator i = begin; i != end; ++i ) v.push_back( _plane.to_2d( *i ) );
        _remove.push_back( v ); 
    }

    void remove( const std::vector< algorithm::Point_3 > & p )
    {
        this->remove( p.begin(), p.end() );
    }

    void remove( const Triangle_3 & t )
    {
        const algorithm::Point_3 v[3] = { t.vertex( 0 ), t.vertex( 1 ), t.vertex( 2 ) };
        this->remove( v, v+3 );
    }

    std::vector< Triangle_3 > pieces()
    {
        // we need to process the split lines because there may be several lines at the same place
        // and this won't play nice with the triangulation, the same stands for the lines lying
        // on the triangle edges
        // 
        // after that we just check, for each triangle, if a point fall in a removed part and remove it
        // we can do that by pairwise union of all segments
        std::vector< Segment_2 > filtered;
        {
            std::vector< Segment_d<2> > lines( _split.begin(), _split.begin()+3 );
            for (typename SegmentVector::const_iterator c = _split.begin()+3; c != _split.end(); ++c){
                Segment_d<2> current( *c );
                for (std::vector< Segment_d<2> >::iterator l = lines.begin(); l != lines.end(); ++l){
                    CGAL::Object inter = CGAL::intersection( *l, current );
                    const Point_2 * p = CGAL::object_cast< Point_2 >( &inter );
                    const Segment_2 * s = CGAL::object_cast< Segment_2 >( &inter );
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
            for (std::vector< Segment_d<2> >::const_iterator l = lines.begin(); l != lines.end(); ++l){
                l->pieces( std::back_inserter( filtered ) );
            }
        }

        DEBUG_OUT << "triangulating " << filtered.size() << " lines\n";

        // now we want to triangulate
        TriangulatedSurface ts;
        {
            typedef triangulate::ConstraintDelaunayTriangulation::Vertex_handle Vertex_handle;
            triangulate::ConstraintDelaunayTriangulation cdt;
            for (std::vector< Segment_2 >::const_iterator f = filtered.begin(); f != filtered.end(); ++f){
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
            const Point_2 a( t->vertex( 0 ).toPoint_2() );
            const Point_2 b( t->vertex( 1 ).toPoint_2() );
            const Point_2 c( t->vertex( 2 ).toPoint_2() );
            const Point_2 point( a + ( Vector_2( a, b ) + Vector_2( a, c ) )/3 );

            // find if triangle is in a removed spot
            SurfaceVector::const_iterator r = _remove.begin();
            for ( ; r != _remove.end() 
                    && CGAL::bounded_side_2( r->begin(), r->end(), point, Kernel() ) 
                    == CGAL::ON_UNBOUNDED_SIDE; ++r );

            if ( r == _remove.end() ){ 
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
struct Surface_d<2>: PolygonWH_2
{
    typedef std::vector< Point_2 > PointVector;
    typedef std::vector< Segment_2 > SegmentVector;
    typedef std::vector< PointVector > SurfaceVector;

    Surface_d( const PolygonWH_2 & s ): PolygonWH_2( s ){}

    void splitAt( const Segment_2 & s){ _split.push_back(s); }

    std::vector< PolygonWH_2 > pieces() const
    { 
        std::vector< PolygonWH_2 > res;
        fix_cgal_valid_polygon( *this, std::back_inserter( res ) );
        return res;
    }

private:
    SegmentVector _split;
};



// takes care of RAII of primitives
template <int Dim>
struct ObservablePrimitive: boost::variant< 
        typename detail::Point_d<Dim>::Type,
        Segment_d<Dim>,
        Surface_d<Dim>,
        typename detail::Volume_d<Dim>::Type,
        EmptyPrimitive >
{
    template <class T>
    ObservablePrimitive( const T & p ) :  boost::variant< 
        typename detail::Point_d<Dim>::Type,
        Segment_d<Dim>,
        Surface_d<Dim>,
        typename detail::Volume_d<Dim>::Type,
        EmptyPrimitive >( p ) {}


    struct Handle: boost::shared_ptr< boost::shared_ptr< ObservablePrimitive > >
    {
        Handle( ): boost::shared_ptr< boost::shared_ptr< ObservablePrimitive > >(new boost::shared_ptr< ObservablePrimitive >( new ObservablePrimitive<Dim>( EmptyPrimitive() ) ) ) {}

        Handle( ObservablePrimitive * p ): boost::shared_ptr< boost::shared_ptr< ObservablePrimitive > >( new boost::shared_ptr< ObservablePrimitive >( p ) )
        {
            p->_observers.push_back( *this );
        }

        typename detail::Point_d<Dim>::Type & asPoint()
        {
            BOOST_ASSERT(this->which() == PrimitivePoint);
            return boost::get<typename detail::Point_d<Dim>::Type& >( *(this->get()->get()) );
        }

        Segment_d<Dim> & asSegment() 
        {
            BOOST_ASSERT(this->which() == PrimitiveSegment);
            return boost::get<Segment_d<Dim> & >( *(this->get()->get()) );
        }

        Surface_d<Dim> & asSurface() 
        {
            BOOST_ASSERT(this->which() == PrimitiveSurface);
            return boost::get<Surface_d<Dim> & >( *(this->get()->get()) );
        }

        typename detail::Volume_d<Dim>::Type & asVolume() 
        {
            BOOST_ASSERT(this->which() == PrimitiveVolume);
            return boost::get<typename detail::Volume_d<Dim>::Type& >( *(this->get()->get()) );
        }

        PrimitiveType which() { return PrimitiveType(this->get()->get()->which()); }

        bool empty() { return which() == PrimitiveEmpty;}

        void registerObservers( Handle a ) 
        { 
            this->get()->get()->_observers.insert(this->get()->get()->_observers.end(), a->get()->_observers.begin(), a->get()->_observers.end() ); 
            for ( typename HandleList::iterator h=this->get()->get()->_observers.begin(); h!=this->get()->get()->_observers.end(); ++h ){ 
                *(h->get()) = *(this->get());
            }
        };
    };

    typedef std::list< Handle > HandleList;


    template <class T>
    inline T& as() 
    {
        return boost::get<T&>( *this );
    }

private:
    HandleList _observers;

    ObservablePrimitive( const ObservablePrimitive &); // no cpy ctor
    ObservablePrimitive & operator=( const ObservablePrimitive & );
};

template <int Dim>
struct HandledBox {
    typedef CGAL::Box_intersection_d::Box_with_handle_d<double, Dim, typename ObservablePrimitive<Dim>::Handle > Type;
    typedef std::vector< Type > Vector;
};

template <int Dim>
typename HandledBox<Dim>::Vector compute_bboxes( const detail::GeometrySet<Dim>& gs )
{
    typename HandledBox<Dim>::Vector bboxes;

    for ( typename detail::GeometrySet<Dim>::PointCollection::const_iterator it = gs.points().begin();
            it != gs.points().end(); ++it ) {
        typename ObservablePrimitive<Dim>::Handle p( new ObservablePrimitive<Dim>( it->primitive() ) );
        bboxes.push_back( typename HandledBox<Dim>::Type(it->primitive().bbox(), p) );
    }

    for ( typename detail::GeometrySet<Dim>::SegmentCollection::const_iterator it = gs.segments().begin();
            it != gs.segments().end(); ++it ) {
        typename ObservablePrimitive<Dim>::Handle p( new ObservablePrimitive<Dim>( it->primitive() ) );
        bboxes.push_back( typename HandledBox<Dim>::Type(it->primitive().bbox(), p) );
    }

    for ( typename detail::GeometrySet<Dim>::SurfaceCollection::const_iterator it = gs.surfaces().begin();
            it != gs.surfaces().end(); ++it ) {
        typename ObservablePrimitive<Dim>::Handle p( new ObservablePrimitive<Dim>( it->primitive() ) );
        bboxes.push_back( typename HandledBox<Dim>::Type(it->primitive().bbox(), p) );
    }

    for ( typename detail::GeometrySet<Dim>::VolumeCollection::const_iterator it = gs.volumes().begin();
            it != gs.volumes().end(); ++it ) {
        typename ObservablePrimitive<Dim>::Handle p( new ObservablePrimitive<Dim>( it->primitive() ) );
        bboxes.push_back( typename HandledBox<Dim>::Type( compute_solid_bbox( it->primitive(), detail::dim_t<Dim>() ), p) );
    }
    return bboxes;
}

template <class Handle>
void union_point_point(Handle a, Handle b )
{
    if ( a.asPoint() == b.asPoint() ){
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

void union_point_surface(typename ObservablePrimitive<2>::Handle a,typename ObservablePrimitive<2>::Handle b )
{
    if ( do_intersect( a.asPoint(), b.asSurface() ) ) {
        b.registerObservers( a );
    }
}

void union_point_surface(typename ObservablePrimitive<3>::Handle a,typename ObservablePrimitive<3>::Handle b )
{
    if ( b.asSurface().has_on( a.asPoint() ) ) {
        b.registerObservers( a );
    }
}

void union_point_volume(typename ObservablePrimitive<2>::Handle ,typename ObservablePrimitive<2>::Handle  )
{
    BOOST_ASSERT(false); // there shouldn't be any volume in 2D
}

void union_point_volume(typename ObservablePrimitive<3>::Handle a,typename ObservablePrimitive<3>::Handle b )
{
    //@todo put is in poly in a struct derived from MarkedPolyhedron to avoid rebuilding every time
    CGAL::Point_inside_polyhedron_3<MarkedPolyhedron, Kernel> is_in_poly( b.asVolume() );
    if ( CGAL::ON_UNBOUNDED_SIDE != is_in_poly( a.asPoint() ) ) {
        b.registerObservers( a );
    }
}



template <int Dim>
void union_segment_segment(typename ObservablePrimitive<Dim>::Handle a,typename ObservablePrimitive<Dim>::Handle b )
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

void union_segment_segment(typename ObservablePrimitive<2>::Handle a,typename ObservablePrimitive<2>::Handle b )
{
    union_segment_segment<2>( a, b );
}

void union_segment_segment(typename ObservablePrimitive<3>::Handle a,typename ObservablePrimitive<3>::Handle b )
{
    union_segment_segment<3>( a, b );
}

void union_segment_surface(typename ObservablePrimitive<2>::Handle a,typename ObservablePrimitive<2>::Handle b )
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
            const Point_2 * p = CGAL::object_cast< Point_2 >( &inter );
            const Segment_2 * s = CGAL::object_cast< Segment_2 >( &inter );
            if ( p ) {
                points.push_back( *p );
            }
            else if ( s ){
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

void union_segment_surface(typename ObservablePrimitive<3>::Handle a,typename ObservablePrimitive<3>::Handle b )
{
    CGAL::Object inter = CGAL::intersection( a.asSegment(), b.asSurface() );
    const Segment_3 * s = CGAL::object_cast< Segment_3 >( &inter );
    if ( s ){
        a.asSegment().remove( *s );
        b.asSurface().splitAt( *s );
    }
}

void union_segment_volume(typename ObservablePrimitive<2>::Handle ,typename ObservablePrimitive<2>::Handle )
{
    BOOST_ASSERT(false); // there shouldn't be any volume in 2D
}

void union_segment_volume(typename ObservablePrimitive<3>::Handle a,typename ObservablePrimitive<3>::Handle b )
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

    CGAL::Point_inside_polyhedron_3<MarkedPolyhedron, Kernel> is_in_poly( polyhedron );
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
            typename ObservablePrimitive<3>::Handle h( new ObservablePrimitive<3>( *tri ) );
            union_segment_surface( a, h );
        }

        // second step, for each segment, add intersection points and test each middle point
        // to know if it's in or out
        std::vector< Point_3 > points;
        for ( std::vector< Triangle_3 >::const_iterator tri=triangles.begin();
                tri != triangles.end(); ++tri ) {
            CGAL::Object inter = CGAL::intersection( segment, *tri );
            const Point_3* p = CGAL::object_cast< Point_3 >( &inter );
            if ( p ) points.push_back( *p );
        }
        if ( points.size() ){
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


void union_surface_surface(typename ObservablePrimitive<2>::Handle a,typename ObservablePrimitive<2>::Handle b )
{
    CGAL::Gps_segment_traits_2<Kernel> traits;
    PolygonWH_2 res;
    if ( CGAL::join(
                are_holes_and_boundary_pairwise_disjoint( a.asSurface(), traits ) ? a.asSurface() : fix_sfs_valid_polygon( a.asSurface() ),
                are_holes_and_boundary_pairwise_disjoint( b.asSurface(), traits ) ? b.asSurface() : fix_sfs_valid_polygon( b.asSurface() ),
                res ) ) {
        DEBUG_OUT << "merged " << a.asSurface() << " and " << b.asSurface() << "\n";
        typename ObservablePrimitive<2>::Handle h( new ObservablePrimitive<2>( res ) );
        h.registerObservers( a );
        h.registerObservers( b );
    }
}

void union_surface_surface(typename ObservablePrimitive<3>::Handle a,typename ObservablePrimitive<3>::Handle b )
{
    CGAL::Object inter = intersection( a.asSurface(), b.asSurface() );
    const Segment_3 * s = CGAL::object_cast< Segment_3 >( &inter );
    const Triangle_3 * t = CGAL::object_cast< Triangle_3 >( &inter );
    const std::vector<Point_3> * p = CGAL::object_cast< std::vector<Point_3> >( &inter );
    if ( s ){
        a.asSurface().splitAt( *s );
        b.asSurface().splitAt( *s );
    }
    else if ( t ){
        a.asSurface().splitAt( *t );
        b.asSurface().remove( *t );
    }
    else if ( p ){
        a.asSurface().splitAt( *p );
        b.asSurface().remove( *p );
    }
}

void union_surface_volume(typename ObservablePrimitive<2>::Handle ,typename ObservablePrimitive<2>::Handle )
{
    BOOST_ASSERT(false); // there shouldn't be any volume in 2D
}

void union_surface_volume(typename ObservablePrimitive<3>::Handle a,typename ObservablePrimitive<3>::Handle b )
{
    detail::GeometrySet<3> res;
    _intersection_solid_triangle( b.asVolume(), a.asSurface(), res );

    for ( detail::GeometrySet<3>::SurfaceCollection::const_iterator it = res.surfaces().begin();
            it != res.surfaces().end(); ++it ) {
        a.asSurface().remove( it->primitive() );
    }
}

void union_volume_volume(typename ObservablePrimitive<2>::Handle ,typename ObservablePrimitive<2>::Handle )
{
    BOOST_ASSERT(false); // there shouldn't be any volume in 2D
}

void union_volume_volume(typename ObservablePrimitive<3>::Handle a,typename ObservablePrimitive<3>::Handle b )
{
    MarkedPolyhedron& p = const_cast<MarkedPolyhedron&>( a.asVolume() );
    MarkedPolyhedron& q = const_cast<MarkedPolyhedron&>( b.asVolume() );
    typedef CGAL::Polyhedron_corefinement<MarkedPolyhedron> Corefinement;
    Corefinement coref;
    CGAL::Emptyset_iterator no_polylines;
    typedef std::vector<std::pair<MarkedPolyhedron*, int> >  ResultType;
    ResultType result;
    try {
        coref( p, q, no_polylines, std::back_inserter( result ), Corefinement::Join_tag );
        if ( result.size() == 1){
            typename ObservablePrimitive<3>::Handle h( new ObservablePrimitive<3>( *result[0].first ) );
            // @todo check that the volume is valid (connection on one point isn't)
            h.registerObservers( a );
            h.registerObservers( b );

            for ( ResultType::iterator it = result.begin(); it != result.end(); it++ ) {
                delete it->first;
            }
        }
    }
    catch (std::logic_error){
        // will happen if they only share an edge
        std::cerr << "SFCGAL NOTICE: the previous CGAL error is due to a shared edge, it is not a problem\n";
    }
}



template <int Dim>
struct UnionOnBoxCollision {
    void operator()( typename HandledBox<Dim>::Type& a,
                     typename HandledBox<Dim>::Type& b ) {
        switch ( a.handle().which() ) {
        case PrimitivePoint:
            switch ( b.handle().which() ) {
            case PrimitivePoint: union_point_point(a.handle(), b.handle() ); break;
            case PrimitiveSegment: union_point_segment(a.handle(), b.handle() ); break;
            case PrimitiveSurface: union_point_surface(a.handle(), b.handle() ); break;
            case PrimitiveVolume: union_point_volume(a.handle(), b.handle() ); break;
            case PrimitiveEmpty: break;
            }
            break;
        case PrimitiveSegment:
            switch ( b.handle().which() ) {
            case PrimitivePoint: union_point_segment(b.handle(), a.handle() ); break;
            case PrimitiveSegment: union_segment_segment(a.handle(), b.handle() ); break;
            case PrimitiveSurface: union_segment_surface(a.handle(), b.handle()); break;
            case PrimitiveVolume:  union_segment_volume(a.handle(), b.handle()); break;
            case PrimitiveEmpty: break;
            }
            break;

        case PrimitiveSurface:
            switch ( b.handle().which() ) {
            case PrimitivePoint: union_point_surface(b.handle(), a.handle() ); break;
            case PrimitiveSegment: union_segment_surface(b.handle(), a.handle()); break;
            case PrimitiveSurface: union_surface_surface(a.handle(), b.handle()); break;
            case PrimitiveVolume:  union_surface_volume(a.handle(), b.handle()); break;
            case PrimitiveEmpty: break;
            }
            break;

        case PrimitiveVolume:
            switch ( b.handle().which() ) {
            case PrimitivePoint: union_point_volume(b.handle(), a.handle() ); break;
            case PrimitiveSegment: union_segment_volume(b.handle(), a.handle()); break;
            case PrimitiveSurface: union_surface_volume(b.handle(), a.handle()); break;
            case PrimitiveVolume: union_volume_volume(a.handle(), b.handle()); break;
            case PrimitiveEmpty: break;
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
    typename ObservablePrimitive<Dim>::Handle empty;
    for ( typename  HandledBox<Dim>::Vector::const_iterator bit = boxes.begin();
            bit != boxes.end(); ++bit ) {
        switch ( (*bit->handle())->which() ) {
        case PrimitivePoint : 
            output.addPrimitive( bit->handle().asPoint() );
            empty.registerObservers( bit->handle() );
            break;

        case PrimitiveSegment :
            {
            typename std::vector< typename detail::Segment_d<Dim>::Type > pieces( bit->handle().asSegment().pieces() );
            output.addSegments( pieces.begin(), pieces.end() );
            empty.registerObservers( bit->handle() );
            }
            break;

        case PrimitiveSurface :
            {
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
    detail::GeometrySet<2> gsa( ga ), gsb( gb ), output;
    typename HandledBox<2>::Vector aboxes( compute_bboxes( gsa ) );
    typename HandledBox<2>::Vector bboxes( compute_bboxes( gsb ) );

    typename HandledBox<2>::Vector boxes( aboxes );
    boxes.insert( boxes.end(), bboxes.begin(), bboxes.end() );

    CGAL::box_intersection_d( boxes.begin(), boxes.begin() + aboxes.size(),
                              boxes.begin() + aboxes.size(), boxes.end(),
                              UnionOnBoxCollision<2>() );

    collectPrimitives( boxes, output );

    //algorithm::union_( gsa, gsb, output );
    return output.recompose();
}

std::auto_ptr<Geometry> union_( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( gb );
    return union_( ga, gb, NoValidityCheck() );
}

std::auto_ptr<Geometry> union3D( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{

    detail::GeometrySet<3> gsa( ga ), gsb( gb ), output;
    typename HandledBox<3>::Vector aboxes( compute_bboxes( gsa ) );
    typename HandledBox<3>::Vector bboxes( compute_bboxes( gsb ) );

    typename HandledBox<3>::Vector boxes( aboxes );
    boxes.insert( boxes.end(), bboxes.begin(), bboxes.end() );

    CGAL::box_intersection_d( boxes.begin(), boxes.begin() + aboxes.size(),
                              boxes.begin() + aboxes.size(), boxes.end(),
                              UnionOnBoxCollision<3>() );

    collectPrimitives( boxes, output );

    //algorithm::union_( gsa, gsb, output );
    return output.recompose();
}


std::auto_ptr<Geometry> union3D( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( gb );
    return union3D( ga, gb, NoValidityCheck() );
}
}
}
