#include <fstream>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/triangulate.h>

#include "../../../test_config.h"

#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>



using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_IntersectsTest )

//
// Load an obj
// Build a polyhedral surface
// Test intersection with some other objects

// FIXME removed wavefront obj loader, transform to wkt
//BOOST_AUTO_TEST_CASE( testIntersectsTeapot )
//{
//    std::string filename( SFCGAL_TEST_DIRECTORY );
//    filename += "/regress/data/teapot.obj" ;
//
//    io::WaveFrontObj obj;
//    obj.load( filename );
//
//    //check points defined in all faces
//    PolyhedralSurface surf;
//    for ( size_t i = 0; i < obj.numFaces(); i++ ){
//	LineString ext_ring;
//	const io::WaveFrontFace & face = obj.faceN(i);
//	for ( size_t j = 0; j < face.size(); j++ ) {
//	    ext_ring.addPoint( obj.vertexN( face[j].v ) );
//	}
//	// close the polygon
//	ext_ring.addPoint( obj.vertexN( face[0].v ) );
//	surf.addPolygon( Polygon(ext_ring) );
//    }
//
//    Triangle tri1( Point(3.0, 1.8, 0.0), Point(-2.99, 1.8, -0.081), Point(-2.99, 1.8, 0.081) );
//    Triangle tri2( Point(3.0, 1.8, 5.0), Point(-2.99, 1.8, 5.081), Point(-2.99, 1.8, 5.081) );
//
//    bool intersects1 = algorithm::intersects3D( tri1, surf );
//    bool intersects2 = algorithm::intersects3D( tri2, surf );
//    BOOST_TEST_MESSAGE( boost::format("triangle1 intersects the teapot: %1%") % (intersects1 ? "YES" : "NO") );
//    BOOST_CHECK_EQUAL( intersects1, true );
//    BOOST_TEST_MESSAGE( boost::format("triangle2 intersects the teapot: %1%") % (intersects2 ? "YES" : "NO") );
//    BOOST_CHECK_EQUAL( intersects2, false );
//}



/**
 * Perform tests in test/regress/data/IntersectsTest.txt
 */
BOOST_AUTO_TEST_CASE( testFileIntersectsTest )
{
	//logger().setLogLevel( Logger::Debug );

	std::string filename( SFCGAL_TEST_DIRECTORY );
	filename += "/regress/data/IntersectsTest.txt" ;

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

		std::string distanceDimension ;
		std::string wktGA, wktGB ;
		std::string trueOrFalse ;

		std::getline( iss, distanceDimension, '|' ) ;
		std::getline( iss, wktGA, '|' ) ;
		std::getline( iss, wktGB, '|' ) ;
		std::getline( iss, trueOrFalse, '|' ) ;

		bool expected = ( trueOrFalse == "true" ) ? true : false ;

		std::auto_ptr< Geometry > gA( io::readWkt( wktGA ) );
		std::auto_ptr< Geometry > gB( io::readWkt( wktGB ) );

		if ( distanceDimension == "2" ){
			BOOST_CHECK_MESSAGE( algorithm::intersects(*gA,*gB) == expected, "intersects(" << gA->asText() << ", " << gB->asText() << ") should be " << (expected ? "TRUE" : "FALSE") );
		}else if ( distanceDimension == "3" ){
			BOOST_CHECK_MESSAGE( algorithm::intersects3D(*gA,*gB) == expected, "intersects3D(" << gA->asText() << ", " << gB->asText() << ") should be " << (expected ? "TRUE" : "FALSE") );
		}else{
			BOOST_CHECK(false);
		}
	}
}

//
// Test limit case
BOOST_AUTO_TEST_CASE( testLimitsIntersects )
{
    std::string filename( SFCGAL_TEST_DIRECTORY );
    filename += "/regress/data/countries.wkt" ;
    
    std::ifstream ifs( filename.c_str() );
    BOOST_REQUIRE( ifs.good() ) ;

    std::string wkt1, wkt2;
    std::getline( ifs, wkt1 );
    std::getline( ifs, wkt2 );
    
    std::auto_ptr< Geometry > g1( io::readWkt(wkt1) );
    std::auto_ptr< Geometry > g2( io::readWkt(wkt2) );

    // check that a call to intersects() does not throw
    bool throws = false;
    algorithm::intersects( *g1, *g2 );
    BOOST_CHECK_EQUAL( throws, false );
}

BOOST_AUTO_TEST_SUITE_END()




