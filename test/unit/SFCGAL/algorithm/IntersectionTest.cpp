/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
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

#include <SFCGAL/Kernel.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/GeometryStreams.h>
#include <SFCGAL/all.h>
#include <SFCGAL/detail/transform/AffineTransform3.h>

#include "../../../test_config.h"

#include <boost/test/unit_test.hpp>
#include <boost/ptr_container/ptr_map.hpp>

using namespace SFCGAL;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_IntersectionTest )

BOOST_AUTO_TEST_CASE( testFileIntersectsTest )
{
	boost::ptr_map<std::string, Geometry*> storedGeom;

	//logger().setLogLevel( Logger::Debug );

	std::string filename( SFCGAL_TEST_DIRECTORY );
	filename += "/data/IntersectionTest.txt" ;

	std::ifstream ifs( filename.c_str() );
	BOOST_REQUIRE( ifs.good() ) ;

	int numLine = 0 ;
	std::string line;
	while ( std::getline( ifs, line ) ){
		numLine++;
		if ( line[0] == '#' || line.empty() )
			continue ;

		BOOST_TEST_MESSAGE( boost::format("line#%s:%s") % numLine % line );

		std::istringstream iss(line);

		std::string dimension ;
		std::string wktGA, wktGB ;
		std::string wktOut ;

		std::getline( iss, dimension, '|' ) ;
		
		if ( dimension == "S" ) {
			// store the geometry
			std::string geomName;
			std::getline( iss, geomName, '|' );
			std::getline( iss, wktGA, '|' );
			storedGeom[ geomName ] = io::readWkt( wktGA ).release();
			continue;
		}

		std::auto_ptr< Geometry > gA(0);
		std::auto_ptr< Geometry > gB(0);
		std::auto_ptr< Geometry > gOut(0);

		std::getline( iss, wktGA, '|' ) ;
		if ( wktGA[0] == '@' ) {
			// stored geometry reference
			std::string name = wktGA.substr(1);
			if ( storedGeom.find( name ) == storedGeom.end() ) {
				BOOST_CHECK_MESSAGE( false, numLine << ": can't find the geometry named " << name );				
			}
			gA.reset( storedGeom[ name ]->clone() );
		}
		else {
			gA = io::readWkt( wktGA );
		}
		storedGeom[ "A" ] = gA->clone();

		std::getline( iss, wktGB, '|' ) ;
		if ( wktGB[0] == '@' ) {
			// stored geometry reference
			std::string name = wktGB.substr(1);
			if ( storedGeom.find( name ) == storedGeom.end() ) {
				BOOST_CHECK_MESSAGE( false, numLine << ": can't find the geometry named " << name );				
			}
			gB.reset( storedGeom[ name ]->clone() );
		}
		else {
			gB = io::readWkt( wktGB );
		}
		storedGeom[ "B" ] = gB->clone();

		std::getline( iss, wktOut, '|' ) ;
		if ( wktOut[0] == '@' ) {
			// stored geometry reference
			std::string name = wktOut.substr(1);
			if ( storedGeom.find( name ) == storedGeom.end() ) {
				BOOST_CHECK_MESSAGE( false, numLine << ": can't find the geometry named " << name );				
			}
			gOut.reset( storedGeom[ name ]->clone() );
		}
		else {
			gOut = io::readWkt( wktOut );
		}

		try {
			if ( dimension == "2" ){
				std::auto_ptr<Geometry> result = algorithm::intersection(*gA,*gB);
				BOOST_CHECK_MESSAGE( *result == *gOut, numLine << ": intersection(" << gA->asText() << ", " << gB->asText() << ") is " << result->asText() << " and should be " << gOut->asText() );
			}else if ( dimension == "3" ){
				std::auto_ptr<Geometry> result = algorithm::intersection3D(*gA,*gB);
				BOOST_CHECK_MESSAGE( *result == *gOut, numLine << ": intersection3D(" << gA->asText() << ", " << gB->asText() << ") is " << result->asText() << " and should be " << gOut->asText() );
			}else{
				BOOST_CHECK(false);
			}
		}
		catch ( std::exception& e ) {
			BOOST_CHECK_MESSAGE( false, numLine << ": " << e.what() );
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()

