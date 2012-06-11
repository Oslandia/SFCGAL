#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

#include <fstream>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>


#include "../../test_config.h"

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_WktTest )

BOOST_AUTO_TEST_CASE( testReadWriter )
{
	std::string filename( SFCGAL_TEST_DIRECTORY );
	filename += "/regress/SFCGAL/WktTest.txt" ;

	std::ifstream ifs( filename.c_str() );
	BOOST_REQUIRE( ifs.good() ) ;

	std::string inputWkt;
	while ( std::getline( ifs, inputWkt ) ){
		if ( inputWkt[0] == '#' || inputWkt.empty() )
			continue ;

		/*
		 * parse wkt and check symmetry
		 */
		std::auto_ptr< Geometry > g( io::readWkt(inputWkt) );
		std::string outputWkt = g->asText(1);
		BOOST_CHECK_EQUAL( inputWkt, outputWkt );
	}
}


BOOST_AUTO_TEST_SUITE_END()




