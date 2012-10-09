#include "../../../test_config.h"

#include <fstream>
#include <boost/format.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/algorithm/area.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_TriangulatePolygonTest )

/**
 * Triangulate polygon and make some checks
 * @ŧodo Check inPolygon.area3D() == outPolygon.area3D();
 */
BOOST_AUTO_TEST_CASE( testTriangulatePolygon )
{
	std::string filename( SFCGAL_TEST_DIRECTORY );
	filename += "/regress/data/TriangulatePolygonTest.txt" ;

	std::ifstream ifs( filename.c_str() );
	BOOST_REQUIRE( ifs.good() ) ;

	std::string line;
	while ( std::getline( ifs, line ) ){
		if ( line[0] == '#' || line.empty() )
			continue ;

		std::istringstream iss(line);
		bool shouldThrowException ;
		iss >> shouldThrowException ;

		std::string inputWkt ;
		std::getline( iss, inputWkt ) ;

		/*
		 * parse wkt
		 */
		BOOST_TEST_MESSAGE( boost::format("triangulate polygon : #%1%") % inputWkt );
		std::auto_ptr< Geometry > g( io::readWkt(inputWkt) );

		/*
		 * check polygon
		 */
		BOOST_CHECK( g->is< Polygon >() || g->is< MultiPolygon >() );

		/*
		 * triangulate polygon
		 */
		TriangulatedSurface triangulatedSurface ;
		BOOST_CHECK_NO_THROW( algorithm::triangulate( *g, triangulatedSurface ) ) ;
		BOOST_TEST_MESSAGE( boost::format("#%1% triangle(s)") % triangulatedSurface.numGeometries() );

		/*
		 * make some check
		 */

		if ( ! g->isEmpty() ){
			BOOST_CHECK( ! triangulatedSurface.isEmpty() );
		}

		if ( g->is3D() ){
			BOOST_CHECK( triangulatedSurface.is3D() );
		}

		//check area
		BOOST_CHECK_CLOSE( algorithm::area3D(*g), algorithm::area3D( triangulatedSurface ), 0.5 );
	}
}


BOOST_AUTO_TEST_SUITE_END()




