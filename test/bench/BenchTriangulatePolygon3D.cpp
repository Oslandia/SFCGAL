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

#include <CGAL/point_generators_2.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;
using namespace SFCGAL::triangulate ;

#define N_POINTS 100000

typedef CGAL::Creator_uniform_2< double, Kernel::Point_2 > Creator ;
typedef CGAL::Creator_uniform_2< double, Epick::Point_2 > EpickCreator ;

BOOST_AUTO_TEST_SUITE( SFCGAL_BenchTriangulatePolygon3D )

BOOST_AUTO_TEST_CASE( testPolygonTriangulationHoch )
{
    const int N = 7 ;
    std::auto_ptr< Polygon > fractal( generator::hoch( N ) );
    BOOST_CHECK_EQUAL( fractal->exteriorRing().numPoints(), 49153U );

    bench().start( boost::format( "triangulatePolygon3D hoch(%s)" ) % N );
    TriangulatedSurface triangulatedSurface ;
    SFCGAL::triangulate::triangulatePolygon3D( *fractal, triangulatedSurface ) ;
    bench().stop() ;
}

BOOST_AUTO_TEST_CASE( testPolygonTriangulationHoch_roundingSixDecimal )
{
    const int N = 7 ;
    std::auto_ptr< Polygon > fractal( generator::hoch( N ) );
    BOOST_CHECK_EQUAL( fractal->exteriorRing().numPoints(), 49153U );

    fractal->round( 100000 ) ;

    bench().start( boost::format( "triangulatePolygon3D hoch(%s) (round 6 six decimals)" ) % N );
    TriangulatedSurface triangulatedSurface ;
    SFCGAL::triangulate::triangulatePolygon3D( *fractal, triangulatedSurface ) ;
    bench().stop() ;
}



BOOST_AUTO_TEST_CASE( testPolygonTriangulationDisc )
{
    const int N = 2000 ;

    std::auto_ptr< Polygon > disc( generator::disc( Point( 0.0,0.0 ), 1.0, 8U ) );
//	std::cout << fractal->asText(5) << std::endl ;

    bench().start( boost::format( "triangulate disc x %s" ) % N );

    for ( int i = 0; i < N; i++ ) {
        TriangulatedSurface triangulatedSurface ;
        SFCGAL::triangulate::triangulatePolygon3D( *disc, triangulatedSurface ) ;
    }

    bench().stop();
}




BOOST_AUTO_TEST_SUITE_END()




