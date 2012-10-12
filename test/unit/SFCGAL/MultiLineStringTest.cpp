#include <boost/test/unit_test.hpp>

#include <exception>

#include <SFCGAL/LineString.h>
#include <SFCGAL/MultiLineString.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_MultiLineStringTest )

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
	MultiLineString g;
	BOOST_CHECK( g.isEmpty() ) ;
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.numGeometries(), 0U );
}


//-- addAllowedGeometry
BOOST_AUTO_TEST_CASE( addLineString )
{
	MultiLineString g;
	g.addGeometry( new LineString() );
	BOOST_CHECK_EQUAL( g.numGeometries(), 1U );
}
//-- addForbidenGeometry
BOOST_AUTO_TEST_CASE( addLineStringThrow )
{
	MultiLineString g;
	BOOST_CHECK_THROW( g.addGeometry( Point() ), std::exception );
}

//-- asText

BOOST_AUTO_TEST_CASE( asTextEmpty )
{
	MultiLineString g;
	BOOST_CHECK_EQUAL( g.asText(1), "MULTILINESTRING EMPTY" );
}

BOOST_AUTO_TEST_CASE( asText2d )
{
	MultiLineString g;
	g.addGeometry( LineString( Point(0.0,0.0), Point(1.0,1.0) ) );
	g.addGeometry( LineString( Point(1.0,1.0), Point(2.0,2.0) ) );
	BOOST_CHECK_EQUAL( g.asText(1), "MULTILINESTRING((0.0 0.0,1.0 1.0),(1.0 1.0,2.0 2.0))" );
}

//-- is< T >

BOOST_AUTO_TEST_CASE( isGeometryCollection )
{
	MultiLineString g;
	BOOST_CHECK( g.is< GeometryCollection >() );
}

BOOST_AUTO_TEST_CASE( isMultiLineString )
{
	MultiLineString g;
	BOOST_CHECK( g.is< MultiLineString >() );
}

BOOST_AUTO_TEST_SUITE_END()




