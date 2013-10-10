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

#include "../test_config.h"
#include "Bench.h"

#include <boost/test/unit_test.hpp>

#include <SFCGAL/detail/generator/sierpinski.h>

#include <SFCGAL/algorithm/area.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_BenchArea )


BOOST_AUTO_TEST_CASE( testAreaSierpinski )
{
    std::auto_ptr< MultiPolygon > fractal( generator::sierpinski( 9 ) ) ;

    bench().start( "area sierpinski" ) ;

    for ( int i = 0; i < 10; i++ ) {
        algorithm::area( *fractal ) ;
    }

    bench().stop();
}

BOOST_AUTO_TEST_CASE( testAreaSierpinski3D )
{
    std::auto_ptr< MultiPolygon > fractal( generator::sierpinski( 9 ) ) ;

    bench().start( "area sierpinski" ) ;

    for ( int i = 0; i < 10; i++ ) {
        algorithm::area3D( *fractal ) ;
    }

    bench().stop() ;
}


BOOST_AUTO_TEST_SUITE_END()




