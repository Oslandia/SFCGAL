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
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/convexHull.h>

#include "../test_config.h"
#include "Bench.h"

#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_IntersectionPerfTest )


#define N_POLYGONS 10000
#define N_POINTS 50

//
// Test limit case
BOOST_AUTO_TEST_CASE( testIntersectionPerf )
{

    //
    // generate polygons
    std::vector<Geometry*> polygons;

    for ( size_t i = 0; i < N_POLYGONS; ++i ) {
        MultiPoint mp;

        for ( size_t j = 0; j < N_POINTS; ++j ) {
            double x = ( rand() +.0 ) / RAND_MAX * 10.0;
            double y = ( rand() +.0 ) / RAND_MAX * 10.0;
            mp.addGeometry( Point( x, y ) );
        }

        std::unique_ptr<Geometry> g( algorithm::convexHull( mp ) );
        polygons.push_back( g.release() );
    }

    bench().start( "intersects convex hull" );

    for ( size_t i = 0; i < N_POLYGONS / 2; ++i ) {
        algorithm::intersects( *polygons[2*i], *polygons[2*i+1] );
    }

    bench().stop();
}

BOOST_AUTO_TEST_SUITE_END()




