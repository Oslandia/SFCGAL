#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

#include <SFCGAL/LineString.h>

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_LineStringTest )

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
	LineString g;
	BOOST_CHECK( g.isEmpty() );
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.numPoints(), 0U );
}


//-- asText

BOOST_AUTO_TEST_CASE( asTextEmpty )
{
	LineString g;
	BOOST_CHECK_EQUAL( g.asText(1), "LINESTRING EMPTY" );
}


BOOST_AUTO_TEST_CASE( asText2d )
{
	LineString g(
		Point(2.0,3.0),
		Point(4.0,5.0)
	);
	BOOST_CHECK_EQUAL( g.asText(3), "LINESTRING(2.000 3.000,4.000 5.000)" );
}
BOOST_AUTO_TEST_CASE( asText3d )
{
	LineString g(
		Point(2.0,3.0,7.0),
		Point(4.0,5.0,8.0)
	);
	BOOST_CHECK_EQUAL( g.asText(3), "LINESTRING(2.000 3.000 7.000,4.000 5.000 8.000)" );
}

BOOST_AUTO_TEST_CASE( isLineString )
{
	LineString g;
	BOOST_CHECK( g.is< LineString >() );
}

BOOST_AUTO_TEST_CASE( iteratorTests )
{
	std::vector<Point> points;
	points.push_back( Point(2.0, 3.0, 1.0) );
	points.push_back( Point(4.0, 5.0, 1.0) );
	points.push_back( Point(6.0, 7.0, 1.0) );
	points.push_back( Point(8.0, 9.0, 1.0) );
	LineString g( points );

	typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

	LineString::Point_2_Iterator<Kernel> pi, pi_end;
	int i = 0;
	for ( boost::tie( pi, pi_end ) = g.points_2<Kernel>(); pi != pi_end; pi++, i++ ) {
		BOOST_CHECK_EQUAL( *pi, g.points()[i].toPoint_2<Kernel>() );
	}
	LineString::Point_3_Iterator<Kernel> ppi, ppi_end;
	i = 0;
	for ( boost::tie( ppi, ppi_end ) = g.points_3<Kernel>(); ppi != ppi_end; ppi++, i++ ) {
		BOOST_CHECK_EQUAL( *ppi, g.points()[i].toPoint_3<Kernel>() );
	}
}

BOOST_AUTO_TEST_SUITE_END()




