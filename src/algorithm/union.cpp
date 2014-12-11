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
#include <SFCGAL/detail/algorithm/needsUnion.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/triangulate/triangulate2DZ.h>

#include <cstdio>

//
// Union kernel


// @todo replace de change adress by some kind of visitor on primitives that will update
// all ptrs pointing to the same thing without the need for going through all lists
// to update adresses
#define DEBUG_OUT if (1) std::cerr << __FILE__ << ":" << __LINE__ << " debug: "

using namespace SFCGAL::detail;

namespace SFCGAL {

namespace algorithm {
// we don't want to merge triangles an retriangulate because it's conceptually
// a pain to have a union of two yielding more than one primitive
// either two primitives merge into one, or they don't merge
struct PolygonWH_3
{
    PolygonWH_3( const Triangle_3 & t)
        : _plane(t.supporting_plane())
        , _poly( projection( t, _plane) )
    {}

    PolygonWH_3( const PolygonWH_2 & poly, const Plane_3 & plane)
        : _plane(plane)
        , _poly(poly)
    {}

    PolygonWH_3( const PolygonWH_3 & poly, const Plane_3 & plane) // put in another plane
        : _plane(plane)
        , _poly( reprojection(poly, plane) )
    {}

    static PolygonWH_2 reprojection( const PolygonWH_3 & p, const Plane_3 & toPlane ){
        Polygon_2 outer;
        for (Polygon_2::Vertex_const_iterator v = p.poly().outer_boundary().vertices_begin(); 
                v!=p.poly().outer_boundary().vertices_end(); ++v){
            outer.push_back( toPlane.to_2d( p.plane().to_3d( *v ) ) );
        }

        std::list< Polygon_2 > holes;
        for ( PolygonWH_2::Hole_const_iterator hit = p.poly().holes_begin(); 
                hit != p.poly().holes_end(); ++hit ){
            holes.push_back( Polygon_2() );
            for ( Polygon_2::Vertex_const_iterator v = hit->vertices_begin(); v != hit->vertices_end(); ++v){
                holes.back().push_back( toPlane.to_2d( p.plane().to_3d( *v ) ) );
            }
        }
        
        return PolygonWH_2( outer, holes.begin(), holes.end() );
    }

    static PolygonWH_2 projection( const Triangle_3 & t, const Plane_3 & plane ){
        Polygon_2 p;
        for (unsigned i=0; i<3; i++) p.push_back( plane.to_2d( t.vertex(i) ) );
        return PolygonWH_2(p);
    }

    // default cpy and = are ok

    const Plane_3 & plane() const {return _plane;}
    const PolygonWH_2 & poly() const {return _poly;}
private:
    Plane_3 _plane;
    PolygonWH_2 _poly;
};

struct Segment_2X : Segment_2
{
    Segment_2X( const Segment_2 & s ): Segment_2( s ) {}
    Segment_2X( const CGAL::Point_2<Kernel> & s, const CGAL::Point_2<Kernel> & t): Segment_2(s, t) {}
    // defaut cpy an = are OK
    std::vector< CGAL::Point_2<Kernel> > cuts;
};

PrimitiveType typeFor( const Point_2& ){return PrimitivePoint;}
PrimitiveType typeFor( const Point_3& ){return PrimitivePoint;}
PrimitiveType typeFor( const Segment_2X& ){return PrimitiveSegment;}
PrimitiveType typeFor( const Segment_3& ){return PrimitiveSegment;}
PrimitiveType typeFor( const PolygonWH_2& ){return PrimitiveSurface;}
PrimitiveType typeFor( const PolygonWH_3& ){return PrimitiveSurface;}
PrimitiveType typeFor( const NoVolume& ){return PrimitiveVolume;}
PrimitiveType typeFor( const MarkedPolyhedron& ){return PrimitiveVolume;}

template <int Dim> struct PointPtr_d {};
template <> struct PointPtr_d<2> { typedef boost::shared_ptr< Point_2 > Type; };
template <> struct PointPtr_d<3> { typedef boost::shared_ptr< Point_3 > Type; };
template <int Dim> struct SegmentPtr_d {};
template <> struct SegmentPtr_d<2> { typedef boost::shared_ptr< Segment_2 > Type; };
template <> struct SegmentPtr_d<3> { typedef boost::shared_ptr< Segment_3 > Type; };
template <int Dim> struct SurfacePtr_d {};
template <> struct SurfacePtr_d<2> { typedef boost::shared_ptr< PolygonWH_2 > Type; };
template <> struct SurfacePtr_d<3> { typedef boost::shared_ptr< PolygonWH_3 > Type; };
template <int Dim> struct VolumePtr_d {};
template <> struct VolumePtr_d<2> { typedef boost::shared_ptr< NoVolume > Type; };
template <> struct VolumePtr_d<3> { typedef boost::shared_ptr< MarkedPolyhedron > Type; };


template <int Dim>
struct PrimitiveVec: boost::variant<
        std::vector<typename PointPtr_d<Dim>::Type >,
        std::vector<typename SegmentPtr_d<Dim>::Type >,
        std::vector<typename SurfacePtr_d<Dim>::Type >,
        std::vector<typename VolumePtr_d<Dim>::Type > > {
    typedef std::vector< typename PointPtr_d<Dim>::Type > PointPtrVec;
    typedef std::vector< typename SegmentPtr_d<Dim>::Type > SegmentPtrVec;
    typedef std::vector< typename SurfacePtr_d<Dim>::Type > SurfacePtrVec;
    typedef std::vector< typename VolumePtr_d<Dim>::Type > VolumePtrVec;

    template <class T>
    PrimitiveVec( const T& v )
        : boost::variant< PointPtrVec, SegmentPtrVec, SurfacePtrVec, VolumePtrVec >( v ) {
    }

    template <typename T>
    T& as() {
        return boost::get< T >( *this );
    }

    template <typename T>
    const T& as() const {
        return boost::get< T >( *this );
    }

    template <typename SharedPrimPtr>
    void changeAdress(SharedPrimPtr oldAdress, SharedPrimPtr & newAdress) {
        if ( this->which() != typeFor(*oldAdress) ) return;
        typedef typename std::vector< SharedPrimPtr > VecType;
        VecType & v = boost::get< VecType >( *this );
        bool replaced = false;
        for (typename VecType::iterator i = v.begin(); i != v.end(); ++i){
            if ( i->get() == oldAdress.get() ){ 
                if (!replaced){
                    *i = newAdress;
                    replaced = true;
                }
                else{
                    while (i != v.end() && i->get() == oldAdress.get()){
                        i = v.erase( i ); // avoid duplicated primitive in the same box
                        if ( i == v.end() ) return; 
                    }
                }
            }
        }
    }

    template <typename SharedPrimPtr>
    void removeAdress(SharedPrimPtr oldAdress) {
        if ( this->which() != typeFor(*oldAdress) ) return;
        typedef typename std::vector< SharedPrimPtr > VecType;
        VecType & v = boost::get< VecType >( *this );
        for (typename VecType::iterator i = v.begin(); i != v.end(); ++i){
            while ( i->get() == oldAdress.get() ){
                i = v.erase( i );
                if ( i == v.end() ) return; 
            }
        }
    }
};

// this one is a tad complicated because we want bboxes with handle to be responsible for the
// primitives, hence the use of shared ptr instead of a dumb ptr
template <int Dim>
struct HandledBox {
    typedef typename CGAL::Box_intersection_d::Box_with_handle_d<double, Dim, boost::shared_ptr< PrimitiveVec<Dim> > > Type;
    typedef typename std::vector< Type > Vector;
};

template < typename SharedPrimPtr, class HandleBoxVectorType >
void changeAdress( SharedPrimPtr oldAdress, SharedPrimPtr & newAdress, HandleBoxVectorType & boxes )
{
    for (typename HandleBoxVectorType::iterator it=boxes.begin(); it!=boxes.end(); ++it){
        it->handle()->changeAdress( oldAdress, newAdress );
    }
}

template < typename SharedPrimPtr, class HandleBoxVectorType >
void removeAdress( SharedPrimPtr oldAdress, HandleBoxVectorType & boxes )
{
    for (typename HandleBoxVectorType::iterator it=boxes.begin(); it!=boxes.end(); ++it){
        it->handle()->removeAdress( oldAdress );
    }
}


template < typename OutputIteratorType >
OutputIteratorType difference( const Point_3& p, const PolygonWH_3& q, OutputIteratorType out )
{
    if ( ! q.plane().has_on( p ) || !do_intersect( q.plane().to_2d(p), q.poly() )  ){
        *out++ = p;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const Segment_3 & s, const PolygonWH_3& q, OutputIteratorType out )
{
    if ( q.plane().has_on( s.source() ) &&q.plane().has_on( s.target() ) ){
        std::list<Segment_2> res;
        difference( Segment_2( q.plane().to_2d( s.source() ), q.plane().to_2d( s.target() ) ), q.poly(), std::back_inserter(res) );
        for (std::list<Segment_2>::const_iterator i = res.begin(); i!=res.end(); ++i ){
            *out++ = Segment_3(  q.plane().to_3d( i->source() ),  q.plane().to_3d( i->target() ) ) ;
        }
    }
    else{
        *out++ = s;
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const PolygonWH_3& p, const PolygonWH_3& q, OutputIteratorType out )
{
    if ( p.plane() != q.plane() ){
        *out++ = p;
    }
    else{
        const PolygonWH_3 qp(q, p.plane()); // put them in the same plane 
                                            // for 2D operation note that
                                            // the equality does not garanty 
                                            // that the projection is the same
                                            // for both planes
        std::vector< PolygonWH_2 > res;
        difference( p.poly(), qp.poly(), std::back_inserter(res) );
        for (unsigned i = 0; i<res.size(); i++) *out++ = PolygonWH_3( res[i], p.plane() );
    }
    return out;
}

template < typename OutputIteratorType >
OutputIteratorType difference( const PolygonWH_3& , const MarkedPolyhedron & , OutputIteratorType out )
{
    BOOST_THROW_EXCEPTION(NotImplementedException("Polygon - Volume not implemented ") );
    return out;
}


template <int Dim>
std::vector< typename HandledBox<Dim>::Type > compute_bboxes( const GeometrySet<Dim>& gs )
{

    typedef typename  PointPtr_d<Dim>::Type PointPtr;
    typedef typename  SegmentPtr_d<Dim>::Type SegmentPtr;
    typedef typename  SurfacePtr_d<Dim>::Type SurfacePtr;
    typedef typename  VolumePtr_d<Dim>::Type VolumePtr;

    typedef typename HandledBox<Dim>::Type HandledB;
    //typedef typename boost::shared_ptr< PrimitiveVec<Dim> > PrimVPtr;
    std::vector< HandledB > bboxes;

    for ( typename GeometrySet<Dim>::PointCollection::const_iterator it = gs.points().begin();
            it != gs.points().end(); ++it ) {
        PointPtr p( new typename PointPtr::element_type( it->primitive() ) );
        boost::shared_ptr< PrimitiveVec<Dim> > pp( new PrimitiveVec<Dim>( typename PrimitiveVec<Dim>::PointPtrVec( 1, p ) ) );
        bboxes.push_back( HandledB( it->primitive().bbox(), pp ) );
    }

    for ( typename GeometrySet<Dim>::SegmentCollection::const_iterator it = gs.segments().begin();
            it != gs.segments().end(); ++it ) {
        SegmentPtr p( new typename SegmentPtr::element_type( it->primitive() ) );
        boost::shared_ptr< PrimitiveVec<Dim> > pp( new PrimitiveVec<Dim>( typename PrimitiveVec<Dim>::SegmentPtrVec( 1, p ) ) );
        bboxes.push_back( HandledB( it->primitive().bbox(), pp ) );
    }

    for ( typename GeometrySet<Dim>::SurfaceCollection::const_iterator it = gs.surfaces().begin();
            it != gs.surfaces().end(); ++it ) {
        SurfacePtr p( new typename SurfacePtr::element_type( it->primitive() ) );
        boost::shared_ptr< PrimitiveVec<Dim> > pp( new PrimitiveVec<Dim>( typename PrimitiveVec<Dim>::SurfacePtrVec( 1, p ) ) );
        bboxes.push_back( HandledB( it->primitive().bbox(), pp ) );
    }

    for ( typename GeometrySet<Dim>::VolumeCollection::const_iterator it = gs.volumes().begin();
            it != gs.volumes().end(); ++it ) {
        VolumePtr p( new typename VolumePtr::element_type( it->primitive() ) );
        boost::shared_ptr< PrimitiveVec<Dim> > pp( new PrimitiveVec<Dim>( typename PrimitiveVec<Dim>::VolumePtrVec( 1, p ) ) );
        bboxes.push_back( HandledB( compute_solid_bbox( it->primitive(), dim_t<Dim>() ), pp ) );
    }

    return bboxes;
}

template <class VectorPrimitiveTypeA, class VectorPrimitiveTypeB, class HandleBoxVectorType >
void differenceInplace_( VectorPrimitiveTypeA& a, VectorPrimitiveTypeB& b, HandleBoxVectorType & boxes )
{
    typedef typename VectorPrimitiveTypeA::value_type PrimPrt;
    typedef typename PrimPrt::element_type PrimitiveType;

    VectorPrimitiveTypeA toAdd;

    for ( unsigned i=0;  i < a.size(); ) { // we may reduce the array size in the loop
        std::vector<PrimitiveType> primitives( 1, *a[i] );

        for ( typename VectorPrimitiveTypeB::const_iterator bit=b.begin(); bit!=b.end(); ++bit ) {
            std::vector<PrimitiveType> new_primitives;

            for ( typename std::vector<PrimitiveType>::const_iterator pit = primitives.begin();
                    pit != primitives.end(); ++pit ) {
                difference( *pit, *( *bit ), std::back_inserter( new_primitives ) );
            }

            primitives.swap( new_primitives );
        }

        if ( primitives.size() ){
            typename std::vector<PrimitiveType>::const_iterator pit = primitives.begin();
            PrimPrt p( new PrimitiveType( *pit ) );
            changeAdress( a[i], p, boxes );
            for ( ++pit; pit != primitives.end(); ++pit ) {
                toAdd.push_back( PrimPrt( new PrimitiveType( *pit ) ) );
            }
            ++i;
        }
        else {
            removeAdress( a[i], boxes ); 
            // do not increase i because the primitive has been removed
        }
    }

    a.insert( a.end(), toAdd.begin(), toAdd.end() );
}

template <int Dim, class HandleBoxVectorType>
void differenceInplace( PrimitiveVec<Dim>& a, const PrimitiveVec<Dim>& b, HandleBoxVectorType & boxes )
{
    switch ( a.which() ) {
    case PrimitivePoint:
        switch( b.which() ) {
        case PrimitiveSegment:
            differenceInplace_( a.template as< typename PrimitiveVec<Dim>::PointPtrVec >(),
                                b.template as< typename PrimitiveVec<Dim>::SegmentPtrVec >(), boxes );
            break;

        case PrimitiveSurface:
            differenceInplace_( a.template as< typename PrimitiveVec<Dim>::PointPtrVec >(),
                                b.template as< typename PrimitiveVec<Dim>::SurfacePtrVec >(), boxes );
            break;

        case PrimitiveVolume:
            differenceInplace_( a.template as< typename PrimitiveVec<Dim>::PointPtrVec >(),
                                b.template as< typename PrimitiveVec<Dim>::VolumePtrVec >(), boxes );
            break;
        }

        break;

    case PrimitiveSegment:
        switch( b.which() ) {
        case PrimitiveSurface:
            differenceInplace_( a.template as< typename PrimitiveVec<Dim>::SegmentPtrVec >(),
                                b.template as< typename PrimitiveVec<Dim>::SurfacePtrVec >(), boxes );
            break;

        case PrimitiveVolume:
            differenceInplace_( a.template as< typename PrimitiveVec<Dim>::SegmentPtrVec >(),
                                b.template as< typename PrimitiveVec<Dim>::VolumePtrVec >(), boxes );
            break;
        }

        break;

    case PrimitiveSurface:
        BOOST_ASSERT( b.which() == PrimitiveVolume );
        differenceInplace_( a.template as< typename PrimitiveVec<Dim>::SurfacePtrVec >(),
                            b.template as< typename PrimitiveVec<Dim>::VolumePtrVec >(), boxes );
        break;

    case PrimitiveVolume:
        BOOST_ASSERT( false ); // bug here, dim(a) < dim(b) means no Volume
    }
}

template <typename PointOutputIteratorType>
PointOutputIteratorType union_( const Point_2& a, const Point_2& b, PointOutputIteratorType out )
{
    if ( a == b ) *out++ = a;
    return out;
}

template <typename PointOutputIteratorType>
PointOutputIteratorType union_( const Point_3& a, const Point_3& b, PointOutputIteratorType out )
{
    if ( a == b ) *out++ = a;
    return out;
}

template <int Dim, typename SegmentOutputIteratorType>
SegmentOutputIteratorType union_( const typename SegmentPtr_d<Dim>::Type::element_type& a, const typename SegmentPtr_d<Dim>::Type::element_type& b, SegmentOutputIteratorType out )
{
    typedef typename TypeForDimension<Dim>::Segment SegmentType;
    typedef typename TypeForDimension<Dim>::Point PointType;

    // we only do something if the segments actually overlap or cross, not just touch at end points
    // othrewise we could mess-up things that are already takend care of in the segment sewing
    // of GeometrySet
    CGAL::Object inter = CGAL::intersection( a, b );

    const PointType* p = CGAL::object_cast< PointType >( &inter );
    const SegmentType* s = CGAL::object_cast< SegmentType >( &inter );

    if ( p ) {
        // create 4 segments:
        SegmentType res[4] = { SegmentType( a.source(), *p ),
                               SegmentType( b.source(), *p ),
                               SegmentType( *p, a.target() ),
                               SegmentType( *p, b.target() )
                             };
        unsigned nbSegmentsWithLength = 0;

        for ( unsigned i=0; i<4; i++ ) {
            nbSegmentsWithLength += ( res[i].target() != res[i].source() ) ? 1 : 0;
        }

        if ( nbSegmentsWithLength >= 3 ) { // end-to end otherwise
            for ( unsigned i=0; i<4; i++ ) {
                if ( res[i].target() != res[i].source() ) {
                    *out++ = res[i];
                }
            }
        }
    }
    else if ( s ) {
        // that is a join, we must find the points that are the further appart
        PointType points[4] = {a.source(), b.source(), a.target(),  b.target() };
        Kernel::FT dist = 0;
        unsigned imax=unsigned( -1 );
        unsigned jmax=unsigned( -1 );

        for ( unsigned i=0; i<4; i++ ) {
            for ( unsigned j=i; j<4; j++ ) {
                const Kernel::FT d = CGAL::squared_distance( points[i], points[j] );

                if ( d > dist ) {
                    dist = d;
                    imax = i;
                    jmax = j;
                }
            }
        }

        *out++ =  SegmentType( points[imax], points[jmax] );
    }

    return out;
}
template <typename SegmentOutputIteratorType>
SegmentOutputIteratorType union_( const Segment_2& a, const Segment_2& b, SegmentOutputIteratorType out )
{
    return union_<2,SegmentOutputIteratorType>( a, b, out );
}

template <typename SegmentOutputIteratorType>
SegmentOutputIteratorType union_( const Segment_3& a, const Segment_3& b, SegmentOutputIteratorType out )
{
    return union_<3,SegmentOutputIteratorType>( a, b, out );
}

template <typename PolygonOutputIteratorType>
PolygonOutputIteratorType union_( const PolygonWH_2& a, const PolygonWH_2& b, PolygonOutputIteratorType out )
{
    CGAL::Gps_segment_traits_2<Kernel> traits;
    PolygonWH_2 res;

    if ( CGAL::join(
                are_holes_and_boundary_pairwise_disjoint( a, traits ) ? a : fix_sfs_valid_polygon( a ),
                are_holes_and_boundary_pairwise_disjoint( b, traits ) ? b : fix_sfs_valid_polygon( b ),
                res ) ) {
        //out = fix_cgal_valid_polygon( res, out );
        *out++ = res;
    }

    return out;
}

template <typename VolumeOutputIteratorType>
VolumeOutputIteratorType union_( const NoVolume& , const NoVolume& , VolumeOutputIteratorType out )
{
    BOOST_ASSERT( false ); // should not be called, just here because template functions generates that
    return out;
}

template <typename OutputIteratorType>
OutputIteratorType union_( const PolygonWH_3 & a, const PolygonWH_3 & b, OutputIteratorType out )
{
    if ( a.plane() == b.plane() ){
        const PolygonWH_3 bp(b, a.plane()); // put them in the same plane 
                                            // for 2D operation 
                                            // (the projection needs beeing the same)
        out = union_( a, bp, out);
    }
    return out;
}

template <typename VolumeOutputIteratorType>
VolumeOutputIteratorType union_( const MarkedPolyhedron& a, const MarkedPolyhedron& b, VolumeOutputIteratorType out )
{
    MarkedPolyhedron& p = const_cast<MarkedPolyhedron&>( a );
    MarkedPolyhedron& q = const_cast<MarkedPolyhedron&>( b );
    typedef CGAL::Polyhedron_corefinement<MarkedPolyhedron> Corefinement;
    Corefinement coref;
    CGAL::Emptyset_iterator no_polylines;
    typedef std::vector<std::pair<MarkedPolyhedron*, int> >  ResultType;
    ResultType result;
    DEBUG_OUT << "joining polyhedron\n";
    try {
        coref( p, q, no_polylines, std::back_inserter( result ), Corefinement::Join_tag );
        if ( result.size() == 1)  *out++ = *result[0].first;
        for ( ResultType::iterator it = result.begin(); it != result.end(); it++ ) {
            delete it->first;
        }
        DEBUG_OUT << "joined in" << result.size() << " polyhedron\n";
    }
    catch (std::logic_error){
        // will happen if they only share an edge
    }

    return out;
}

template <class VectorPrimitiveType>
void gnuplot( const char* , VectorPrimitiveType )
{
}

//void gnuplot( const char * fileName, const std::vector<typename boost::shared_ptr<Triangle_3> >&   a )
//{
//    std::ofstream out( fileName );
//
//    for ( unsigned i=0; i<a.size(); i++ ) {
//        if ( !a[i].get() ) continue;
//        for ( unsigned j=0; j<4; j++ ) {
//            out << a[i]->vertex( j%3 ).x() << " " <<   a[i]->vertex( j%3 ).y() << "\n";
//        }
//
//        out << "\n";
//    }
//
//}

void gnuplot( const char * fileName, const std::vector<typename boost::shared_ptr<PolygonWH_2> >&   a )
{
    std::ofstream out( fileName );

    for ( unsigned i=0; i<a.size(); i++ ) {
        if ( !a[i].get() ) continue;
        std::vector< Polygon_2 > rings( 1, a[i]->outer_boundary() );
        rings.insert( rings.end(), a[i]->holes_begin(), a[i]->holes_end() );

        for ( std::vector< Polygon_2 >::iterator ring = rings.begin(); ring != rings.end(); ++ring ) {
            for (Polygon_2::Vertex_const_iterator v = ring->vertices_begin(); 
                    v!=ring->vertices_end(); ++v){
                out << v->x() << " " <<  v->y() << "\n";
            }
            out << "\n";
        }
    }
}

/*
void union_( std::vector<typename boost::shared_ptr<Triangle_3> > , std::vector<typename boost::shared_ptr<Triangle_3> > )
{
    if ( !a.size() || !b.size() ) return;
    const Plane_3 plane = a.supporting_plane();

    if ( b.supporting_plane() == plane ){
        // we deal globally with the merge, we put all triangles in the plane an create
        // a constrained triangulation

        triangulate::ConstraintDelaunayTriangulation cdt;

        for (std::vector<typename boost::shared_ptr<Triangle_3> >::const_iterator tit = a.begin(); 
                tit != a.end(); ++tit ){
            Vertex_handle last ;
            for ( unsigned i=0; i<4; i++ ) {
                Vertex_handle vertex = cdt.addVertex( g.vertex( i ).coordinate() );
                if ( i != 0 ) cdt.addConstraint( last, vertex ) ;
                last = vertex ;
            }
        }
        for (std::vector<typename boost::shared_ptr<Triangle_3> >::const_iterator tit = b.begin(); 
                tit != b.end(); ++tit ){
            Vertex_handle last ;
            for ( unsigned i=0; i<4; i++ ) {
                Vertex_handle vertex = cdt.addVertex( g.vertex( i ).coordinate() );
                if ( i != 0 ) cdt.addConstraint( last, vertex ) ;
                last = vertex ;
            }
        }

        cdt.markDomains();
        TriangulatedSurface ts;
        cdt.getTriangles( ts, true );

        a.resize(0);
        b.resize(0);

        for ( TriangulatedSurface::iterator t = ts.begin(); t != ts.end(); ++t ) {
            const Triangle_3 * t = new Triangle_3( plane.to_3d( t->vertex( 0 ).toPoint_2() ),
                                                   plane.to_3d( t->vertex( 1 ).toPoint_2() ),
                                                   plane.to_3d( t->vertex( 2 ).toPoint_2() ) ) ;
            a.push_back( t );
            b.push_back( t
        }
        

    }
    else{
        // do nothing yet
        // @todo decide if we want to split the triangles the way we do segments (and do it)
    }

}
*/


template <class VectorPrimitiveType, class HandleBoxVectorType>
void unionT( VectorPrimitiveType& a, VectorPrimitiveType& b, HandleBoxVectorType & boxes)
{

    // we can do it the dumb o(n^2) way because n is small
    // for points it's quite simple, we juste have to merge duplicate points
    // for volumes, either not touching, and primitives are unchanged, or merge
    // for polygons it's the same as for volumes
    // for segments and surfaces it's different since we can increase the number of primitives
    //
    // because we are only dealing with triangles for 3D surface, we cannot rely only
    // the fact that the two primitives ptr are pointing to the same primitive
    // to avoid join of already joinde primitives, the join of two triangles yields a TIN
    // and we must have a way to identify wich triangle pairs belong to the same TIN
    // we need a specific triangle U triangle
    //
    // For polygons and triangles, the merge can result in more than one primitive
    // because: 
    // - the union of 2 polygons yield more than one polygon in the case where the 
    //   polygon is cgal valid but sfs invalid (self intersecting outer ring)
    // - the union of 2 triangles yiels a polygon that is triangulated
    gnuplot("a.out", a);
    gnuplot("b.out", b);

    typedef typename VectorPrimitiveType::value_type PrimPrt;
    typedef typename PrimPrt::element_type PrimitiveType;

    std::vector< std::vector< unsigned > > alreadyJoined( a.size() );

    for ( unsigned i=0; i<a.size(); i++ ) {
        for ( unsigned j=0; j<b.size(); j++ ) {
            if ( a[i].get() == b[j].get() ) continue; // it's the same primitive

            // because they have been merged previously
            std::vector<PrimitiveType> out;
            union_( *a[i], *b[j], std::back_inserter( out ) );

            if ( out.size() == 1 ) { // they have been merged into one, so we replace both with it
                typename boost::shared_ptr<PrimitiveType> newPrim( new PrimitiveType( out[0] ) );
                changeAdress( a[i], newPrim , boxes);
                changeAdress( b[j], newPrim , boxes);
            }
            else if ( out.size() > 1 ) { // they have been merged into several,
                // so we put them at the end and set the ptr to NULL;
                DEBUG_OUT << "result " << out.size() << "\n";
                BOOST_THROW_EXCEPTION( NotImplementedException("union yields several primitives") );
            }
            // else no merge occured, so we do nothing

            //char af[20];
            //char bf[20];
            //sprintf(af, "a%05d%05d.out", i, j);
            //sprintf(bf, "b%05d%05d.out", i, j);

            //gnuplot(af, a);
            //gnuplot(bf, b);
        }
    }

    // now we remove NULL from a and b
    for ( typename VectorPrimitiveType::iterator ait=a.begin(); ait!=a.end(); ++ait ) {
        while ( ait!=a.end() && !ait->get() ) {
            ait = a.erase( ait );
        }
    }

    for ( typename VectorPrimitiveType::iterator bit=b.begin(); bit!=b.end(); ++bit ) {
        while ( bit!=b.end() && !bit->get() ) {
            bit = b.erase( bit );
        }
    }
}


template <int Dim>
void union_( PrimitiveVec<Dim>& a, PrimitiveVec<Dim>& b, typename HandledBox<Dim>::Vector & boxes )
{
    switch ( a.which() ) {
    case PrimitivePoint:
        unionT( a.template as< typename PrimitiveVec<Dim>::PointPtrVec >(),
                b.template as< typename PrimitiveVec<Dim>::PointPtrVec >(), boxes );
        break;

    case PrimitiveSegment:
        unionT( a.template as< typename PrimitiveVec<Dim>::SegmentPtrVec >(),
                b.template as< typename PrimitiveVec<Dim>::SegmentPtrVec >(), boxes );
        break;

    case PrimitiveSurface:
        unionT( a.template as< typename PrimitiveVec<Dim>::SurfacePtrVec >(),
                b.template as< typename PrimitiveVec<Dim>::SurfacePtrVec >(), boxes );
        break;

    case PrimitiveVolume:
        unionT( a.template as< typename PrimitiveVec<Dim>::VolumePtrVec >(),
                b.template as< typename PrimitiveVec<Dim>::VolumePtrVec >(), boxes );
        break;
    }
}

template <int Dim>
struct UnionOnBoxCollision {

    UnionOnBoxCollision( typename HandledBox<Dim>::Vector& boxes ):_boxes(boxes){}

    void operator()( typename HandledBox<Dim>::Type& a,
                     typename HandledBox<Dim>::Type& b ) {
        // note that all primitives in a have the same type, same holds for primitives in b
        if (  b.handle()->which() < a.handle()->which() ) {
            differenceInplace( *b.handle(), *a.handle(), _boxes );
        }
        else if ( a.handle()->which() < b.handle()->which() ) {
            differenceInplace( *a.handle(), *b.handle(), _boxes );
        }
        else {
            union_( *a.handle(), *b.handle(), _boxes );
        }
    }
private:
    typename HandledBox<Dim>::Vector& _boxes;
};

void collectSurfaces( typename PrimitiveVec<2>::SurfacePtrVec v,  GeometrySet<2>& output )
{
    for ( typename PrimitiveVec<2>::SurfacePtrVec::const_iterator it=v.begin(); 
            it!= v.end(); ++it ) {
        std::list< PolygonWH_2 > result;
        fix_cgal_valid_polygon( *(*it), std::back_inserter(result));
        output.addSurfaces( result.begin(), result.end() );
    }
}

void collectSurfaces( typename PrimitiveVec<3>::SurfacePtrVec v,  GeometrySet<3>& output )
{
    for ( typename PrimitiveVec<3>::SurfacePtrVec::const_iterator it=v.begin(); 
            it!= v.end(); ++it ) {

        const Plane_3 & plane = (*it)->plane();
        std::list< PolygonWH_2 > result;
        fix_cgal_valid_polygon( (*it)->poly(), std::back_inserter(result));

        for ( std::list< PolygonWH_2 >::const_iterator pit = result.begin();
                pit != result.end(); ++pit ){
            const Polygon poly( *pit );
            TriangulatedSurface ts;
            triangulate::ConstraintDelaunayTriangulation cdt;
            triangulate::triangulate2DZ( poly, cdt );
            cdt.markDomains();
            cdt.getTriangles( ts, true );

            for ( TriangulatedSurface::iterator t = ts.begin(); t != ts.end(); ++t ) {
                output.addPrimitive( 
                         Triangle_3( plane.to_3d( t->vertex( 0 ).toPoint_2() ),
                                     plane.to_3d( t->vertex( 1 ).toPoint_2() ),
                                     plane.to_3d( t->vertex( 2 ).toPoint_2() ) ) );
            }
        }
    }
}

template <int Dim>
void collectPrimitives( const typename HandledBox<Dim>::Vector& boxes, GeometrySet<Dim>& output )
{
    typedef typename PrimitiveVec<Dim>::PointPtrVec PointPtrVec;
    typedef typename PrimitiveVec<Dim>::SegmentPtrVec SegmentPtrVec;
    typedef typename PrimitiveVec<Dim>::SurfacePtrVec SurfacePtrVec;
    typedef typename PrimitiveVec<Dim>::VolumePtrVec VolumePtrVec;

    for ( typename  HandledBox<Dim>::Vector::const_iterator bit = boxes.begin();
            bit != boxes.end(); ++bit ) {
        switch ( bit->handle()->which() ) {
        case PrimitivePoint : {
            const PointPtrVec& v = bit->handle()->template as< PointPtrVec >();

            for ( typename PointPtrVec::const_iterator it=v.begin(); it!= v.end(); ++it ) {
                output.addPrimitive( *( *it ) );
            }
        }
        break;

        case PrimitiveSegment : {
            const SegmentPtrVec& v = bit->handle()->template as< SegmentPtrVec >();

            for ( typename SegmentPtrVec::const_iterator it=v.begin(); it!= v.end(); ++it ) {
                output.addPrimitive( *( *it ) );
            }
        }
        break;

        case PrimitiveSurface : {
            collectSurfaces( bit->handle()->template as< SurfacePtrVec >(), output );
        }
        break;

        case PrimitiveVolume : {
            const VolumePtrVec& v = bit->handle()->template as< VolumePtrVec >();

            for ( typename VolumePtrVec::const_iterator it=v.begin(); it!= v.end(); ++it ) {
                output.addPrimitive( *( *it ) );
            }
        }
        break;
        }
    }
}

template <int Dim>
void union_( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b, GeometrySet<Dim>& output )
{
    // we need the bbox handle to be able to hold mode than one primitive that can
    // be eddited and which pointer can be moved to point toward anoter primitive
    // we must be able to remove a primitive through the bbox

    typename HandledBox<Dim>::Vector aboxes = compute_bboxes( a );
    typename HandledBox<Dim>::Vector bboxes = compute_bboxes( b );

    typename HandledBox<Dim>::Vector boxes( aboxes );
    boxes.insert( boxes.end(), bboxes.begin(), bboxes.end() );

    CGAL::box_intersection_d( boxes.begin(), boxes.begin() + aboxes.size(),
                              boxes.begin() + aboxes.size(), boxes.end(),
                              UnionOnBoxCollision<Dim>(boxes) );

    // now collect primitives in all bboxes and we are done
    GeometrySet<Dim> temp, temp2;
    collectPrimitives<Dim>( aboxes, temp );
    collectPrimitives<Dim>( bboxes, temp );

    output.merge( temp );
}

template void union_<2>( const GeometrySet<2>& a, const GeometrySet<2>& b, GeometrySet<2>& );
template void union_<3>( const GeometrySet<3>& a, const GeometrySet<3>& b, GeometrySet<3>& );

std::auto_ptr<Geometry> union_( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    GeometrySet<2> gsa( ga ), gsb( gb ), output;
    algorithm::union_( gsa, gsb, output );

    GeometrySet<2> filtered;
    output.filterCovered( filtered ); //@todo remove this, normally the algo won't duplicate primitives
    return filtered.recompose();
}

std::auto_ptr<Geometry> union_( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D( gb );

    DEBUG_OUT << "union_ " << ga.asText() << " and " << gb.asText() << "\n";

    return union_( ga, gb, NoValidityCheck() );
}

std::auto_ptr<Geometry> union3D( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    GeometrySet<3> gsa( ga ), gsb( gb ), output;
    algorithm::union_( gsa, gsb, output );

    GeometrySet<3> filtered;
    output.filterCovered( filtered );

    return filtered.recompose();
}

std::auto_ptr<Geometry> union3D( const Geometry& ga, const Geometry& gb )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( ga );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D( gb );

    DEBUG_OUT << "union3D " << ga.asText() << " and " << gb.asText() << "\n";

    return union3D( ga, gb, NoValidityCheck() );
}
}
}
