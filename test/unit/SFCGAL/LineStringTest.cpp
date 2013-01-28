/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <boost/test/unit_test.hpp>

#include <SFCGAL/LineString.h>

using namespace SFCGAL ;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_LineStringTest )

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
	LineString g;
	BOOST_CHECK( g.isEmpty() );
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.numPoints(), 0U );
}

BOOST_AUTO_TEST_CASE( twoPointsConstructor )
{
	LineString g( Point(0.0,0.0), Point(2.0,3.0) );
	BOOST_CHECK( ! g.isEmpty() );
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.asText(1), "LINESTRING(0.0 0.0,2.0 3.0)" );
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

BOOST_AUTO_TEST_CASE( simpleIteratorTest )
{
	LineString g;
	g.addPoint( Point( 0.0, 0.0 ) );
	g.addPoint( Point( 1.0, 1.0 ) );
	g.addPoint( Point( 2.0, 2.0 ) );

	LineString::const_iterator it  = g.begin() ;
	LineString::const_iterator end = g.end() ;
	BOOST_CHECK_EQUAL( it->asText(3), "POINT(0.000 0.000)" ); ++it ;
	BOOST_CHECK( it != end );
	BOOST_CHECK_EQUAL( it->asText(3), "POINT(1.000 1.000)" ); ++it ;
	BOOST_CHECK( it != end );
	BOOST_CHECK_EQUAL( it->asText(3), "POINT(2.000 2.000)" ); ++it ;
	BOOST_CHECK( it == end );
}


BOOST_AUTO_TEST_CASE( iteratorTests )
{
	std::vector<Point> points;
	points.push_back( Point(2.0, 3.0, 1.0) );
	points.push_back( Point(4.0, 5.0, 1.0) );
	points.push_back( Point(6.0, 7.0, 1.0) );
	points.push_back( Point(8.0, 9.0, 1.0) );
	LineString g( points );

	LineString::Point_2_const_iterator pi, pi_end;
	int i = 0;
	for ( boost::tie( pi, pi_end ) = g.points_2(); pi != pi_end; pi++, i++ ) {
		BOOST_CHECK_EQUAL( *pi, g.pointN(i).toPoint_2() );
	}
	LineString::Point_3_const_iterator ppi, ppi_end;
	i = 0;
	for ( boost::tie( ppi, ppi_end ) = g.points_3(); ppi != ppi_end; ppi++, i++ ) {
		BOOST_CHECK_EQUAL( *ppi, g.pointN(i).toPoint_3() );
	}
}

BOOST_AUTO_TEST_SUITE_END()




