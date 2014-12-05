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
#include <SFCGAL/Exception.h>
#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/algorithm/isValid.h>

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/box_intersection_d.h>

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
    DEBUG_OUT << "\n";
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

template <int Dim>
void union_( PrimitiveVec<Dim> & a, const PrimitiveVec<Dim> & )
{
    DEBUG_OUT << "\n";
    switch (a.which()){
        case PrimitivePoint:
            DEBUG_OUT << "Points U Points\n";
            break;
        case PrimitiveSegment:
            DEBUG_OUT << "Segments U Segments\n";
            break;
        case PrimitiveSurface:
            DEBUG_OUT << "Surfaces U Surfaces\n";
            break;
        case PrimitiveVolume:
            DEBUG_OUT << "Volumes U Volumes\n";
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
            DEBUG_OUT << "dim(b) < dim(a)\n";
            differenceInplace( *b.handle(), *a.handle() ); 
        }
        else if ( a.handle()->which() < b.handle()->which() ) {
            DEBUG_OUT << "dim(a) < dim(b)\n";
            differenceInplace( *a.handle(), *b.handle() ); 
        }
        else{
            DEBUG_OUT << "same dimension\n";
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

    return union3D( ga, gb, NoValidityCheck() );
}
}
}
