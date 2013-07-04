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
#include <SFCGAL/algorithm/convexHull.h>
#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/all.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Exception.h>
#include <boost/format.hpp>

#include <CGAL/point_generators_3.h>
#include <CGAL/algorithm.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/convex_hull_3.h>
#include <vector>

namespace SFCGAL {
namespace algorithm {

typedef CGAL::Point_3< Kernel >                              Point_3;
typedef CGAL::Segment_3< Kernel >                            Segment_3;
typedef CGAL::Triangle_3< Kernel >                           Triangle_3;
typedef CGAL::Polyhedron_3< Kernel >                         Polyhedron_3;

typedef CGAL::Point_2< Kernel >                              Point_2;

///
///
///
std::auto_ptr<Geometry> convexHull( const Geometry& g )
{
    using CGAL::object_cast ;

    if ( g.isEmpty() ) {
        return std::auto_ptr<Geometry>( g.clone() );
    }

    SFCGAL::detail::GetPointsVisitor getPointVisitor;
    const_cast< Geometry& >( g ).accept( getPointVisitor );

    // collect points

    if ( getPointVisitor.points.size() == 0 ) {
        return std::auto_ptr<Geometry>( new GeometryCollection() );
    }

    std::vector< Point_2 > points ;

    for ( size_t i = 0; i < getPointVisitor.points.size(); i++ ) {
        points.push_back( getPointVisitor.points[i]->toPoint_2() );
    }

    // resulting extreme points
    std::list<Point_2> epoints;
    CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter( epoints ) ) ;

    if ( epoints.size() == 1 ) {
        return std::auto_ptr<Geometry>( new Point( *epoints.begin() ) );
    }
    else if ( epoints.size() == 2 ) {
        std::list<Point_2>::const_iterator it = epoints.begin();
        return std::auto_ptr<Geometry>( new LineString( Point( *it++ ), Point( *it++ ) ) );
    }
    // GEOS does not seem to return triangles
    else if ( epoints.size() == 3 ) {
        std::list<Point_2>::const_iterator it = epoints.begin();
        Point_2 p( *it++ );
        Point_2 q( *it++ );
        Point_2 r( *it++ );
        return std::auto_ptr<Geometry>( new Triangle( p, q, r ) ) ;
    }
    else if ( epoints.size() > 3 ) {
        Polygon* poly = new Polygon;

        for ( std::list<Point_2>::const_iterator it = epoints.begin(); it != epoints.end(); ++it ) {
            poly->exteriorRing().addPoint( *it );
        }

        // add back the first point to close the ring
        poly->exteriorRing().addPoint( *epoints.begin() );
        return std::auto_ptr<Geometry>( poly );
    }
    else {
        BOOST_THROW_EXCEPTION( Exception( "unexpected CGAL output type in CGAL::convex_hull_2" ) );
    }
}

///
///
///
std::auto_ptr<Geometry> convexHull3D( const Geometry& g )
{
    using CGAL::object_cast ;

    SFCGAL::detail::GetPointsVisitor getPointVisitor;
    const_cast< Geometry& >( g ).accept( getPointVisitor );

    // collect points

    std::vector< Point_3 > points ;

    for ( size_t i = 0; i < getPointVisitor.points.size(); i++ ) {
        points.push_back( getPointVisitor.points[i]->toPoint_3() );
    }

    /*
     * http://www.cgal.org/Manual/latest/doc_html/cgal_manual/Convex_hull_3/Chapter_main.html
     *
     * handles all degenerate cases and returns a CGAL::Object,
     * which may be a point, a segment, a triangle, or a polyhedron.
     */
    CGAL::Object hull;
    CGAL::convex_hull_3( points.begin(), points.end(), hull ) ;

    if ( hull.empty() ) {
        return std::auto_ptr<Geometry>( new GeometryCollection() );
    }
    else if ( const Point_3* point = object_cast< Point_3 >( &hull ) ) {
        return std::auto_ptr<Geometry>( new Point( *point ) );
    }
    else if ( const Segment_3* segment = object_cast< Segment_3 >( &hull ) ) {
        return std::auto_ptr<Geometry>( new LineString( Point( segment->start() ), Point( segment->end() ) ) );
    }
    else if ( const Triangle_3* triangle = object_cast< Triangle_3 >( &hull ) ) {
        return std::auto_ptr<Geometry>( new Triangle(
                                            Point( triangle->vertex( 0 ) ),
                                            Point( triangle->vertex( 1 ) ),
                                            Point( triangle->vertex( 2 ) )
                                        ) );
    }
    else if ( const Polyhedron_3* polyhedron = object_cast< Polyhedron_3 >( &hull ) ) {
        std::auto_ptr< PolyhedralSurface > result( new PolyhedralSurface() );

        for ( Polyhedron_3::Facet_const_iterator it_facet = polyhedron->facets_begin();
                it_facet != polyhedron->facets_end(); ++it_facet ) {
            Polyhedron_3::Halfedge_around_facet_const_circulator it = it_facet->facet_begin();

            std::vector< Point > ring ;

            do {
                ring.push_back( Point( it->vertex()->point() ) );
            }
            while ( ++it != it_facet->facet_begin() );

            ring.push_back( ring.front() );

            result->addPolygon( Polygon( ring ) );
        }

        return std::auto_ptr<Geometry>( result.release() );
    }
    else {
        BOOST_THROW_EXCEPTION( Exception( "unexpected CGAL output type in CGAL::convex_hull_3" ) );
    }
}


}//algorithm
}//SFCGAL
