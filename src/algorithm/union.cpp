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

#include <SFCGAL/algorithm/union.h>
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

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
//
// Union kernel

using namespace SFCGAL::detail;

namespace SFCGAL {

typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Triangle_2<Kernel> Triangle_2;

namespace algorithm {
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

// class called by  box_intersection_d
// the intersecting boxes create edges in the graph
// the primitives are stored as graph node so that we have a cluster
// even for primitives that are completely disjoint from the rest
template <int Dim> 
struct Cluster
{
    typedef std::vector< PrimitiveHandle<Dim> > PrimitiveHandleSet;
    typedef typename SFCGAL::detail::BoxCollection<Dim>::Type BoxCollection;
    typedef boost::adjacency_list<
        boost::listS, 
        boost::vecS, 
        boost::undirectedS, 
        boost::no_property, 
        boost::no_property,
        boost::no_property, 
        boost::vecS> Graph;
    typedef std::map< PrimitiveHandle<Dim>*, int > PrimitiveMap;

    Cluster( const BoxCollection & aboxes,  const BoxCollection & bboxes)
        : _map( new PrimitiveMap() )
        , _graph( new Graph ) 
    {
        addHandles( aboxes.begin(), aboxes.end() );
        addHandles( bboxes.begin(), bboxes.end() );
    }

    void operator()( const typename PrimitiveBox<Dim>::Type& a,
                     const typename PrimitiveBox<Dim>::Type& b )
    {
        BOOST_ASSERT( _map->find(a.handle()) != _map->end()  && _map->find(b.handle()) != _map->end() );
        boost::add_edge( (*_map)[a.handle()], (*_map)[b.handle()], *_graph);
    }

    std::vector< PrimitiveHandleSet > get() 
    {
        std::vector<int> components( boost::num_vertices(*_graph) );
        const int num = boost::connected_components(*_graph, &components[0]);
        std::vector< PrimitiveHandleSet > out(num);
        for (typename PrimitiveMap::const_iterator it = _map->begin(); it != _map->end(); ++it){
            out[ components[ it->second ] ].push_back( *it->first );
        }
        return out;
    }

private:
    // we use shared ptr in order to have a correct default cpy when passing an instance as a functor
    // by value
    // @note we could have used a boost::ref wrapper instead but it doesn't play nice with box_intersection_d
    boost::shared_ptr<PrimitiveMap> _map;
    boost::shared_ptr<Graph> _graph;

    template <typename BoxIterator>
    void addHandles( BoxIterator begin, BoxIterator end)
    {
        for (BoxIterator it = begin; it != end; it++ ) 
            (*_map)[it->handle()] = boost::add_vertex(*_graph);
    }

};

// append to result the union of all primitives in cluster, one at a time
// we just append union to the result
template <int Dim>
void union_( const typename Cluster<Dim>::PrimitiveHandleSet & cluster, GeometrySet<Dim> & /*result*/){
    BOOST_THROW_EXCEPTION( NotImplementedException( std::string(__FUNCTION__)+" is not implemented" ) );
    if ( cluster.size() ) return; // easy one :)



}


template <int Dim>
void union_( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b, GeometrySet<Dim>& output )
{
    typedef typename SFCGAL::detail::HandleCollection<Dim>::Type HandleCollection;
    typedef typename SFCGAL::detail::BoxCollection<Dim>::Type BoxCollection;
    HandleCollection ahandles, bhandles;
    BoxCollection aboxes, bboxes;
    a.computeBoundingBoxes( ahandles, aboxes );
    b.computeBoundingBoxes( bhandles, bboxes );

    // GLOBAL
    // first step is clustering of primitives
    // for each cluster union the primitives
    // union the clusters (easy, they are disjoint, so the function union_(cluster, geomset) just appends to geomset)
    //
    // DETAIL
    // to cluster the primitives we need to build a graph
    // and extract all disjoint subgraphs
    // that's what Cluster class does in conjunction with box_intersection_d
    //
    // to union cluster primitives
    // store the first primitive
    // order the other according to their distance to the first primitive
    // union them one at a time
    //
    // to union one single primitive to a set
    // subtract all set primitives to the primitive to union
    // add the result
    
    Cluster<Dim> cluster( aboxes, bboxes );
    CGAL::box_intersection_d( aboxes.begin(), aboxes.end(),
                              bboxes.begin(), bboxes.end(),
                              cluster );
    
    typedef typename std::vector< typename Cluster<Dim>::PrimitiveHandleSet > ClusterVector;
    ClusterVector clusters( cluster.get() );

    GeometrySet<Dim> result;
    for ( typename ClusterVector::iterator cluster = clusters.begin(); cluster != clusters.end(); ++cluster ){
        // @TODO reorder primitives occording to their distance to the fist one
        // or use a r-tree for cascaded union on the cluster but it it way more complicated
        union_(*cluster, result);
    }

    GeometrySet<Dim> temp;
    post_union( result, temp );
    output.merge( temp );
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
