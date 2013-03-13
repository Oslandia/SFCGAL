/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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
#include <fstream>

#include <SFCGAL/all.h>
#include <SFCGAL/triangulate/ConstraintDelaunayTriangulation.h>
#include <SFCGAL/algorithm/area.h>


#include "../../../test_config.h"

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_ConstraintDelaunayTriangulationTest )

BOOST_AUTO_TEST_CASE( testTriangulateRGC )
{
	triangulate::ConstraintDelaunayTriangulation triangulation ;

	/*
	 * read points from file
	 */
	std::string filename( SFCGAL_TEST_DIRECTORY );
	filename += "/regress/data/rgc-france-ign.xyz" ;
	std::ifstream ifs( filename.c_str() );
	BOOST_REQUIRE( ifs.good() ) ;

	double x,y,z ;
	while ( ifs >> x >> y >> z ){
		triangulation.addVertex( Coordinate(x,y,z) );
	}
	ifs.close();

	//std::string wkt = triangulation.getTriangulatedSurface()->asText(5.0) ;
	//std::cerr << "INSERT INTO draw (geometry) VALUES ( '" << "MULTIPOLYGON" << wkt.substr(3) << "'::geometry );" << std::endl;

	BOOST_CHECK_EQUAL( triangulation.numVertices(), 36566U );
	BOOST_CHECK_EQUAL( triangulation.numTriangles(), 73114U );
	BOOST_CHECK_EQUAL( triangulation.getTriangulatedSurface()->numTriangles(), 73114U );
	BOOST_CHECK_CLOSE( algorithm::area( *triangulation.getTriangulatedSurface() ), 818056610000.0, 0.1 );
}




BOOST_AUTO_TEST_SUITE_END()




