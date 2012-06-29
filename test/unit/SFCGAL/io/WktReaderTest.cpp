#include <memory>
#include <string>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;
using namespace SFCGAL::io ;


BOOST_AUTO_TEST_SUITE( SFCGAL_io_WktReaderTest )

//-- WKT POINT

BOOST_AUTO_TEST_CASE( pointEmpty )
{
	std::auto_ptr< Geometry > g( readWkt("POINT EMPTY") );
	BOOST_CHECK( g->is< Point >() );
	BOOST_CHECK( g->isEmpty() );
}


BOOST_AUTO_TEST_CASE( pointXY )
{
	std::auto_ptr< Geometry > g( readWkt("POINT(4.0 6.0)") );
	BOOST_CHECK( g->is< Point >() );
	BOOST_CHECK( ! g->isEmpty() );

	BOOST_CHECK_EQUAL( g->as< Point >().x(), 4.0 );
	BOOST_CHECK_EQUAL( g->as< Point >().y(), 6.0 );
}

//-- WKT LINESTRING


BOOST_AUTO_TEST_CASE( lineStringEmpty )
{
	std::auto_ptr< Geometry > g( readWkt("LINESTRING EMPTY") );
	BOOST_CHECK( g->is< LineString >() );
	BOOST_CHECK( g->isEmpty() );
}

BOOST_AUTO_TEST_CASE( lineString_twoPoints )
{
	std::auto_ptr< Geometry > g( readWkt("LINESTRING(0.0 0.0,1.0 1.0)") );
	BOOST_CHECK( g->is< LineString >() );
	BOOST_CHECK( ! g->isEmpty() );
	BOOST_CHECK_EQUAL( g->as< LineString >().numPoints(), 2U );
}


//-- WKT POLYGON


BOOST_AUTO_TEST_CASE( polygonEmpty )
{
	std::auto_ptr< Geometry > g( readWkt("POLYGON EMPTY") );
	BOOST_CHECK( g->is< Polygon >() );
	BOOST_CHECK( g->isEmpty() );
}



//-- WKT MULTIPOINT

BOOST_AUTO_TEST_CASE( multiPointEmpty )
{
	std::auto_ptr< Geometry > g( readWkt("MULTIPOINT EMPTY") );
	BOOST_CHECK( g->is< MultiPoint >() );
	BOOST_CHECK( g->isEmpty() );
}

//-- WKT MULTILINESTRING

BOOST_AUTO_TEST_CASE( multiLineStringEmpty )
{
	std::auto_ptr< Geometry > g( readWkt("MULTILINESTRING EMPTY") );
	BOOST_CHECK( g->is< MultiLineString >() );
	BOOST_CHECK( g->isEmpty() );
}


//-- WKT MULTIPOLYGON

BOOST_AUTO_TEST_CASE( multiPolygonEmpty )
{
	std::auto_ptr< Geometry > g( readWkt("MULTIPOLYGON EMPTY") );
	BOOST_CHECK( g->is< MultiPolygon >() );
	BOOST_CHECK( g->isEmpty() );
}



//-- WKT GEOMETRYCOLLECTION

BOOST_AUTO_TEST_CASE( geometryCollectionEmpty )
{
	std::auto_ptr< Geometry > g( readWkt("GEOMETRYCOLLECTION EMPTY") );
	BOOST_CHECK( g->is< GeometryCollection >() );
	BOOST_CHECK( g->isEmpty() );
}



//-- WKT TRIANGULATEDSURFACE

BOOST_AUTO_TEST_CASE( triangulatedSurface_Empty )
{
	std::auto_ptr< Geometry > g( readWkt("TIN EMPTY") );
	BOOST_CHECK( g->is< TriangulatedSurface >() );
	BOOST_CHECK( g->isEmpty() );
}

BOOST_AUTO_TEST_CASE( triangulatedSurface_fourTriangles )
{
	std::string wkt = "TIN("
			"((0 0 0, 0 0 1, 0 1 0, 0 0 0)),"
			"((0 0 0, 0 1 0, 1 0 0, 0 0 0)),"
			"((0 0 0, 1 0 0, 0 0 1, 0 0 0)),"
			"((1 0 0, 0 1 0, 0 0 1, 1 0 0))"
			")";
	std::auto_ptr< Geometry > g( readWkt(wkt) );
	BOOST_CHECK( g->is< TriangulatedSurface >() );
	BOOST_CHECK( ! g->isEmpty() );

	BOOST_CHECK_EQUAL( g->as< TriangulatedSurface >().numTriangles(), 4U );
}



BOOST_AUTO_TEST_SUITE_END()



