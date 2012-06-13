#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

#include <fstream>

#include <SFCGAL/all.h>
#include <SFCGAL/io/WaveFrontObj.h>

#include "../../test_config.h"

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_io_WaveFrontObjTest )

BOOST_AUTO_TEST_CASE( testTeaPot )
{
	std::string filename( SFCGAL_TEST_DIRECTORY );
	filename += "/regress/SFCGAL/teapot.obj" ;

	io::WaveFrontObj obj;
	obj.load( filename );

	BOOST_CHECK_EQUAL( obj.numVertices(), 3644U );
	BOOST_CHECK_EQUAL( obj.numFaces(),    6320U );

	//check points defined in all faces
	for ( size_t i = 0; i < obj.numFaces(); i++ ){
		const io::WaveFrontFace & face = obj.faceN(i);
		for ( size_t j = 0; j < face.size(); j++ ){
			BOOST_CHECK_GE( face[j].v, 0 );
			BOOST_CHECK_LT( face[j].v, obj.numVertices() );
		}
	}
}


BOOST_AUTO_TEST_SUITE_END()




