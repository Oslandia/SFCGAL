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
#include <boost/test/unit_test.hpp>

#include <cmath>
#include <fstream>

#include <SFCGAL/Kernel.h>
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
#include <SFCGAL/algorithm/covers.h>

#include "../../../test_config.h"

using namespace SFCGAL ;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_CoversTest )

/**
 * Perform tests in test/data/CoversTest.txt
 */
BOOST_AUTO_TEST_CASE( testFileCoversTest )
{
    int argc = framework::master_test_suite().argc;
    char** argv = framework::master_test_suite().argv;

    // look for options
    int test_one_line = -1;

    for ( int i = 0; i < argc; ++i ) {
        std::string argi( argv[i] );

        if ( argi == "--line" ) {
            // only test one line
            if ( argc >= i+1 ) {
                sscanf( argv[i+1], "%d", &test_one_line );
                ++i;
                continue;
            }
        }
    }

    std::string filename( SFCGAL_TEST_DIRECTORY );
    filename += "/data/CoversTest.txt" ;

    std::ifstream ifs( filename.c_str() );
    BOOST_REQUIRE( ifs.good() ) ;

    int numLine = 0 ;
    std::string line;

    while ( std::getline( ifs, line ) ) {
        numLine++;

        if ( test_one_line != -1 && test_one_line != numLine ) {
            continue;
        }

        if ( line[0] == '#' || line.empty() ) {
            continue ;
        }

        BOOST_TEST_MESSAGE( boost::format( "line#%s:%s" ) % numLine % line );

        std::istringstream iss( line );

        std::string distanceDimension ;
        std::string wktGA, wktGB ;
        std::string trueOrFalse ;

        std::getline( iss, distanceDimension, '|' ) ;
        std::getline( iss, wktGA, '|' ) ;
        std::getline( iss, wktGB, '|' ) ;
        std::getline( iss, trueOrFalse, '|' ) ;

        bool expected = ( trueOrFalse == "true" ) ? true : false ;

        std::unique_ptr< Geometry > gA( io::readWkt( wktGA ) );
        std::unique_ptr< Geometry > gB( io::readWkt( wktGB ) );

        try {
            if ( distanceDimension == "2" ) {
                BOOST_CHECK_MESSAGE( algorithm::covers( *gA,*gB ) == expected, numLine << ": covers(" << gA->asText() << ", " << gB->asText() << ") should be " << ( expected ? "TRUE" : "FALSE" ) );
            }
            else if ( distanceDimension == "3" ) {
                bool got = algorithm::covers3D( *gA,*gB );
                BOOST_CHECK_MESSAGE( got == expected, numLine << ": covers3D(" << gA->asText() << ", " << gB->asText() << ") should be " << ( expected ? "TRUE" : "FALSE" ) );
            }
            else {
                BOOST_CHECK( false );
            }
        }
        catch ( std::exception& e ) {
            BOOST_CHECK_MESSAGE( false, numLine << ": " << e.what() );
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

