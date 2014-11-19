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

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include "../test_config.h"
#include "Bench.h"

#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>

using namespace boost::unit_test ;
using namespace SFCGAL ;

typedef CGAL::Exact_predicates_exact_constructions_kernel ExactKernel;
typedef CGAL::Exact_predicates_inexact_constructions_kernel InexactKernel;

BOOST_AUTO_TEST_SUITE( SFCGAL_KernelPerfTest )

#define N_POINTS 10000000
//
// Test limit case
BOOST_AUTO_TEST_CASE( testPointCopyPerf )
{
    bench().start( "Inexact kernel copy" );
    InexactKernel::Point_2 ip2;

    for ( size_t i = 0; i < N_POINTS; ++i ) {
        // test copy
        InexactKernel::Point_2 p( ( double )rand() / RAND_MAX, ( double )rand() / RAND_MAX );
        ip2 = p;
    }

    bench().stop();

    bench().start( "Exact kernel copy" );
    ExactKernel::Point_2 ep2;

    for ( size_t i = 0; i < N_POINTS; ++i ) {
        // test copy
        ExactKernel::Point_2 p( ( double )rand() / RAND_MAX, ( double )rand() / RAND_MAX );
        ep2 = p;
    }

    bench().stop();
}

BOOST_AUTO_TEST_SUITE_END()




