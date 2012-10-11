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




