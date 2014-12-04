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

#define DEBUG_OUT if (1) std::cerr << __FILE__ << ":" << __LINE__ << "debug: " 

using namespace SFCGAL::detail;

namespace SFCGAL {

namespace algorithm {

template <int Dim>
struct PrimitivePtr: boost::variant< 
        typename boost::shared_ptr<typename Point_d<Dim>::Type>,
        typename boost::shared_ptr<typename Segment_d<Dim>::Type>,
        typename boost::shared_ptr<typename Surface_d<Dim>::Type>,
        typename boost::shared_ptr<typename Volume_d<Dim>::Type> >
{

    template <class T>
    PrimitivePtr( T* p )
        : boost::variant< 
            typename boost::shared_ptr<typename Point_d<Dim>::Type>,
            typename boost::shared_ptr<typename Segment_d<Dim>::Type>,
            typename boost::shared_ptr<typename Surface_d<Dim>::Type>,
            typename boost::shared_ptr<typename Volume_d<Dim>::Type> 
            >( boost::shared_ptr<T>(p) )
    {}

    template <typename T>
    const T& as() const {
        return *(boost::get< boost::shared_ptr<T> >( *this ) );
    }
};

template <class T, int Dim>
inline const T& to( )
{
}

template <int Dim>
struct PrimitivePtrVec{
    typedef std::vector< PrimitivePtr<Dim> > Type;
};

// this one is a tad complicated because we want bboxes with handle to be responsible for the
// primitives, hence the use of shared ptr instead of a dumb ptr
template <int Dim>
struct HandledBox{
    typedef CGAL::Box_intersection_d::Box_with_handle_d<double, Dim, boost::shared_ptr< typename PrimitivePtrVec<Dim>::Type > > Type;
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

    typedef typename HandledBox<Dim>::Type HandledB;
    typedef typename PrimitivePtrVec<Dim>::Type PrimPtrV;
    typedef boost::shared_ptr< PrimPtrV > PrimPtrVPtr;
    std::vector< HandledB > bboxes;

    for ( typename GeometrySet<Dim>::PointCollection::const_iterator it = gs.points().begin(); 
            it != gs.points().end(); ++it ) {
        bboxes.push_back( HandledB( it->primitive().bbox(), 
             PrimPtrVPtr(new PrimPtrV(1, new typename TypeForDimension<Dim>::Point( it->primitive() ) ) ) ) );
    }
    for ( typename GeometrySet<Dim>::SegmentCollection::const_iterator it = gs.segments().begin(); 
            it != gs.segments().end(); ++it ) {
        bboxes.push_back( HandledB( it->primitive().bbox(), 
            PrimPtrVPtr(new PrimPtrV(1, new typename TypeForDimension<Dim>::Segment( it->primitive() ) ) ) ) );
    }
    for ( typename GeometrySet<Dim>::SurfaceCollection::const_iterator it = gs.surfaces().begin(); 
            it != gs.surfaces().end(); ++it ) {
        bboxes.push_back( HandledB( it->primitive().bbox(), 
            PrimPtrVPtr(new PrimPtrV(1, new typename TypeForDimension<Dim>::Surface( it->primitive() ) ) ) ) );
    }
    for ( typename GeometrySet<Dim>::VolumeCollection::const_iterator it = gs.volumes().begin(); 
            it != gs.volumes().end(); ++it ) {
        bboxes.push_back( HandledB( compute_solid_bbox( it->primitive(), dim_t<Dim>() ), 
            PrimPtrVPtr(new PrimPtrV(1, new typename TypeForDimension<Dim>::Volume( it->primitive() ) ) ) ) );
    }

    return bboxes;
}

template <int Dim>
struct UnionOnBoxCollision
{
    void operator()( const typename HandledBox<Dim>::Type& a,
                     const typename HandledBox<Dim>::Type& b ) 
    {
        if ( !a.handle()->size() || !b.handle()->size() ) return; // nothing to do
        // note that all primitives in a have the same type, same holds for primitives in b
        if ( (*a.handle())[0].which() == (*a.handle())[0].which() ){
            DEBUG_OUT << "same dimension";
        }
        else {
            DEBUG_OUT << "different dimension";
        }
    }
};

template <int Dim>
void union_( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b, GeometrySet<Dim>& output)
{
    // we need the bbox handle to be able to hold mode than one primitive that can
    // be eddited and which pointer can be moved to point toward anoter primitive
    // we must be able to remove a primitive through the bbox

    typedef std::vector< typename HandledBox<Dim>::Type > BoxVector;
    BoxVector boxes[2] = { compute_bboxes( a ), compute_bboxes( b ) };

    CGAL::box_intersection_d( boxes[0].begin(), boxes[0].end(),
                              boxes[1].begin(), boxes[1].end(),
                              UnionOnBoxCollision<Dim>() );

    // now collect primitives in all bboxes and we are done
    GeometrySet<Dim> temp, temp2;
    for (unsigned i=0; i<2; i++){
        for (typename BoxVector::const_iterator bit = boxes[i].begin(); 
                bit != boxes[i].end(); ++bit){
            for ( typename PrimitivePtrVec<Dim>::Type::const_iterator pit = bit->handle()->begin();
                    pit != bit->handle()->end(); ++pit ){
                switch ( pit->which() ){
                    case PrimitivePoint : 
                        temp.addPrimitive( pit->template as< typename TypeForDimension<Dim>::Point >() );
                        break;
                    case PrimitiveSegment : 
                        temp.addPrimitive( pit->template as< typename TypeForDimension<Dim>::Segment >() );
                        break;
                    case PrimitiveSurface : 
                        temp.addPrimitive( pit->template as< typename TypeForDimension<Dim>::Surface >() );
                        break;
                    case PrimitiveVolume : 
                        temp.addPrimitive( pit->template as< typename TypeForDimension<Dim>::Volume >() );
                        break;
                }
            }
        }
    }

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
