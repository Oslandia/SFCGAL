/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
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
#include <boost/test/unit_test.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/tesselate.h>

#include <SFCGAL/detail/tools/Registry.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_TesselateTest )


BOOST_AUTO_TEST_CASE( testEmpty )
{
	tools::Registry & registry = tools::Registry::instance() ;

	std::vector< std::string > geometryTypes = registry.getGeometryTypes() ;
	for ( size_t i = 0; i < geometryTypes.size(); i++ ){
		std::auto_ptr< Geometry > g( registry.newGeometryByTypeName( geometryTypes[i] ) ) ;
		BOOST_TEST_MESSAGE( boost::format("tesselate(%s)") % g->asText() );
		std::auto_ptr< Geometry > result = algorithm::tesselate( *g ) ;
		BOOST_CHECK( result->isEmpty() );
	}
}

/*
 * test invariants (Point,LineString & co)
 */

BOOST_AUTO_TEST_CASE( testPoint )
{
	std::string wkt = "POINT(3.0 4.0)" ;
	std::auto_ptr< Geometry > g( io::readWkt(wkt) );
	BOOST_CHECK_EQUAL( algorithm::tesselate(*g)->asText(1), wkt );
}
BOOST_AUTO_TEST_CASE( testLineString )
{
	std::string wkt = "LINESTRING(0.0 0.0,1.0 1.0)" ;
	std::auto_ptr< Geometry > g( io::readWkt(wkt) );
	BOOST_CHECK_EQUAL( algorithm::tesselate(*g)->asText(1), wkt );
}
BOOST_AUTO_TEST_CASE( testMultiPoint )
{
	std::string wkt = "MULTIPOINT((3.0 4.0),(5.0 6.0))" ;
	std::auto_ptr< Geometry > g( io::readWkt(wkt) );
	BOOST_CHECK_EQUAL( algorithm::tesselate(*g)->asText(1), wkt );
}
BOOST_AUTO_TEST_CASE( testMultiLineString )
{
	std::string wkt = "MULTILINESTRING((0.0 0.0,1.0 1.0),(1.0 1.0,2.0 2.0))" ;
	std::auto_ptr< Geometry > g( io::readWkt(wkt) );
	BOOST_CHECK_EQUAL( algorithm::tesselate(*g)->asText(1), wkt );
}


/*
 * test with polygon, MultiPolygon & co
 */
BOOST_AUTO_TEST_CASE( testPolygon )
{
	std::string wkt = "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0))" ;
	std::string wktOut = "TIN(((0.0 1.0,1.0 0.0,1.0 1.0,0.0 1.0)),((0.0 1.0,0.0 0.0,1.0 0.0,0.0 1.0)))" ;
	std::auto_ptr< Geometry > g( io::readWkt(wkt) );
	std::auto_ptr< Geometry > result( algorithm::tesselate(*g) );
	BOOST_CHECK_EQUAL( result->asText(1), wktOut );
}
BOOST_AUTO_TEST_CASE( testMultiPolygon )
{
	std::string wkt = "MULTIPOLYGON(((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0)),((2.0 0.0,3.0 0.0,3.0 1.0,2.0 1.0,2.0 0.0)))" ;
	std::string wktOut = "GEOMETRYCOLLECTION(TIN(((0.0 1.0,1.0 0.0,1.0 1.0,0.0 1.0)),((0.0 1.0,0.0 0.0,1.0 0.0,0.0 1.0))),TIN(((2.0 1.0,3.0 0.0,3.0 1.0,2.0 1.0)),((2.0 1.0,2.0 0.0,3.0 0.0,2.0 1.0))))" ;
	std::auto_ptr< Geometry > g( io::readWkt(wkt) );
	std::auto_ptr< Geometry > result( algorithm::tesselate(*g) );
	BOOST_CHECK_EQUAL( result->asText(1), wktOut );
}


BOOST_AUTO_TEST_SUITE_END()

