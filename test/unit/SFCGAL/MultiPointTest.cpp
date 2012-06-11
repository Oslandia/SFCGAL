#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

#include <exception>

#include <SFCGAL/LineString.h>
#include <SFCGAL/MultiPoint.h>

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_MultiPointTest )

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
	MultiPoint g;
	BOOST_CHECK( g.isEmpty() ) ;
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.numGeometries(), 0U );
}


//-- addAllowedGeometry
BOOST_AUTO_TEST_CASE( addPoint )
{
	MultiPoint g;
	g.addGeometry( new Point( 2.0, 3.0 ) );
	BOOST_CHECK_EQUAL( g.numGeometries(), 1U );
}
//-- addForbidenGeometry
BOOST_AUTO_TEST_CASE( addLineStringThrow )
{
	MultiPoint g;
	BOOST_CHECK_THROW( g.addGeometry( LineString() ), std::exception );
}

//-- asText

BOOST_AUTO_TEST_CASE( asTextEmpty )
{
	MultiPoint g;
	BOOST_CHECK_EQUAL( g.asText(1), "MULTIPOINT EMPTY" );
}

BOOST_AUTO_TEST_CASE( asText2d )
{
	MultiPoint g;
	g.addGeometry( Point(2.0,3.0) );
	g.addGeometry( Point(3.0,4.0) );
	BOOST_CHECK_EQUAL( g.asText(3), "MULTIPOINT((2.000 3.000),(3.000 4.000))" );
}

//-- is< T >

BOOST_AUTO_TEST_CASE( isGeometryCollection )
{
	MultiPoint g;
	BOOST_CHECK( g.is< GeometryCollection >() );
}

BOOST_AUTO_TEST_CASE( isMultiPoint )
{
	MultiPoint g;
	BOOST_CHECK( g.is< MultiPoint >() );
}

BOOST_AUTO_TEST_SUITE_END()




