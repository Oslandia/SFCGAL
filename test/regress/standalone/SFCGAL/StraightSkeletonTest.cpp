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
#include <SFCGAL/algorithm/straightSkeleton.h>

#include "../../../test_config.h"

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_StraightSkeletonTest )

namespace {

    void runTest(const std::string& filename)
    {
        std::ifstream ifs( filename.c_str() );
        BOOST_REQUIRE( ifs.good() ) ;

        std::string line;
        std::string lbl_base = boost::filesystem::basename(filename);

        int lineno = 0;
        while ( std::getline( ifs, line ) ) {
            if ( line[0] == '#' || line.empty() ) {
                continue ;
            }
            std::stringstream lblstream;
            lblstream << lbl_base << ':' << lineno << ": ";
            std::string lbl = lblstream.str();

            std::istringstream iss( line );
            std::string inputWkt ;
            std::string outputWkt ;
            std::string obtWkt ;

            BOOST_CHECK( std::getline( iss, inputWkt, '|' ) );
            BOOST_CHECK( std::getline( iss, outputWkt, '|' ) );

            std::auto_ptr< Geometry > g( io::readWkt( inputWkt ) );
            std::auto_ptr< MultiLineString > result;
            try {
              result = algorithm::straightSkeleton( *g ) ;
              obtWkt = result->asText( 6 );
            } catch (const std::exception& e) {
              obtWkt = std::string(e.what());
            }
            std::string obt = lbl + obtWkt;
            std::string exp = lbl + outputWkt;
            BOOST_CHECK_EQUAL( exp, obt );
        }
    }
}

BOOST_AUTO_TEST_CASE( testStraightSkeletonTest )
{
    using namespace boost;
    using namespace boost::filesystem;

    std::string testdir( SFCGAL_TEST_DIRECTORY );
    path dirname = testdir + "/data/StraightSkeletonTest" ;
    if ( is_directory(dirname) )
    {
      directory_iterator it = directory_iterator(dirname);
      while ( it != directory_iterator() )
      {
        path f = *it;
        try {
          runTest(f.c_str());
        } catch (const std::exception &e) {
          // There must be a better way to do this...
          std::cerr << "Failed test: " << f << ": " << e.what() << std::endl;
        }
        ++it;
      }
    }
}


BOOST_AUTO_TEST_SUITE_END()




