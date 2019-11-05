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


#include <SFCGAL/detail/GeometrySet.h>

#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/MultiSolid.h>
#include <SFCGAL/Envelope.h>
#include <SFCGAL/detail/TypeForDimension.h>

#include <SFCGAL/algorithm/covers.h>
#include <SFCGAL/algorithm/volume.h>
#include <SFCGAL/algorithm/connection.h>

#include <CGAL/Bbox_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>

#include <boost/graph/adjacency_list.hpp>

#include <map>

bool operator< ( const CGAL::Segment_2<SFCGAL::Kernel>& sega, const CGAL::Segment_2<SFCGAL::Kernel>& segb )
{
    if ( sega.source() == segb.source() ) {
        return sega.target() < segb.target();
    }

    return sega.source() < segb.source();
}

bool operator< ( const CGAL::Segment_3<SFCGAL::Kernel>& sega, const CGAL::Segment_3<SFCGAL::Kernel>& segb )
{
    if ( sega.source() == segb.source() ) {
        return sega.target() < segb.target();
    }

    return sega.source() < segb.source();
}

namespace SFCGAL {
namespace detail {

void _decompose_triangle( const Triangle& tri, GeometrySet<2>::SurfaceCollection& surfaces, dim_t<2> )
{
    CGAL::Polygon_2<Kernel> outer;
    outer.push_back( tri.vertex( 0 ).toPoint_2() );
    outer.push_back( tri.vertex( 1 ).toPoint_2() );
    outer.push_back( tri.vertex( 2 ).toPoint_2() );

    if ( outer.orientation() == CGAL::CLOCKWISE ) {
        outer.reverse_orientation();
    }

    surfaces.push_back( CGAL::Polygon_with_holes_2<Kernel>( outer ) );
}
void _decompose_triangle( const Triangle& tri, GeometrySet<3>::SurfaceCollection& surfaces, dim_t<3> )
{
    CGAL::Triangle_3<Kernel> outtri( tri.vertex( 0 ).toPoint_3(),
                                     tri.vertex( 1 ).toPoint_3(),
                                     tri.vertex( 2 ).toPoint_3() );
    surfaces.push_back( outtri );
}

void _decompose_polygon( const Polygon& poly, GeometrySet<2>::SurfaceCollection& surfaces, dim_t<2> )
{
    BOOST_ASSERT( ! poly.isEmpty() );
    surfaces.push_back( poly.toPolygon_with_holes_2() );
}
void _decompose_polygon( const Polygon& poly, GeometrySet<3>::SurfaceCollection& surfaces, dim_t<3> )
{
    BOOST_ASSERT( ! poly.isEmpty() );
    TriangulatedSurface surf;
    triangulate::triangulatePolygon3D( poly, surf );

    for ( size_t i = 0; i < surf.numTriangles(); ++i ) {
        const Triangle& tri = surf.triangleN( i );
        surfaces.push_back( CGAL::Triangle_3<Kernel>( tri.vertex( 0 ).toPoint_3(),
                            tri.vertex( 1 ).toPoint_3(),
                            tri.vertex( 2 ).toPoint_3() )
                          );
    }
}

void _decompose_solid( const Solid&, GeometrySet<2>::VolumeCollection&, dim_t<2> )
{
}
void _decompose_solid( const Solid& solid, GeometrySet<3>::VolumeCollection& volumes, dim_t<3> )
{
    BOOST_ASSERT( ! solid.isEmpty() );
    // volume orientation test
    // TODO: simplfiy ?
    MarkedPolyhedron p = *solid.exteriorShell().toPolyhedron_3<Kernel, MarkedPolyhedron >();

    if ( algorithm::volume( solid ) < 0 ) {
        // if the volume is "inverted", we reverse it
        // TODO: Once every boolean operations work with complement geometries, we may want to keep the solid inverted
        p.inside_out();
    }

    volumes.push_back( p );
}

template <int Dim>
GeometrySet<Dim>::GeometrySet( )
{
}

template <int Dim>
GeometrySet<Dim>::GeometrySet( const Geometry& g )
{
    _decompose( g );
}

template <int Dim>
GeometrySet<Dim>::GeometrySet( const typename TypeForDimension<Dim>::Point& g, int /*flags*/ )
{
    addPrimitive( g );
}

template <int Dim>
GeometrySet<Dim>::GeometrySet( const typename TypeForDimension<Dim>::Segment& g, int /*flags*/ )
{
    addPrimitive( g );
}

template <int Dim>
GeometrySet<Dim>::GeometrySet( const typename TypeForDimension<Dim>::Surface& g, int /*flags*/ )
{
    addPrimitive( g );
}

template <int Dim>
GeometrySet<Dim>::GeometrySet( const typename TypeForDimension<Dim>::Volume& g, int /*flags*/ )
{
    addPrimitive( g );
}

template <int Dim>
void GeometrySet<Dim>::merge( const GeometrySet<Dim>& g )
{
    std::copy( g.points().begin(), g.points().end(), std::inserter( points(), points().end() ) );
    std::copy( g.segments().begin(), g.segments().end(), std::inserter( segments(), segments().end() ) );
    std::copy( g.surfaces().begin(), g.surfaces().end(), std::back_inserter( surfaces() ) );
    std::copy( g.volumes().begin(), g.volumes().end(), std::back_inserter( volumes() ) );
}

template <int Dim>
void GeometrySet<Dim>::addGeometry( const Geometry& g )
{
    _decompose( g );
}

template <>
void GeometrySet<2>::addPrimitive( const PrimitiveHandle<2>& p )
{
    switch ( p.handle.which() ) {
    case PrimitivePoint:
        _points.insert( *boost::get<const TypeForDimension<2>::Point*>( p.handle ) );
        break;

    case PrimitiveSegment:
        _segments.insert( *boost::get<const TypeForDimension<2>::Segment*>( p.handle ) );
        break;

    case PrimitiveSurface:
        _surfaces.push_back( *boost::get<const TypeForDimension<2>::Surface*>( p.handle ) );
        break;

    default:
        break;
    }
}

template <>
void GeometrySet<3>::addPrimitive( const PrimitiveHandle<3>& p )
{
    switch ( p.handle.which() ) {
    case PrimitivePoint:
        _points.insert( *boost::get<const TypeForDimension<3>::Point*>( p.handle ) );
        break;

    case PrimitiveSegment:
        _segments.insert( *boost::get<const TypeForDimension<3>::Segment*>( p.handle ) );
        break;

    case PrimitiveSurface:
        _surfaces.push_back( *boost::get<const TypeForDimension<3>::Surface*>( p.handle ) );
        break;

    case PrimitiveVolume: {
        const TypeForDimension<3>::Volume& vol = *boost::get<const TypeForDimension<3>::Volume*>( p.handle );
        BOOST_ASSERT( !vol.empty() );
        _volumes.push_back( vol );
        break;
    }
    }
}

template <>
void GeometrySet<3>::addPrimitive( const CGAL::Object& o, bool pointsAsRing )
{
    typedef TypeForDimension<3>::Point TPoint;
    typedef TypeForDimension<3>::Segment TSegment;
    typedef TypeForDimension<3>::Surface TSurface;
    typedef TypeForDimension<3>::Volume TVolume;

    if ( const TPoint* p = CGAL::object_cast<TPoint>( &o ) ) {
        _points.insert( TPoint( *p ) );
    }
    else if ( const std::vector<TPoint>* pts = CGAL::object_cast<std::vector<TPoint> >( &o ) ) {
        if ( pointsAsRing ) {
            // if pointsAsRing is true, build a polygon out of points
            // FIXME : we use triangulation here, which is not needed
            // We should have created a (planar) Polyhedron directly out of the points
            LineString ls;

            for ( size_t i = 0; i < pts->size(); ++i ) {
                ls.addPoint( ( *pts )[i] );
            }

            // close the ring
            ls.addPoint( ( *pts )[0] );
            Polygon poly( ls );
            _decompose_polygon( poly, _surfaces, dim_t<3>() );
        }
        else {
            std::copy( pts->begin(), pts->end(), std::inserter( _points, _points.end() ) );
        }
    }
    else if ( const TSegment* p = CGAL::object_cast<TSegment>( &o ) ) {
        _segments.insert( TSegment( *p ) );
    }
    else if ( const TSurface* p = CGAL::object_cast<TSurface>( &o ) ) {
        _surfaces.push_back( TSurface( *p ) );
    }
    else if ( const TVolume* p = CGAL::object_cast<TVolume>( &o ) ) {
        BOOST_ASSERT( ! p->empty() );
        _volumes.push_back( TVolume( *p ) );
    }
}

template <>
void GeometrySet<2>::addPrimitive( const CGAL::Object& o, bool pointsAsRing )
{
    typedef TypeForDimension<2>::Point TPoint;
    typedef TypeForDimension<2>::Segment TSegment;
    typedef TypeForDimension<2>::Surface TSurface;
    typedef TypeForDimension<2>::Volume TVolume;

    if ( const TPoint* p = CGAL::object_cast<TPoint>( &o ) ) {
        _points.insert( TPoint( *p ) );
    }
    else if ( const std::vector<TPoint>* pts = CGAL::object_cast<std::vector<TPoint> >( &o ) ) {
        if ( pointsAsRing ) {
            // if pointsAsRing is true, build a polygon out of points
            CGAL::Polygon_2<Kernel> poly;

            for ( size_t i = 0; i < pts->size(); ++i ) {
                poly.push_back( ( *pts )[i] );
            }

            CGAL::Polygon_with_holes_2<Kernel> polyh( poly );
            _surfaces.push_back( polyh );
        }
        else {
            std::copy( pts->begin(), pts->end(), std::inserter( _points, _points.end() ) );
        }
    }
    else if ( const CGAL::Triangle_2<Kernel>* tri = CGAL::object_cast<CGAL::Triangle_2<Kernel> >( &o ) ) {
        // convert to a polygon
        CGAL::Polygon_2<Kernel> poly;
        poly.push_back( tri->vertex( 0 ) );
        poly.push_back( tri->vertex( 1 ) );
        poly.push_back( tri->vertex( 2 ) );
        CGAL::Polygon_with_holes_2<Kernel> polyh( poly );
        _surfaces.push_back( polyh );
    }
    else if ( const TSegment* p = CGAL::object_cast<TSegment>( &o ) ) {
        _segments.insert( TSegment( *p ) );
    }
    else if ( const TSurface* p = CGAL::object_cast<TSurface>( &o ) ) {
        BOOST_ASSERT( ! p->is_unbounded() );
        _surfaces.push_back( TSurface( *p ) );
    }
    else if ( const TVolume* p = CGAL::object_cast<TVolume>( &o ) ) {
        _volumes.push_back( TVolume( *p ) );
    }
}

template <int Dim>
void GeometrySet<Dim>::addPrimitive( const typename TypeForDimension<Dim>::Point& p, int flags )
{
    _points.insert( CollectionElement<typename Point_d<Dim>::Type>( p, flags ) );
}

template <int Dim>
void GeometrySet<Dim>::addPrimitive( const typename TypeForDimension<Dim>::Segment& p, int flags )
{
    _segments.insert( CollectionElement<typename Segment_d<Dim>::Type>( p, flags ) );
}

template <>
void GeometrySet<2>::addPrimitive( const TypeForDimension<2>::Surface& p, int flags )
{
    BOOST_ASSERT( ! p.is_unbounded() );
    _surfaces.push_back( p );
    _surfaces.back().setFlags( flags );
}
template <>
void GeometrySet<3>::addPrimitive( const TypeForDimension<3>::Surface& p, int flags )
{
    _surfaces.push_back( p );
    _surfaces.back().setFlags( flags );
}

template <>
void GeometrySet<2>::addPrimitive( const TypeForDimension<2>::Volume&, int )
{
}

template <>
void GeometrySet<3>::addPrimitive( const TypeForDimension<3>::Volume& p, int flags )
{
    BOOST_ASSERT( ! p.empty() );

    if ( p.is_closed() ) {
        _volumes.push_back( GeometrySet<3>::VolumeCollection::value_type( p, flags ) );
    }
    else {
        // it is an unclosed volume, i.e. a surface
        BOOST_ASSERT( p.is_pure_triangle() );
        CGAL::Point_3<Kernel> p1, p2, p3;

        for ( MarkedPolyhedron::Facet_const_iterator fit = p.facets_begin();
                fit != p.facets_end();
                ++fit ) {
            MarkedPolyhedron::Halfedge_around_facet_const_circulator cit = fit->facet_begin();
            p1 = cit->vertex()->point();
            cit++;
            p2 = cit->vertex()->point();
            cit++;
            p3 = cit->vertex()->point();
            CGAL::Triangle_3<Kernel> tri( p1, p2, p3 );
            _surfaces.push_back( tri );
        }
    }
}

template <int Dim>
bool GeometrySet<Dim>::hasPoints() const
{
    return ! points().empty();
}

template <int Dim>
bool GeometrySet<Dim>::hasSegments() const
{
    return ! segments().empty();
}

template <>
bool GeometrySet<2>::hasSurfaces() const
{
    return ! surfaces().empty();
}
template <>
bool GeometrySet<3>::hasSurfaces() const
{
    if ( ! surfaces().empty() ) {
        return true;
    }

    if ( ! volumes().empty() ) {
        for ( VolumeCollection::const_iterator it = _volumes.begin(); it != _volumes.end(); ++it ) {
            if ( ! it->primitive().is_closed() ) {
                return true;
            }
        }
    }

    return false;
}

template <>
bool GeometrySet<2>::hasVolumes() const
{
    return false;
}
template <>
bool GeometrySet<3>::hasVolumes() const
{
    if ( ! volumes().empty() ) {
        return true;
    }

    if ( ! volumes().empty() ) {
        for ( VolumeCollection::const_iterator it = _volumes.begin(); it != _volumes.end(); ++it ) {
            if ( it->primitive().is_closed() ) {
                return true;
            }
        }
    }

    return false;
}
template <int Dim>
void GeometrySet<Dim>::_decompose( const Geometry& g )
{
    if ( g.isEmpty() ) {
        return;
    }

    if ( g.is<GeometryCollection>() ) {
        const GeometryCollection& collect = g.as<GeometryCollection>();

        for ( size_t i = 0; i < g.numGeometries(); ++i ) {
            _decompose( collect.geometryN( i ) );
        }

        return;
    }

    switch ( g.geometryTypeId() ) {
    case TYPE_POINT:
        _points.insert( g.as<Point>().toPoint_d<Dim>() );
        break;

    case TYPE_LINESTRING: {
        const LineString& ls = g.as<LineString>();

        for ( size_t i = 0; i < ls.numPoints() - 1; ++i ) {
            typename TypeForDimension<Dim>::Segment seg( ls.pointN( i ).toPoint_d<Dim>(),
                    ls.pointN( i+1 ).toPoint_d<Dim>() );
            _segments.insert( seg );
        }

        break;
    }

    case TYPE_TRIANGLE: {
        _decompose_triangle( g.as<Triangle>(), _surfaces, dim_t<Dim>() );
        break;
    }

    case TYPE_POLYGON: {
        _decompose_polygon( g.as<Polygon>(), _surfaces, dim_t<Dim>() );
        break;
    }

    case TYPE_TRIANGULATEDSURFACE: {
        const TriangulatedSurface& tri = g.as<TriangulatedSurface>();

        for ( size_t i = 0; i < tri.numTriangles(); ++i ) {
            _decompose( tri.triangleN( i ) );
        }

        break;
    }

    case TYPE_POLYHEDRALSURFACE: {
        const PolyhedralSurface& tri = g.as<PolyhedralSurface>();

        for ( size_t i = 0; i < tri.numPolygons(); ++i ) {
            _decompose( tri.polygonN( i ) );
        }

        break;
    }

    case TYPE_SOLID: {
        const Solid& solid = g.as<Solid>();
        _decompose_solid( solid, _volumes, dim_t<Dim>() );
        break;
    }

    default:
        break;
    }
}


template <int Dim>
void GeometrySet<Dim>::computeBoundingBoxes( typename HandleCollection<Dim>::Type& handles,
        typename BoxCollection<Dim>::Type& boxes ) const
{
    boxes.clear();

    for ( typename PointCollection::const_iterator it = _points.begin(); it != _points.end(); ++it ) {
        const typename TypeForDimension<Dim>::Point* pt = &( it->primitive() );
        PrimitiveHandle<Dim> h( pt );
        handles.push_back( h );
        boxes.push_back( typename PrimitiveBox<Dim>::Type( it->primitive().bbox(), &handles.back() ) );
    }

    for ( typename SegmentCollection::const_iterator it = _segments.begin(); it != _segments.end(); ++it ) {
        handles.push_back( PrimitiveHandle<Dim>( &( it->primitive() ) ) );
        boxes.push_back( typename PrimitiveBox<Dim>::Type( it->primitive().bbox(), &handles.back() ) );
    }

    for ( typename SurfaceCollection::const_iterator it = _surfaces.begin(); it != _surfaces.end(); ++it ) {
        handles.push_back( PrimitiveHandle<Dim>( &( it->primitive() ) ) );
        boxes.push_back( typename PrimitiveBox<Dim>::Type( it->primitive().bbox(), &handles.back() ) );
    }

    for ( typename VolumeCollection::const_iterator it = _volumes.begin(); it != _volumes.end(); ++it ) {
        handles.push_back( PrimitiveHandle<Dim>( &( it->primitive() ) ) );
        boxes.push_back( typename PrimitiveBox<Dim>::Type( compute_solid_bbox( it->primitive(), dim_t<Dim>() ),
                         &handles.back() ) );
    }
}

template <int Dim>
void recompose_points( const typename GeometrySet<Dim>::PointCollection& points,
                       std::vector<Geometry*>& rpoints,
                       dim_t<Dim> )
{
    if ( points.empty() ) {
        return;
        //			rpoints.push_back( new Point() );
    }
    else {
        for ( typename GeometrySet<Dim>::PointCollection::const_iterator it = points.begin();
                it != points.end();
                ++it ) {
            rpoints.push_back( new Point( it->primitive() ) );
        }
    }
}

// compare less than
struct ComparePoints {
    bool operator()( const CGAL::Point_2<Kernel>& lhs, const CGAL::Point_2<Kernel>& rhs ) const {
        return lhs.x() == rhs.x() ? lhs.y() < rhs.y() : lhs.x() < rhs.x();
    }
    bool operator()( const CGAL::Point_3<Kernel>& lhs, const CGAL::Point_3<Kernel>& rhs ) const {
        return lhs.x() == rhs.x() ? (lhs.y() == rhs.y() ? lhs.z() < rhs.z() : lhs.y() < rhs.y()) : lhs.x() < rhs.x();
    }
};


template <int Dim>
void recompose_segments( const typename GeometrySet<Dim>::SegmentCollection& segments,
                         std::vector<Geometry*>& lines,
                         dim_t<Dim> )
{
    if ( segments.empty() ) {
        //			lines.push_back( new LineString );
        return;
    }

    // what we need is a graph, we do a depth first traversal and stop a linestring
    // when more than one segment is connected
    // first we need to label vertices
    // then build the graph and traverse depth first
    std::vector<Point> points;
    typedef std::pair<int,int> Edge;
    std::vector<Edge> edges;
    {
        typedef typename std::map< typename TypeForDimension<Dim>::Point, int, ComparePoints > PointMap;
        PointMap pointMap;

        for ( typename GeometrySet<Dim>::SegmentCollection::const_iterator it = segments.begin();
                it != segments.end();
                ++it ) {
            const typename PointMap::const_iterator foundSource = pointMap.find( it->primitive().source() );
            const typename PointMap::const_iterator foundTarget = pointMap.find( it->primitive().target() );
            const int sourceId = foundSource != pointMap.end() ? foundSource->second : points.size();

            if ( foundSource == pointMap.end() ) {
                points.push_back( it->primitive().source() );
                pointMap[ it->primitive().source() ] = sourceId;
            }

            const int targetId = foundTarget != pointMap.end() ? foundTarget->second : points.size();

            if ( foundTarget == pointMap.end() ) {
                points.push_back( it->primitive().target() );
                pointMap[ it->primitive().target() ] = targetId;
            }

            edges.push_back( Edge( sourceId, targetId ) );
        }
    }

    typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::bidirectionalS,
            boost::no_property,
            boost::property<boost::edge_color_t, boost::default_color_type> > Graph;
    Graph g( edges.begin(), edges.end(), edges.size() );

    // now we find all branches without bifurcations,

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;

    for ( boost::tie( ei, ei_end ) = boost::edges( g ); ei != ei_end; ++ei ) {
        if ( boost::get( boost::edge_color, g )[*ei] == boost::white_color ) {
            // not already marked, find the first ancestor with multiple connections, or no connections
            // or self (in case of a loop)
            boost::graph_traits<Graph>::edge_descriptor root = *ei;
            {
                boost::graph_traits<Graph>::in_edge_iterator ej, ek;

                for ( boost::tie( ej, ek ) = boost::in_edges( boost::source( root, g ), g );
                        ek - ej == 1 && *ej != *ei ;
                        boost::tie( ej, ek ) = boost::in_edges( boost::source( root, g ), g ) ) {
                    root = *ej;
                }
            }

            // now we go down
            LineString* line = new LineString;
            lines.push_back( line );
            line->addPoint( points[ boost::source( root, g ) ] );
            line->addPoint( points[ boost::target( root, g ) ] );
            boost::get( boost::edge_color, g )[root] = boost::black_color;

            boost::graph_traits<Graph>::out_edge_iterator ej, ek;

            for ( boost::tie( ej, ek ) = boost::out_edges( boost::target( root, g ), g );
                    ek - ej == 1 && *ej != root;
                    boost::tie( ej, ek ) = boost::out_edges( boost::target( *ej, g ), g ) ) {
                line->addPoint( points[ boost::target( *ej, g ) ] );
                boost::get( boost::edge_color, g )[*ej] = boost::black_color;
            }
        }
    }
}

void recompose_surfaces( const GeometrySet<2>::SurfaceCollection& surfaces, std::vector<Geometry*>& output, dim_t<2> )
{
    for ( GeometrySet<2>::SurfaceCollection::const_iterator it = surfaces.begin(); it != surfaces.end(); ++it ) {
        if ( it->primitive().holes_begin() == it->primitive().holes_end() &&
                it->primitive().outer_boundary().size() == 3 ) {
            CGAL::Polygon_2<Kernel>::Vertex_iterator vit = it->primitive().outer_boundary().vertices_begin();
            CGAL::Point_2<Kernel> p1( *vit++ );
            CGAL::Point_2<Kernel> p2( *vit++ );
            CGAL::Point_2<Kernel> p3( *vit++ );
            output.push_back( new Triangle( CGAL::Triangle_2<Kernel>( p1, p2, p3 ) ) );
        }
        else {
            output.push_back( new Polygon( it->primitive() ) );
        }
    }
}

void recompose_surfaces( const GeometrySet<3>::SurfaceCollection& surfaces, std::vector<Geometry*>& output, dim_t<3> )
{
    if ( surfaces.empty() ) {
        return;
    }

    // TODO : regroup triangles of the same mesh
    if ( surfaces.size() == 1 ) {
        output.push_back( new Triangle( surfaces.begin()->primitive() ) );
        return;
    }


    std::unique_ptr<TriangulatedSurface> tri( new TriangulatedSurface );

    for ( GeometrySet<3>::SurfaceCollection::const_iterator it = surfaces.begin(); it != surfaces.end(); ++it ) {
        tri->addTriangle( new Triangle( it->primitive() ) );
    }

    algorithm::SurfaceGraph graph( *tri );
    std::vector< size_t > component( boost::num_vertices( graph.faceGraph() ) );
    BOOST_ASSERT( tri->numTriangles() == component.size() );
    const size_t numComponents = boost::connected_components( graph.faceGraph(), &component[0] );

    if ( 1 == numComponents ) {
        output.push_back( tri.release() );
    }
    else {
        std::vector< TriangulatedSurface* > sout( numComponents );

        for ( unsigned c = 0; c < numComponents; c++ ) {
            sout[c] = new TriangulatedSurface;
            output.push_back( sout[c] );
        }

        const size_t numTriangles = tri->numTriangles() ;

        for ( size_t t = 0; t != numTriangles; ++t ) {
            sout[ component[t] ]->addTriangle( tri->triangleN( t ) );
        }
    }
}

void recompose_volumes( const GeometrySet<2>::VolumeCollection&, std::vector<Geometry*>&, dim_t<2> )
{
}

void recompose_volumes( const GeometrySet<3>::VolumeCollection& volumes, std::vector<Geometry*>& output, dim_t<3> )
{
    if ( volumes.empty() ) {
        return;
    }

    for ( GeometrySet<3>::VolumeCollection::const_iterator vit = volumes.begin(); vit != volumes.end(); ++vit ) {
        if ( vit->flags() & FLAG_IS_PLANAR ) {
            // extract the boundary
            std::list<CGAL::Point_3<Kernel> > boundary;

            for ( MarkedPolyhedron::Halfedge_const_iterator it = vit->primitive().halfedges_begin();
                    it != vit->primitive().halfedges_end();
                    ++it ) {
                if ( !it->is_border() ) {
                    continue;
                }

                CGAL::Point_3<Kernel> p1 = it->prev()->vertex()->point();
                CGAL::Point_3<Kernel> p2 = it->vertex()->point();

                // TODO: test for colinearity
                // Temporary vertice may have been introduced during triangulations
                // and since we expect here a planar surface, it is safe to simplify the boundary
                // by eliminating collinear points.
                if ( boundary.size() == 0 ) {
                    boundary.push_back( p1 );
                    boundary.push_back( p2 );
                }
                else if ( boundary.back() == p1 ) {
                    boundary.push_back( p2 );
                }
                else if ( boundary.front() == p2 ) {
                    boundary.push_front( p1 );
                }
            }

            if ( boundary.size() == 3 ) {
                // It is a triangle

                Point p[3];
                std::list<CGAL::Point_3<Kernel> >::const_iterator it = boundary.begin();

                for ( size_t i = 0; i < 3; ++i, ++it ) {
                    p[i] = *it;
                }

                output.push_back( new Triangle( p[0], p[1], p[2] ) );
            }
            else {
                // Else it is a polygon
                LineString* ls = new LineString;

                for ( std::list<CGAL::Point_3<Kernel> >::const_iterator it = boundary.begin(); it != boundary.end(); ++it ) {
                    ls->addPoint( *it );
                }

                output.push_back( new Polygon( ls ) );
            }
        }
        else {

            PolyhedralSurface* shell = new PolyhedralSurface( vit->primitive() );
            // TODO: test open / closed
            output.push_back( new Solid( shell ) );
        }
    }
}

template <int Dim>
std::unique_ptr<Geometry> GeometrySet<Dim>::recompose() const
{
    std::vector<Geometry*> geometries;

    recompose_points( _points, geometries, dim_t<Dim>() );
    recompose_segments( _segments, geometries, dim_t<Dim>() );
    recompose_surfaces( _surfaces, geometries, dim_t<Dim>() );
    recompose_volumes( _volumes, geometries, dim_t<Dim>() );

    if ( geometries.empty() ) {
        return std::unique_ptr<Geometry>( new GeometryCollection );
    }

    if ( geometries.size() == 1 ) {
        return std::unique_ptr<Geometry>( geometries[0] );
    }

    // else we have a mix of different types
    bool hasCommonType = true;
    int commonType = geometries[0]->geometryTypeId();

    for ( size_t i = 0; i < geometries.size(); ++i ) {
        if ( geometries[i]->geometryTypeId() != commonType ) {
            hasCommonType = false;
            break;
        }
    }

    GeometryCollection* ret = 0;

    if ( hasCommonType ) {
        if ( commonType == TYPE_POINT ) {
            ret = new MultiPoint;
        }
        else if ( commonType == TYPE_LINESTRING ) {
            ret = new MultiLineString;
        }
        else if ( commonType == TYPE_POLYGON ) {
            ret = new MultiPolygon;
        }
        else if ( commonType == TYPE_SOLID ) {
            ret = new MultiSolid;
        }
        else {
            // one common type, but no MULTI equivalent
            ret = new GeometryCollection;
        }
    }
    else {
        ret = new GeometryCollection;
    }

    BOOST_ASSERT( ret != 0 );

    for ( size_t i = 0; i < geometries.size(); ++i ) {
        ret->addGeometry( geometries[i] );
    }

    return std::unique_ptr<Geometry>( ret );
}

void _collect_points( const CGAL::Polygon_with_holes_2<Kernel>& poly, GeometrySet<2>::PointCollection& points )
{
    for ( CGAL::Polygon_2<Kernel>::Vertex_iterator vit = poly.outer_boundary().vertices_begin();
            vit != poly.outer_boundary().vertices_end();
            ++vit ) {
        points.insert( *vit );
    }

    for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator hit = poly.holes_begin();
            hit != poly.holes_end();
            ++hit ) {
        for ( CGAL::Polygon_2<Kernel>::Vertex_iterator vit = hit->vertices_begin();
                vit != hit->vertices_end();
                ++vit ) {
            points.insert( *vit );
        }
    }
}

void _collect_points( const CGAL::Triangle_3<Kernel>& tri, GeometrySet<3>::PointCollection& points )
{
    points.insert( tri.vertex( 0 ) );
    points.insert( tri.vertex( 1 ) );
    points.insert( tri.vertex( 2 ) );
}

void _collect_points( const NoVolume&, GeometrySet<2>::PointCollection& )
{
}

void _collect_points( const MarkedPolyhedron& poly, GeometrySet<3>::PointCollection& points )
{
    for ( MarkedPolyhedron::Vertex_const_iterator vit = poly.vertices_begin();
            vit != poly.vertices_end();
            ++vit ) {
        points.insert( vit->point() );
    }
}

template <int Dim>
void GeometrySet<Dim>::collectPoints( const PrimitiveHandle<Dim>& pa )
{
    typedef typename TypeForDimension<Dim>::Point TPoint;
    typedef typename TypeForDimension<Dim>::Segment TSegment;
    typedef typename TypeForDimension<Dim>::Surface TSurface;
    typedef typename TypeForDimension<Dim>::Volume TVolume;

    switch ( pa.handle.which() ) {
    case PrimitivePoint: {
        const TPoint* pt = boost::get<const TPoint*>( pa.handle );
        _points.insert( *pt );
        break;
    }

    case PrimitiveSegment: {
        const TSegment* seg = boost::get<const TSegment*>( pa.handle );
        _points.insert( seg->source() );
        _points.insert( seg->target() );
        break;
    }

    case PrimitiveSurface: {
        _collect_points( *boost::get<const TSurface*>( pa.handle ), _points );
        break;
    }

    case PrimitiveVolume: {
        _collect_points( *boost::get<const TVolume*>( pa.handle ), _points );
        break;
    }
    }
}

template <int Dim, class IT>
void _filter_covered( IT ibegin, IT iend, GeometrySet<Dim>& output )
{
    for ( IT it = ibegin; it != iend; ++it ) {
        GeometrySet<Dim> v1;
        v1.addPrimitive( it->primitive() );
        bool v1_covered = false;

        for ( IT it2 = it; it2 != iend; ++it2 ) {
            if ( it == it2 ) {
                continue;
            }

            GeometrySet<Dim> v2;
            v2.addPrimitive( it2->primitive() );

            if ( algorithm::covers( v2, v1 ) ) {
                v1_covered = true;
                break;
            }
        }

        // if its not covered by another primitive
        if ( !v1_covered ) {
            // and not covered by another already inserted primitive
            bool b = algorithm::covers( output, v1 );

            if ( !b ) {
                output.addPrimitive( it->primitive(), it->flags() );
            }
        }
    }
}

template <>
void GeometrySet<2>::addBoundary( const TypeForDimension<2>::Surface& surface )
{
    addSegments( surface.outer_boundary().edges_begin(), surface.outer_boundary().edges_end() );

    for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator hit = surface.holes_begin();
            hit != surface.holes_end();
            ++hit ) {
        addSegments( hit->edges_begin(), hit->edges_end() );
    }
}

template <>
void GeometrySet<3>::addBoundary( const TypeForDimension<3>::Surface& )
{
    // TODO
}


template <>
int GeometrySet<2>::dimension() const
{
    if ( ! surfaces().empty() ) {
        return 2;
    }

    if ( ! segments().empty() ) {
        return 1;
    }

    if ( ! points().empty() ) {
        return 0;
    }

    return -1;
}

template <>
int GeometrySet<3>::dimension() const
{
    if ( ! volumes().empty() ) {
        for ( GeometrySet<3>::VolumeCollection::const_iterator it = volumes().begin();
                it != volumes().end();
                ++it ) {
            if ( it->primitive().is_closed() ) {
                return 3;
            }
        }

        return 2;
    }

    if ( ! surfaces().empty() ) {
        return 2;
    }

    if ( ! segments().empty() ) {
        return 1;
    }

    if ( ! points().empty() ) {
        return 0;
    }

    return -1;
}

template <int Dim>
void GeometrySet<Dim>::filterCovered( GeometrySet<Dim>& output ) const
{
    _filter_covered( _volumes.begin(), _volumes.end(), output );
    _filter_covered( _surfaces.begin(), _surfaces.end(), output );
    _filter_covered( _segments.begin(), _segments.end(), output );
    _filter_covered( _points.begin(), _points.end(), output );
}

std::ostream& operator<<( std::ostream& ostr, const GeometrySet<2>& g )
{
    ostr << "points: ";
    std::ostream_iterator<CollectionElement<Point_d<2>::Type> > out_pt ( ostr,", " );
    std::copy( g.points().begin(), g.points().end(), out_pt );
    ostr << std::endl << "segments: ";
    std::ostream_iterator<CollectionElement<Segment_d<2>::Type> > out_seg ( ostr,", " );
    std::copy( g.segments().begin(), g.segments().end(), out_seg );
    ostr << std::endl << "surfaces: ";
    std::ostream_iterator<CollectionElement<Surface_d<2>::Type> > out_surf ( ostr,", " );
    std::copy( g.surfaces().begin(), g.surfaces().end(), out_surf );
    ostr << std::endl;
    return ostr;
}

std::ostream& operator<<( std::ostream& ostr, const GeometrySet<3>& g )
{
    ostr << "points: ";
    std::ostream_iterator<CollectionElement<Point_d<3>::Type> > out_pt ( ostr,", " );
    std::copy( g.points().begin(), g.points().end(), out_pt );
    ostr << std::endl << "segments: ";
    std::ostream_iterator<CollectionElement<Segment_d<3>::Type> > out_seg ( ostr,", " );
    std::copy( g.segments().begin(), g.segments().end(), out_seg );
    ostr << std::endl << "surfaces: ";
    std::ostream_iterator<CollectionElement<Surface_d<3>::Type> > out_surf ( ostr,", " );
    std::copy( g.surfaces().begin(), g.surfaces().end(), out_surf );
    ostr << std::endl << "volumes: ";
    std::ostream_iterator<CollectionElement<Volume_d<3>::Type> > out_vol ( ostr,", " );
    std::copy( g.volumes().begin(), g.volumes().end(), out_vol );
    ostr << std::endl;
    return ostr;
}

template class GeometrySet<2>;
template class GeometrySet<3>;
} // detail
} // SFCGAL
