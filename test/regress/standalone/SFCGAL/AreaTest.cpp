#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

#include "../../../test_config.h"

#include <fstream>
#include <boost/format.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/algorithm/area.h>
#include <SFCGAL/Transform.h>


using namespace SFCGAL ;


class RotateCoordinate : public Transform {
public:

	virtual void transform( Point & p )
	{
		BOOST_ASSERT( ! p.isEmpty() );
		p.z() = p.is3D() ? p.z() : 0.0 ;
		std::swap( p.x(), p.z() );
		std::swap( p.y(), p.z() );
	}

};


BOOST_AUTO_TEST_SUITE( SFCGAL_AreaTest )

/**
 * Triangulate polygon and make some checks
 * @ŧodo Check inPolygon.area3D() == outPolygon.area3D();
 */
BOOST_AUTO_TEST_CASE( testComputeArea )
{
	std::string filename( SFCGAL_TEST_DIRECTORY );
	filename += "/regress/data/AreaTest.txt" ;

	std::ifstream ifs( filename.c_str() );
	BOOST_REQUIRE( ifs.good() ) ;

	std::string line;
	while ( std::getline( ifs, line ) ){
		if ( line[0] == '#' || line.empty() )
			continue ;

		std::istringstream iss(line);

		std::string id ;
		iss >> id ;

		double expectedArea ;
		iss >> expectedArea ;

		std::string inputWkt ;
		std::getline( iss, inputWkt ) ;

		std::auto_ptr< Geometry > g( io::readWkt(inputWkt) );
		double area = algorithm::area3D( *g ) ;
		BOOST_TEST_MESSAGE( boost::format("area( '%1%' ) = %2%") % inputWkt % area );

		RotateCoordinate rotateCoordinate ;
		g->accept(rotateCoordinate);
		double areaRotate = algorithm::area3D( *g ) ;

		//check area == areaRotate
		BOOST_CHECK_CLOSE( area, areaRotate, 0.5 );
		//check area == expectedArea
		BOOST_CHECK_CLOSE( area, expectedArea, 0.5 );
	}
}


BOOST_AUTO_TEST_SUITE_END()




