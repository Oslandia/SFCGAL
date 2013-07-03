/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/length.h>
#include <SFCGAL/algorithm/orientation.h>
#include <SFCGAL/algorithm/distance.h>
#include <SFCGAL/algorithm/distance3d.h>
#include <SFCGAL/algorithm/plane.h>
#include <SFCGAL/algorithm/normal.h>
#include <SFCGAL/detail/algorithm/coversPoints.h>
#include <SFCGAL/algorithm/connection.h>
#include <SFCGAL/all.h>
#include <SFCGAL/detail/tools/Log.h>
#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/Kernel.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/undirected_dfs.hpp>


using namespace SFCGAL::detail::algorithm;

namespace SFCGAL {
namespace algorithm {

// to detect unconnected interior in polygon
struct LoopDetector : public boost::dfs_visitor<> {
    LoopDetector(bool & hasLoop):_hasLoop(hasLoop){}

    template <class Edge, class Graph>
    void back_edge(Edge, const Graph&) { _hasLoop = true; }
private:
    bool & _hasLoop;
};



/**
 * @note empty geometries are valid, but the test is only performed in the interface function
 * in individual functions for implementation, an assertion !empty is present for this reason
 */

//const Validity isValid( const Coordinate & p)
//{
//    BOOST_ASSERT( !p.isEmpty() );
//    if ( !CGAL::is_finite(p.x()) || CGAL::is_finite(p.y()) ) return Validity::invalid("NaN coordinate");
//    //if ( p.x().is_inf() || p.y().is_inf() ) return Validity::invalid("infinite coordinate");
//    return Validity::valid();
//}
//
const Validity isValid( const Point & p)
{
    BOOST_ASSERT( !p.isEmpty() );(void)p;
    //return( isValid(p.coordinate() ) );
    return Validity::valid();
}

const Validity isValid( const LineString & l, const double & toleranceAbs )
{
    BOOST_ASSERT( !l.isEmpty() );
//    const size_t numPoints = l.numPoints();
//    for ( size_t p=0; p!=numPoints; ++p) {
//        const Validity v = isValid(l.pointN(p));
//        if (!v) return Validity::invalid( ( boost::format("Point %d is invalid: %s") % p % v.reason() ).str() );
//    }
    
    return length3D( l ) > toleranceAbs ? Validity::valid() : Validity::invalid("no length");
}

const Validity isValid( const Polygon & p, const double & toleranceAbs )
{
    BOOST_ASSERT( !p.isEmpty() );

    // Closed simple rings
    const size_t numRings =  p.numRings();
    for (size_t r=0; r != numRings; ++r) {
        if ( p.ringN(r).numPoints() < 4 ) {
            return Validity::invalid((boost::format("not enought points in ring %d") % r ).str() );
        }
//        const Validity v = isValid( p.ringN(r) );
//        if (!v) return Validity::invalid((boost::format("ring %d is invalid: %s") % r % v.reason()).str() );
        
        const double distanceToClose = 
            p.is3D() ? distancePointPoint3D( p.ringN(r).startPoint(), p.ringN(r).endPoint() )
                     : distancePointPoint( p.ringN(r).startPoint(), p.ringN(r).endPoint() ) 
        ;
        if ( distanceToClose > 0 ) {
            return Validity::invalid((boost::format("ring %d is not closed") % r ).str() );
        }
        if ( p.is3D() ? selfIntersects3D( p.ringN(r) ) : selfIntersects( p.ringN(r) ) ) {
            return Validity::invalid((boost::format("ring %d self intersects") % r ).str());
        }
    }

    // Orientation in 2D
    if ( !p.is3D() ) {
        // Opposit orientation for interior and exterior rings
        const bool extCCWO = isCounterClockWiseOriented( p.exteriorRing() );
        for (std::size_t r=0; r<p.numInteriorRings(); ++r) {
            if ( extCCWO == isCounterClockWiseOriented( p.interiorRingN( r ) ) ) {
                return Validity::invalid( ( boost::format("exterior ring and interior ring %d have the same orientation") % r ).str() );
            }
        }
    }
    // Orientation in 3D
    else {
        // Polygone must be planar (all points in the same plane)
        if ( !isPlane3D< Kernel >( p, toleranceAbs ) ) {
            return Validity::invalid("points don't lie in the same plane");
        }

        // interior rings must be oriented opposit to exterior;
        if ( p.hasInteriorRings() ) {
            const CGAL::Vector_3< Kernel > nExt = normal3D< Kernel >( p.exteriorRing() );
            for (std::size_t r=0; r<p.numInteriorRings(); ++r) {
                const CGAL::Vector_3< Kernel > nInt = normal3D< Kernel>( p.interiorRingN( r ) );
                if ( nExt * nInt > 0 ) {
                    return Validity::invalid( ( boost::format("interior ring %d is oriented in the same direction as exterior ring") % r ).str() );
                }
            }
        }
    }

    // Rings must not share more than one point (no intersection)
    {
        typedef std::pair<int,int> Edge;
        std::vector<Edge> touchingRings;
        for (size_t ri=0; ri < numRings; ++ri) { // no need for numRings-1, the next loop won't be entered for the last ring
            for (size_t rj=ri+1; rj < numRings; ++rj) {
                std::auto_ptr<Geometry> inter = p.is3D()
                                                ? intersection3D( p.ringN( ri ), p.ringN( rj ) )
                                                : intersection( p.ringN( ri ), p.ringN( rj ) );
                if ( ! inter->isEmpty() && ! inter->is< Point >() ) {
                    return Validity::invalid( ( boost::format("intersection between ring %d and %d") % ri % rj ).str() );
                } else if ( ! inter->isEmpty() && inter->is< Point >() ) {
                    touchingRings.push_back( Edge(ri,rj) );
                }
            }
        }
        {
            using namespace boost;
            typedef adjacency_list< vecS, vecS, undirectedS,
                    no_property,
                    property<edge_color_t, default_color_type> > Graph;
            typedef graph_traits<Graph>::vertex_descriptor vertex_t;

            Graph g(touchingRings.begin(), touchingRings.end(), numRings);
       
            bool hasLoop = false;
            LoopDetector vis(hasLoop);
            undirected_dfs(g, root_vertex(vertex_t(0)).visitor(vis).edge_color_map(get(edge_color, g)));
            if (hasLoop) {
                return Validity::invalid("interior is not connected");
            }
        }
    }

    if ( p.hasInteriorRings() ) {
        // Interior rings must be interior to exterior ring
        for (size_t r=0; r < p.numInteriorRings(); ++r) { // no need for numRings-1, the next loop won't be entered for the last ring
            if ( p.is3D()
                    ? !coversPoints3D( Polygon( p.exteriorRing() ), Polygon( p.interiorRingN( r ) ) )
                    : !coversPoints( Polygon( p.exteriorRing() ), Polygon( p.interiorRingN( r ) ) )
               ) {
                return Validity::invalid( ( boost::format("exterior ring doesn't cover interior ring %d") % r ).str() );
            }
        }

        // Interior ring must not cover one another
        for (size_t ri=0; ri < p.numInteriorRings(); ++ri) { // no need for numRings-1, the next loop won't be entered for the last ring
            for (size_t rj=ri+1; rj < p.numInteriorRings(); ++rj) {
                if ( p.is3D()
                        ? coversPoints3D( Polygon( p.interiorRingN( ri ) ), Polygon( p.interiorRingN( rj ) ) )
                        : coversPoints( Polygon( p.interiorRingN( ri ) ), Polygon( p.interiorRingN( rj ) ) )
                   ) {
                    return Validity::invalid( ( boost::format("interior ring %d covers interior ring %d") % ri % rj ).str() );
                }
            }
        }
    }
    return Validity::valid();
}

const Validity isValid( const Triangle & t, const double & toleranceAbs )
{
    BOOST_ASSERT( !t.isEmpty() );
    return isValid( t.toPolygon(), toleranceAbs );
}

const Validity isValid( const MultiLineString & ml, const double & toleranceAbs )
{
    BOOST_ASSERT( !ml.isEmpty() );
    const size_t numLineString = ml.numGeometries();
    for (size_t l = 0; l != numLineString; ++l ) {
        Validity v = isValid( ml.lineStringN(l), toleranceAbs );
        if ( !v ) return Validity::invalid(
                                 ( boost::format( "LineString %d is invalid: %s" ) % l % v.reason() ).str()
                             );
    }
    return Validity::valid();
}

const Validity isValid( const MultiPolygon & mp, const double & toleranceAbs )
{
    BOOST_ASSERT( !mp.isEmpty() );
    const size_t numPolygons = mp.numGeometries();
    for (size_t p = 0; p != numPolygons; ++p ) {
        Validity v = isValid( mp.polygonN(p), toleranceAbs );
        if ( !v ) return Validity::invalid(
                                 ( boost::format( "Polygon %d is invalid: %s" ) % p % v.reason() ).str()
                             );
    }

    for (size_t pi = 0; pi != numPolygons; ++pi ) {
        for (size_t pj = pi+1; pj < numPolygons; ++pj ) {
            std::auto_ptr< Geometry > inter = mp.is3D()
                                              ? intersection3D( mp.polygonN(pi), mp.polygonN(pj) )
                                              : intersection( mp.polygonN(pi), mp.polygonN(pj) ) ;
            // intersection can be empty, a point, or a set of points
            if ( !inter->isEmpty() && inter->dimension() != 0 ) {
                return Validity::invalid(
                           (boost::format("intersection between Polygon %d and %d") % pi % pj ).str()
                       );
            }
        }
    }

    return Validity::valid();
}

const Validity isValid( const GeometryCollection & gc, const double & toleranceAbs )
{
    BOOST_ASSERT( !gc.isEmpty() );
    const size_t numGeom = gc.numGeometries();
    for (size_t g = 0; g != numGeom; ++g ) {
        Validity v = isValid( gc.geometryN(g), toleranceAbs );
        if ( !v ) return Validity::invalid(
                                 ( boost::format( "%s %d is invalid: %s" ) % gc.geometryN(g).geometryType()  % g % v.reason() ).str()
                             );
    }
    return Validity::valid();
}

const Validity isValid( const TriangulatedSurface & tin, const SurfaceGraph & graph, const double & toleranceAbs )
{
    BOOST_ASSERT( !tin.isEmpty() );
    size_t numTriangles = tin.numTriangles();
    for ( size_t t=0; t != numTriangles; ++t ) {
        Validity v = isValid( tin.triangleN(t), toleranceAbs );
        if ( !v ) return Validity::invalid(
                                 ( boost::format( "Triangle %d is invalid: %s" ) % t % v.reason() ).str()
                             );
    }
    if ( !isConnected( graph ) ) return Validity::invalid( "not connected" );

    if ( tin.is3D() ? selfIntersects3D( tin, graph ) : selfIntersects( tin, graph ) ) return Validity::invalid( "self intersects" );
    return Validity::valid();
}

const Validity isValid( const TriangulatedSurface & tin, const double & toleranceAbs )
{
    BOOST_ASSERT( !tin.isEmpty() );
    const SurfaceGraph graph( tin );
    return graph.isValid() ? isValid( tin, graph, toleranceAbs ) : graph.isValid() ;
}

const Validity isValid( const PolyhedralSurface & s, const SurfaceGraph & graph, const double & toleranceAbs )
{
    BOOST_ASSERT( !s.isEmpty() );
    size_t numPolygons = s.numPolygons();
    for ( size_t p=0; p != numPolygons; ++p ) {
        Validity v = isValid( s.polygonN(p), toleranceAbs );
        if ( !v ) return Validity::invalid(
                                 ( boost::format( "Polygon %d is invalid: %s" ) % p % v.reason() ).str()
                             );
    }
    if ( !isConnected( graph ) ) return Validity::invalid( "not connected" );

    if ( s.is3D() ? selfIntersects3D( s, graph ) : selfIntersects( s, graph ) ) return Validity::invalid( "self intersects" );
    return Validity::valid();
}

const Validity isValid( const PolyhedralSurface & s, const double & toleranceAbs )
{
    BOOST_ASSERT( !s.isEmpty() );
    const SurfaceGraph graph( s );
    return graph.isValid() ? isValid( s, graph, toleranceAbs ) : graph.isValid() ;
}

const Validity isValid( const Solid & solid, const double & toleranceAbs )
{
    BOOST_ASSERT( !solid.isEmpty() );
    const size_t numShells = solid.numShells();
    for ( size_t s = 0; s != numShells; ++s ) {
        const SurfaceGraph graph( solid.shellN(s) );
        Validity v = isValid( solid.shellN(s), graph, toleranceAbs );
        if (!v) return Validity::invalid(
                               ( boost::format( "PolyhedralSurface (shell) %d is invalid: %s" ) % s % v.reason() ).str()
                           );
        if ( !isClosed( graph ) ) return Validity::invalid(
                                                 ( boost::format( "PolyhedralSurface (shell) %d is not closed" ) % s ).str()
                                             );
    }

    if ( solid.numInteriorShells() ) {
        BOOST_THROW_EXCEPTION(Exception("function is not fully implemented (covering and intersections of interior shells missing"));
    }

    return Validity::valid();
}

const Validity isValid( const MultiSolid & ms, const double & toleranceAbs )
{
    BOOST_ASSERT( !ms.isEmpty() );
    const size_t numMultiSolid = ms.numGeometries();
    for (size_t s = 0; s != numMultiSolid; ++s ) {
        Validity v = isValid( ms.solidN(s), toleranceAbs );
        if ( !v ) return Validity::invalid(
                                 ( boost::format( "Solid %d is invalid: %s" ) % s % v.reason() ).str()
                             );
    }
    return Validity::valid();
}


const Validity isValid( const Geometry& g, const double & toleranceAbs )
{
    if ( g.isEmpty() ) return Validity::valid();

    switch ( g.geometryTypeId() ) {
    case TYPE_POINT:
        return isValid( g.as< Point >() );
    case TYPE_LINESTRING:
        return isValid( g.as< LineString >(),          toleranceAbs ) ;
    case TYPE_POLYGON:
        return isValid( g.as< Polygon >(),             toleranceAbs ) ;
    case TYPE_TRIANGLE:
        return isValid( g.as< Triangle >(),            toleranceAbs ) ;
    case TYPE_SOLID:
        return isValid( g.as< Solid >(),               toleranceAbs ) ;
    case TYPE_MULTIPOINT:
        return Validity::valid();
    case TYPE_MULTILINESTRING:
        return isValid( g.as< MultiLineString >(),     toleranceAbs ) ;
    case TYPE_MULTIPOLYGON:
        return isValid( g.as< MultiPolygon >(),        toleranceAbs ) ;
    case TYPE_MULTISOLID:
        return isValid( g.as< MultiSolid >(),          toleranceAbs ) ;
    case TYPE_GEOMETRYCOLLECTION:
        return isValid( g.as< GeometryCollection >(),  toleranceAbs ) ;
    case TYPE_TRIANGULATEDSURFACE:
        return isValid( g.as< TriangulatedSurface >(), toleranceAbs ) ;
    case TYPE_POLYHEDRALSURFACE:
        return isValid( g.as< PolyhedralSurface >(),   toleranceAbs ) ;
    }
    BOOST_THROW_EXCEPTION(Exception(
                              ( boost::format("isValid( %s ) is not defined") % g.geometryType() ).str()
                          ));
    return Validity::invalid(( boost::format("isValid( %s ) is not defined") % g.geometryType() ).str()); // to avoid warning
}

} // namespace algorithm
} // namespace SFCGAL
