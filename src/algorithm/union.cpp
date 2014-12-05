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

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
//
// Union kernel

#define DEBUG_OUT if (1) std::cerr << __FILE__ << ":" << __LINE__ << " debug: " 

using namespace SFCGAL::detail;

namespace SFCGAL {

namespace algorithm {


template <int Dim>
struct PrimitiveVec: boost::variant< 
        typename std::vector<typename boost::shared_ptr<typename Point_d<Dim>::Type> >,
        typename std::vector<typename boost::shared_ptr<typename Segment_d<Dim>::Type> >,
        typename std::vector<typename boost::shared_ptr<typename Surface_d<Dim>::Type> >,
        typename std::vector<typename boost::shared_ptr<typename Volume_d<Dim>::Type> > >
{
    typedef boost::shared_ptr< typename Point_d<Dim>::Type > PointPtr;
    typedef boost::shared_ptr< typename Segment_d<Dim>::Type> SegmentPtr;
    typedef boost::shared_ptr< typename Surface_d<Dim>::Type> SurfacePtr;
    typedef boost::shared_ptr< typename Volume_d<Dim>::Type > VolumePtr;
    typedef std::vector< PointPtr   > PointPtrVec;
    typedef std::vector< SegmentPtr > SegmentPtrVec;
    typedef std::vector< SurfacePtr > SurfacePtrVec;
    typedef std::vector< VolumePtr  > VolumePtrVec;

    template <class T>
    PrimitiveVec( const T & v )
        : boost::variant< PointPtrVec, SegmentPtrVec, SurfacePtrVec, VolumePtrVec >( v )
    {}

    template <typename T>
    T& as() {
        return boost::get< T >( *this );
    }

    template <typename T>
    const T& as() const {
        return boost::get< T >( *this );
    }
};

// this one is a tad complicated because we want bboxes with handle to be responsible for the
// primitives, hence the use of shared ptr instead of a dumb ptr
template <int Dim>
struct HandledBox{
    typedef CGAL::Box_intersection_d::Box_with_handle_d<double, Dim, boost::shared_ptr< PrimitiveVec<Dim> > > Type;
    typedef std::vector< Type > Vector;
};

//void union_( const PrimitiveHandle<3>& pa, const PrimitiveHandle<3>& pb,
//                   GeometrySet<3>& output, dim_t<3> ){BOOST_ASSERT(false);}
//void union_( const PrimitiveHandle<2>& pa, const PrimitiveHandle<2>& pb,
//                   GeometrySet<2>& output, dim_t<2> ){BOOST_ASSERT(false);}
//

//template void union_<2>( const PrimitiveHandle<2>& a, const PrimitiveHandle<2>& b, GeometrySet<2>& );
//template void union_<3>( const PrimitiveHandle<3>& a, const PrimitiveHandle<3>& b, GeometrySet<3>& );
//

/**
 * union post processing
 */
void post_union( const GeometrySet<2>& input, GeometrySet<2>& output )
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

void post_union( const GeometrySet<3>& input, GeometrySet<3>& output )
{
    // nothing special to do
    output = input;
}

template <int Dim>
std::vector< typename HandledBox<Dim>::Type > compute_bboxes( const GeometrySet<Dim>& gs )
{
    typedef typename PrimitiveVec<Dim>::PointPtr PointPtr;
    typedef typename PrimitiveVec<Dim>::SegmentPtr SegmentPtr;
    typedef typename PrimitiveVec<Dim>::SurfacePtr SurfacePtr;
    typedef typename PrimitiveVec<Dim>::VolumePtr VolumePtr;
    typedef typename std::vector< PointPtr > PointPtrVec;
    typedef typename std::vector< SegmentPtr > SegmentPtrVec;
    typedef typename std::vector< SurfacePtr > SurfacePtrVec;
    typedef typename std::vector< VolumePtr > VolumePtrVec;

    typedef typename HandledBox<Dim>::Type HandledB;
    //typedef typename boost::shared_ptr< PrimitiveVec<Dim> > PrimVPtr;
    std::vector< HandledB > bboxes;

    for ( typename GeometrySet<Dim>::PointCollection::const_iterator it = gs.points().begin(); 
            it != gs.points().end(); ++it ) {
        PointPtr p( new typename TypeForDimension<Dim>::Point( it->primitive() ) );
        boost::shared_ptr< PrimitiveVec<Dim> > pp( new PrimitiveVec<Dim>( PointPtrVec(1, p) ) );
        bboxes.push_back( HandledB( it->primitive().bbox(), pp ) );
    }
    for ( typename GeometrySet<Dim>::SegmentCollection::const_iterator it = gs.segments().begin(); 
            it != gs.segments().end(); ++it ) {
        SegmentPtr p( new typename TypeForDimension<Dim>::Segment( it->primitive() ) );
        boost::shared_ptr< PrimitiveVec<Dim> > pp( new PrimitiveVec<Dim>( SegmentPtrVec(1, p) ) );
        bboxes.push_back( HandledB( it->primitive().bbox(), pp ) );
    }
    for ( typename GeometrySet<Dim>::SurfaceCollection::const_iterator it = gs.surfaces().begin(); 
            it != gs.surfaces().end(); ++it ) {
        SurfacePtr p( new typename TypeForDimension<Dim>::Surface( it->primitive() ) );
        boost::shared_ptr< PrimitiveVec<Dim> > pp( new PrimitiveVec<Dim>( SurfacePtrVec(1, p) ) );
        bboxes.push_back( HandledB( it->primitive().bbox(), pp ) );
    }
    for ( typename GeometrySet<Dim>::VolumeCollection::const_iterator it = gs.volumes().begin(); 
            it != gs.volumes().end(); ++it ) {
        VolumePtr p( new typename TypeForDimension<Dim>::Volume( it->primitive() ) );
        boost::shared_ptr< PrimitiveVec<Dim> > pp( new PrimitiveVec<Dim>( VolumePtrVec(1, p) ) );
        bboxes.push_back( HandledB( compute_solid_bbox( it->primitive(), dim_t<Dim>() ), pp ) );
    }

    return bboxes;
}

template <class VectorPrimitiveTypeA, class VectorPrimitiveTypeB>
void differenceInplace_(VectorPrimitiveTypeA & a, VectorPrimitiveTypeB & b)
{
    typedef typename VectorPrimitiveTypeA::value_type PrimPrt;
    typedef typename PrimPrt::element_type PrimitiveType;

    VectorPrimitiveTypeA result;

    for ( typename VectorPrimitiveTypeA::iterator ait=a.begin(); ait!=a.end(); ++ait ){
        std::vector<PrimitiveType> primitives( 1, *(*ait) );
        for ( typename VectorPrimitiveTypeB::const_iterator bit=b.begin(); bit!=b.end(); ++bit ){
            std::vector<PrimitiveType> new_primitives;
            for ( typename std::vector<PrimitiveType>::const_iterator pit = primitives.begin();
                    pit != primitives.end(); ++pit ) {
                difference( *pit, *(*bit), std::back_inserter( new_primitives ) );
            }
            primitives.swap( new_primitives );
        }

        for ( typename std::vector<PrimitiveType>::const_iterator pit = primitives.begin();
                pit != primitives.end(); ++pit ) {
            result.push_back( PrimPrt( new PrimitiveType( *pit ) ) );
        }
    }

    a.swap( result );
}

template <int Dim>
void differenceInplace( PrimitiveVec<Dim> & a, const PrimitiveVec<Dim> & b)
{
    switch (a.which()){
        case PrimitivePoint:
            switch(b.which()){
                case PrimitiveSegment:
                    differenceInplace_( a.template as< typename PrimitiveVec<Dim>::PointPtrVec >(), 
                            b.template as< typename PrimitiveVec<Dim>::SegmentPtrVec >());
                    break;
                case PrimitiveSurface:
                    differenceInplace_( a.template as< typename PrimitiveVec<Dim>::PointPtrVec >(), 
                            b.template as< typename PrimitiveVec<Dim>::SurfacePtrVec >());
                    break;
                case PrimitiveVolume:
                    differenceInplace_( a.template as< typename PrimitiveVec<Dim>::PointPtrVec >(), 
                            b.template as< typename PrimitiveVec<Dim>::VolumePtrVec >());
                    break;
            }
            break;
        case PrimitiveSegment:
            switch(b.which()){
                case PrimitiveSurface:
                    differenceInplace_( a.template as< typename PrimitiveVec<Dim>::SegmentPtrVec >(), 
                            b.template as< typename PrimitiveVec<Dim>::SurfacePtrVec >());
                    break;
                case PrimitiveVolume:
                    differenceInplace_( a.template as< typename PrimitiveVec<Dim>::SegmentPtrVec >(), 
                            b.template as< typename PrimitiveVec<Dim>::VolumePtrVec >());
                    break;
            }
            break;
        case PrimitiveSurface:
            BOOST_ASSERT( b.which() == PrimitiveVolume );
            differenceInplace_( a.template as< typename PrimitiveVec<Dim>::SurfacePtrVec >(), 
                    b.template as< typename PrimitiveVec<Dim>::VolumePtrVec >());
            break;
        case PrimitiveVolume: BOOST_ASSERT(false); // bug here, dim(a) < dim(b) means no Volume
    }
}

template <typename PointOutputIteratorType>
PointOutputIteratorType union_( const Point_2 & a, const Point_2 & b, PointOutputIteratorType out )
{
    if ( a == b ) *out++ = a;
    return out;
}

template <typename PointOutputIteratorType>
PointOutputIteratorType union_( const Point_3 & a, const Point_3 & b, PointOutputIteratorType out )
{
    if ( a == b ) *out++ = a;
    return out;
}

template <int Dim, typename SegmentOutputIteratorType>
SegmentOutputIteratorType union_( const typename TypeForDimension<Dim>::Segment & a, const typename TypeForDimension<Dim>::Segment & b, SegmentOutputIteratorType out )
{
    typedef typename TypeForDimension<Dim>::Segment SegmentType;
    typedef typename TypeForDimension<Dim>::Point PointType;

    // we only do something if the segments actually overlap or cross, not just touch at end points
    // othrewise we could mess-up things that are already takend care of in the segment sewing
    // of GeometrySet
    CGAL::Object inter = CGAL::intersection( a, b );

    const PointType* p = CGAL::object_cast< PointType >( &inter );
    const SegmentType* s = CGAL::object_cast< SegmentType >( &inter );
    if ( p ){
        // create 4 segments:
        SegmentType res[4] = { SegmentType(a.source(), *p),
                               SegmentType(b.source(), *p),
                               SegmentType(*p, a.target()),
                               SegmentType(*p, b.target()) };
        unsigned nbSegmentsWithLength = 0;
        for (unsigned i=0; i<4; i++ ) 
            nbSegmentsWithLength += (res[i].target() != res[i].source()) ? 1 : 0;
        if (nbSegmentsWithLength >= 3) { // end-to end otherwise
            for (unsigned i=0; i<4; i++ ){ 
                if (res[i].target() != res[i].source()) *out++ = res[i];
            }
        }
    }
    else if ( s ){
        // that is a join, we must find the points that are the further appart
        PointType points[4] = {a.source(), b.source(), a.target(),  b.target() };
        Kernel::FT dist = 0;
        unsigned imax=unsigned(-1);
        unsigned jmax=unsigned(-1);
        for (unsigned i=0; i<4; i++){
            for (unsigned j=i; j<4; j++){
                const Kernel::FT d = CGAL::squared_distance( points[i], points[j] );
                if ( d > dist ){
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
SegmentOutputIteratorType union_( const Segment_2 & a, const Segment_2 & b, SegmentOutputIteratorType out )
{
    return union_<2,SegmentOutputIteratorType>(a, b, out);
}

template <typename SegmentOutputIteratorType>
SegmentOutputIteratorType union_( const Segment_3 & a, const Segment_3 & b, SegmentOutputIteratorType out )
{
    return union_<3,SegmentOutputIteratorType>(a, b, out);
}

template <typename PolygonOutputIteratorType>
PolygonOutputIteratorType union_( const PolygonWH_2 & a, const PolygonWH_2 & b, PolygonOutputIteratorType out )
{
    CGAL::Gps_segment_traits_2<Kernel> traits;
    PolygonWH_2 res;
    if ( CGAL::join(
        are_holes_and_boundary_pairwise_disjoint( a, traits ) ? a : fix_sfs_valid_polygon( a ),
        are_holes_and_boundary_pairwise_disjoint( b, traits ) ? b : fix_sfs_valid_polygon( b ),
        res) ) *out++ = res;
    return out;
}

template <typename VolumeOutputIteratorType>
VolumeOutputIteratorType union_( const NoVolume & , const NoVolume & , VolumeOutputIteratorType out )
{
    BOOST_ASSERT(false); // should not be called, just here because template functions generates that
    return out;
}

template <typename PolygonOutputIteratorType>
PolygonOutputIteratorType union_( const Triangle_3 & a, const Triangle_3 & b, PolygonOutputIteratorType out )
{
    const Plane_3 plane = a.supporting_plane();

    if ( plane == b.supporting_plane() && CGAL::do_intersect( a, b ) ) {
        // project on plane
        // union polygons
        // triangulate the result

        // @gotcha do not union if triangles are just sharing an edge, they may be the result 
        // of a previous triangulation
        for ( unsigned i=0; i<3; i++ ){
            for ( unsigned j=0; j<3; j++){
                if ( a.vertex( i ) == b.vertex( (j+1)%3 ) && 
                     a.vertex( (i+1)%3 ) == b.vertex( j )){
                    // now check that the last points are on opposite side (dot product of cross products is negative or null)
                    Vector_3 a0a1( a.vertex( i ), a.vertex( (i+1)%3 ) );
                    Vector_3 a0a2( a.vertex( i ), a.vertex( (i+2)%3 ) );
                    Vector_3 a0b2( a.vertex( i ), b.vertex( (i+2)%3 ) );
                    Vector_3 n1 = CGAL::cross_product( a0a1, a0a2 );
                    Vector_3 n2 = CGAL::cross_product( a0a1, a0b2 );
                    if ( n1*n2 <= 0 ){
                        return out;
                    }
                }
            }
        }

        // the latter cannot be used because of ambiguous resolution of CGAL::intersection
        // its internal to CGAL, and cause by the Boolean_set_operations_2 include
        // so instead we have to check that the triangles are not just touching on a point
        //
        //CGAL::Object inter = CGAL::intersection( a, b );
        //if ( CGAL::object_cast< Point_3>( &inter ) ) return out; // just touching on a point

        Polygon_2 aProj, bProj;

        for ( unsigned i=0; i<3; i++ ) {
            aProj.push_back( plane.to_2d( a.vertex( i ) ) );
            bProj.push_back( plane.to_2d( b.vertex( i ) ) );
        }

        PolygonWH_2 res;
        if ( CGAL::join( aProj, bProj, res ) ){
            if (!res.outer_boundary().is_simple()) return out; // ring self intersects, triangles just touching on point

            const Polygon poly( res );
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


template <typename VolumeOutputIteratorType>
VolumeOutputIteratorType union_( const MarkedPolyhedron & a, const MarkedPolyhedron & b, VolumeOutputIteratorType out )
{
    MarkedPolyhedron& p = const_cast<MarkedPolyhedron&>( a );
    MarkedPolyhedron& q = const_cast<MarkedPolyhedron&>( b );
    typedef CGAL::Polyhedron_corefinement<MarkedPolyhedron> Corefinement;
    Corefinement coref;
    CGAL::Emptyset_iterator no_polylines;
    typedef std::vector<std::pair<MarkedPolyhedron*, int> >  ResultType;
    ResultType result;
    coref( p, q, no_polylines, std::back_inserter( result ), Corefinement::Join_tag );

    if (result.size() > 1){ //otherwise, they are left as they are
        for ( ResultType::iterator it = result.begin(); it != result.end(); it++ ) {
            *out++ = *it->first;
            delete it->first;
        }
    }
    return out;
}

template <class VectorPrimitiveType>
void union_( VectorPrimitiveType & a, VectorPrimitiveType & b )
{
    // we can do it the dumb o(n^2) way because n is small
    // for points it's quite simple, we juste have to merge duplicate points
    // for volumes, either not touching, and primitives are unchanged, or merge
    // for polygons it's the same as for volumes
    // for segments and surfaces it's different since we can increase the number of primitives
    //

    typedef typename VectorPrimitiveType::value_type PrimPrt;
    typedef typename PrimPrt::element_type PrimitiveType;

    for ( typename VectorPrimitiveType::iterator ait=a.begin(); ait!=a.end(); ++ait ){
        for ( typename VectorPrimitiveType::iterator bit=b.begin(); bit!=b.end(); ++bit ){
            if ( !ait->get()  || !bit->get() || // removed in the loop
                    ait->get() == bit->get() ) continue; // they are already the same, 
                                                      // because they have been merged previously
            std::vector<PrimitiveType> out;
            union_( *(*ait), *(*bit), std::back_inserter(out) );
            if ( out.size() == 1 ){ // they have been merged into one, so we replace both with it
                ait->reset( new PrimitiveType(out[0]) );
                *bit = *ait;
            }
            else if ( out.size() > 1 ){ // they have been merged into several, 
                                        // so we put them at the end and set the ptr to NULL;
                ait->reset();
                bit->reset();
                for (typename std::vector<PrimitiveType>::const_iterator it=out.begin(); 
                        it!=out.end(); ++it ){
                    const unsigned i =  a.begin() - ait;
                    const unsigned j =  b.begin() - bit;
                    a.push_back( PrimPrt( new PrimitiveType( *it ) ) );
                    b.push_back( a.back() );
                    ait = a.begin() + i; // push_back can invalidate iterators
                    bit = b.begin() + j;
                }
            }
            // else no merge occured, so we do nothing
        }
    }

    // now we remove NULL from a and b
    for ( typename VectorPrimitiveType::iterator ait=a.begin(); ait!=a.end(); ++ait ){
        while ( ait!=a.end() && !ait->get() ) ait = a.erase( ait );
    }
    for ( typename VectorPrimitiveType::iterator bit=b.begin(); bit!=b.end(); ++bit ){
        while ( bit!=b.end() && !bit->get() ) bit = b.erase( bit );
    }
}


template <int Dim>
void union_( PrimitiveVec<Dim> & a, PrimitiveVec<Dim> & b)
{
    switch (a.which()){
        case PrimitivePoint:
            union_( a.template as< typename PrimitiveVec<Dim>::PointPtrVec >(), 
                    b.template as< typename PrimitiveVec<Dim>::PointPtrVec >());
            break;
        case PrimitiveSegment:
            union_( a.template as< typename PrimitiveVec<Dim>::SegmentPtrVec >(), 
                    b.template as< typename PrimitiveVec<Dim>::SegmentPtrVec >());
            break;
        case PrimitiveSurface:
            union_( a.template as< typename PrimitiveVec<Dim>::SurfacePtrVec >(), 
                    b.template as< typename PrimitiveVec<Dim>::SurfacePtrVec >());
            break;
        case PrimitiveVolume:
            union_( a.template as< typename PrimitiveVec<Dim>::VolumePtrVec >(), 
                    b.template as< typename PrimitiveVec<Dim>::VolumePtrVec >());
            break;
    }
}

template <int Dim>
struct UnionOnBoxCollision
{
    void operator()( typename HandledBox<Dim>::Type& a,
                     typename HandledBox<Dim>::Type& b ) 
    {
        // note that all primitives in a have the same type, same holds for primitives in b
        if (  b.handle()->which() < a.handle()->which() ){
            differenceInplace( *b.handle(), *a.handle() ); 
        }
        else if ( a.handle()->which() < b.handle()->which() ) {
            differenceInplace( *a.handle(), *b.handle() ); 
        }
        else{
            union_( *a.handle(), *b.handle() ); 
        }
    }
};

template <int Dim>
void collectPrimitives( const typename HandledBox<Dim>::Vector & boxes, GeometrySet<Dim> & output )
{
    typedef typename PrimitiveVec<Dim>::PointPtrVec PointPtrVec;
    typedef typename PrimitiveVec<Dim>::SegmentPtrVec SegmentPtrVec;
    typedef typename PrimitiveVec<Dim>::SurfacePtrVec SurfacePtrVec;
    typedef typename PrimitiveVec<Dim>::VolumePtrVec VolumePtrVec;
    for (typename  HandledBox<Dim>::Vector::const_iterator bit = boxes.begin(); 
            bit != boxes.end(); ++bit){
        switch ( bit->handle()->which() ){
        case PrimitivePoint : 
            {
            const PointPtrVec & v = bit->handle()->template as< PointPtrVec >();
            for (typename PointPtrVec::const_iterator it=v.begin(); it!= v.end(); ++it)
                output.addPrimitive( *(*it) );
            }
            break;
        case PrimitiveSegment :
            {
            const SegmentPtrVec & v = bit->handle()->template as< SegmentPtrVec >();
            for (typename SegmentPtrVec::const_iterator it=v.begin(); it!= v.end(); ++it)
                output.addPrimitive( *(*it) );
            }
            break;
        case PrimitiveSurface :
            { 
            const SurfacePtrVec & v = bit->handle()->template as< SurfacePtrVec >();
            for (typename SurfacePtrVec::const_iterator it=v.begin(); it!= v.end(); ++it)
                output.addPrimitive( *(*it) );
            }
            break;
        case PrimitiveVolume : 
            {
            const VolumePtrVec & v = bit->handle()->template as< VolumePtrVec >();
            for (typename VolumePtrVec::const_iterator it=v.begin(); it!= v.end(); ++it)
                output.addPrimitive( *(*it) );
            }
            break;
        }
    }
}

template <int Dim>
void union_( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b, GeometrySet<Dim>& output)
{
    // we need the bbox handle to be able to hold mode than one primitive that can
    // be eddited and which pointer can be moved to point toward anoter primitive
    // we must be able to remove a primitive through the bbox

    typename HandledBox<Dim>::Vector aboxes = compute_bboxes( a );
    typename HandledBox<Dim>::Vector bboxes = compute_bboxes( b );

    CGAL::box_intersection_d( aboxes.begin(), aboxes.end(),
                              bboxes.begin(), bboxes.end(),
                              UnionOnBoxCollision<Dim>() );

    // now collect primitives in all bboxes and we are done
    GeometrySet<Dim> temp, temp2;
    collectPrimitives<Dim>( aboxes, temp );
    collectPrimitives<Dim>( bboxes, temp );

    post_union( temp, temp2 );
    output.merge( temp2 );
}

template void union_<2>( const GeometrySet<2>& a, const GeometrySet<2>& b, GeometrySet<2>& );
template void union_<3>( const GeometrySet<3>& a, const GeometrySet<3>& b, GeometrySet<3>& );

std::auto_ptr<Geometry> union_( const Geometry& ga, const Geometry& gb, NoValidityCheck )
{
    GeometrySet<2> gsa( ga ), gsb( gb ), output;
    algorithm::union_( gsa, gsb, output );

    GeometrySet<2> filtered;
    output.filterCovered( filtered );
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
