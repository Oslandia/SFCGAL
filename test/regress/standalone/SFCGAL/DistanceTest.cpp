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
#include <boost/test/unit_test.hpp>

#include "../../../test_config.h"

#include <fstream>
#include <boost/format.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include <SFCGAL/tools/Log.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;


BOOST_AUTO_TEST_SUITE( SFCGAL_DistanceTest )

/**
 * Compute the area for given geometries
 */
BOOST_AUTO_TEST_CASE( testFileDistanceTest )
{
	//logger().setLogLevel( Logger::Debug );

	std::string filename( SFCGAL_TEST_DIRECTORY );
	filename += "/regress/data/DistanceTest.txt" ;

	std::ifstream ifs( filename.c_str() );
	BOOST_REQUIRE( ifs.good() ) ;

	std::string line;
	while ( std::getline( ifs, line ) ){
		if ( line[0] == '#' || line.empty() )
			continue ;

		BOOST_TEST_MESSAGE( line );

		std::istringstream iss(line);

		std::string distanceDimension ;
		std::string wktGA, wktGB ;
		double expectedDistance ;

		std::getline( iss, distanceDimension, '|' ) ;
		std::getline( iss, wktGA, '|' ) ;
		std::getline( iss, wktGB, '|' ) ;
		iss >> expectedDistance ;

		std::auto_ptr< Geometry > gA( io::readWkt( wktGA ) );
		std::auto_ptr< Geometry > gB( io::readWkt( wktGB ) );

		if ( distanceDimension == "2" ){
			BOOST_CHECK_EQUAL( gA->distance(*gB), expectedDistance );
		}else if ( distanceDimension == "3" ){
			BOOST_CHECK_EQUAL( gA->distance3D(*gB), expectedDistance );
		}else{
			BOOST_CHECK(false);
		}
	}
}


BOOST_AUTO_TEST_SUITE_END()




