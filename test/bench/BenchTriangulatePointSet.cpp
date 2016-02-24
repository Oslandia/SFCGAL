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
#include <fstream>

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/detail/generator/hoch.h>
#include <SFCGAL/detail/generator/disc.h>
#include <SFCGAL/triangulate/triangulate2DZ.h>
#include <SFCGAL/triangulate/triangulatePolygon.h>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include "../test_config.h"
#include "Bench.h"

#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include <CGAL/point_generators_2.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;
using namespace SFCGAL::triangulate ;

#define N_POINTS 100000

template < typename K >
std::vector< CGAL::Point_2<K> > generatePoints(){
    CGAL::default_random = CGAL::Random(0);
    
    typedef CGAL::Creator_uniform_2< double, CGAL::Point_2<K> > Creator ;
    typedef CGAL::Point_2<K> Point_2 ;
    
    CGAL::Random_points_in_disc_2< Point_2, Creator > g( 150.0 );
    std::vector< Point_2 > points ;
    points.reserve(N_POINTS);
    CGAL::cpp11::copy_n( g, N_POINTS, std::back_inserter(points));
    return points ;
}


BOOST_AUTO_TEST_SUITE( SFCGAL_BenchTriangulatePointSet )

BOOST_AUTO_TEST_CASE( testMultiPointTriangulationReferenceEpick )
{
    // prepare point set
    std::vector< Epick::Point_2 > points = generatePoints<Epick>() ;
    
    // compute triangulation
    bench().start( boost::format( "ref Epick - triangulate2DZ %s points" ) % N_POINTS );
    CGAL::Constrained_Delaunay_triangulation_2< Epick > triangulation ;
    triangulation.insert(points.begin(), points.end());
    bench().stop();
    
    BOOST_CHECK_EQUAL( N_POINTS, triangulation.number_of_vertices() );
    BOOST_CHECK( triangulation.number_of_faces() > 0 );
}


BOOST_AUTO_TEST_CASE( testMultiPointTriangulationReferenceEpeck )
{
    // prepare point set
    std::vector< Epeck::Point_2 > points = generatePoints<Epeck>() ;
    
    // compute triangulation
    bench().start( boost::format( "ref Epeck - triangulate2DZ %s points (reference)" ) % N_POINTS );
    CGAL::Constrained_Delaunay_triangulation_2< Epeck > triangulation ;
    triangulation.insert(points.begin(), points.end());
    bench().stop();
    
    BOOST_CHECK_EQUAL( N_POINTS, triangulation.number_of_vertices() );
    BOOST_CHECK( triangulation.number_of_faces() > 0 );
}


BOOST_AUTO_TEST_CASE( testMultiPointTriangulationReferenceEpeckMisused )
{
    // prepare point set
    std::vector< Epeck::Point_2 > points = generatePoints<Epeck>() ;
    
    // compute triangulation
    bench().start( boost::format( "ref Epeck - triangulate2DZ %s points (reference)" ) % N_POINTS );
    CGAL::Constrained_Delaunay_triangulation_2< Epeck > triangulation ;
    // BAD WAY!
    BOOST_FOREACH(Epeck::Point_2 point, points){
        triangulation.insert(point);
    }
    // GOOD WAY
    //triangulation.insert(points.begin(), points.end());
    
    
    bench().stop();
    
    BOOST_CHECK_EQUAL( N_POINTS, triangulation.number_of_vertices() );
    BOOST_CHECK( triangulation.number_of_faces() > 0 );
}


BOOST_AUTO_TEST_CASE( testTriangulate2DZ )
{
    // prepare point set
    std::vector< Epeck::Point_2 > points = generatePoints<Epeck>() ;
    
    MultiPoint multiPoint ;
    for ( int i = 0; i < N_POINTS; i++ ) {
        multiPoint.addGeometry( new Point( points[i] ) );
    }
    
    // compute triangulation
    bench().start( boost::format( "SFCGAL - triangulate2DZ %s points" ) % N_POINTS );
    std::auto_ptr< TriangulatedSurface > triangulation = triangulate2DZ( multiPoint ) ;
    bench().stop();
    
    BOOST_CHECK( triangulation->numTriangles() > 0 );
}


BOOST_AUTO_TEST_SUITE_END()




