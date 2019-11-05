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
#include <SFCGAL/detail/triangulate/ConstraintDelaunayTriangulation.h>
#include <SFCGAL/algorithm/area.h>


#include "../../../test_config.h"

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;
using namespace SFCGAL::triangulate ;

BOOST_AUTO_TEST_SUITE( SFCGAL_ConstraintDelaunayTriangulationTest )

BOOST_AUTO_TEST_CASE( testTriangulateRGC )
{
    ConstraintDelaunayTriangulation triangulation ;

    /*
     * read points from file
     */
    std::string filename( SFCGAL_TEST_DIRECTORY );
    filename += "/data/rgc-france-ign.xyz" ;
    std::ifstream ifs( filename.c_str() );
    BOOST_REQUIRE( ifs.good() ) ;

    double x,y,z ;

    while ( ifs >> x >> y >> z ) {
        triangulation.addVertex( Coordinate( x,y,z ) );
    }

    ifs.close();

    //std::string wkt = triangulation.getTriangulatedSurface()->asText(5.0) ;
    //std::cerr << "INSERT INTO draw (geometry) VALUES ( '" << "MULTIPOLYGON" << wkt.substr(3) << "'::geometry );" << std::endl;

    BOOST_CHECK_EQUAL( triangulation.numVertices(), 36566U );
    BOOST_CHECK_EQUAL( triangulation.numTriangles(), 73114U );

    std::unique_ptr< TriangulatedSurface > triangulatedSurface = triangulation.getTriangulatedSurface() ;
    BOOST_CHECK_EQUAL( triangulatedSurface->numTriangles(), 73114U );
    BOOST_CHECK_CLOSE( algorithm::area( *triangulatedSurface ), 818056610000.0, 0.1 );
}




BOOST_AUTO_TEST_SUITE_END()




