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

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/minkowskiSum.h>
#include <SFCGAL/generator/hoch.h>

#include <SFCGAL/tools/Registry.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_MinkowskiTest )

BOOST_AUTO_TEST_CASE( testEmpty )
{
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((0 0,0 1,1 1,1 0,0 0))") );

	tools::Registry & registry = tools::Registry::instance() ;
	std::vector< std::string > typeNames = tools::Registry::instance().getGeometryTypes();
	for ( size_t i = 0; i < typeNames.size(); i++ ){
		std::auto_ptr< Geometry > g( registry.newGeometryByTypeName( typeNames[i] ) ) ;
		BOOST_CHECK( algorithm::minkowskiSum( *g, gB->as< Polygon >() )->isEmpty() );
	}
}

BOOST_AUTO_TEST_CASE( testEmptyPoint )
{
	std::auto_ptr< Geometry > gA( io::readWkt("MULTIPOINT EMPTY") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((0 0,0 1,1 1,1 0,0 0))") );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	BOOST_CHECK_EQUAL( sum->asText(0), "MULTIPOLYGON EMPTY" );
}

BOOST_AUTO_TEST_CASE( testPoint )
{
	std::auto_ptr< Geometry > gA( io::readWkt("POINT(0 0)") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((0 0,0 1,1 1,1 0,0 0))") );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	BOOST_CHECK_EQUAL( sum->asText(0), "MULTIPOLYGON(((0 0,1 0,1 1,0 1,0 0)))" );
}
BOOST_AUTO_TEST_CASE( testLineString )
{
	std::auto_ptr< Geometry > gA( io::readWkt("LINESTRING(0 0,5 0)") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((-1 0,0 -1,1 0,0 1,-1 0))") );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	BOOST_CHECK_EQUAL( sum->asText(0), "MULTIPOLYGON(((5 1,0 1,-1 0,0 -1,5 -1,6 0,5 1)))" );
}
/*
 * check that CGAL doesn't use the center of the polygon gB
 */
BOOST_AUTO_TEST_CASE( testLineString2 )
{
	std::auto_ptr< Geometry > gA( io::readWkt("LINESTRING(0 0,5 0)") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((0 0,1 -1,2 0,1 1,0 0))") );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	BOOST_CHECK_EQUAL( sum->asText(0), "MULTIPOLYGON(((6 1,1 1,0 0,1 -1,6 -1,7 0,6 1)))" );
}



BOOST_AUTO_TEST_CASE( testLineString3 )
{
	std::auto_ptr< Geometry > gA( io::readWkt("LINESTRING(5 5,0 5,5 0,0 0)") );
	std::auto_ptr< Geometry > gB( io::readWkt("POLYGON((-1 0,0 -1,1 0,0 1,-1 0))") );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	BOOST_CHECK_EQUAL( sum->asText(0), "MULTIPOLYGON(((5 1,2 4,5 4,6 5,5 6,0 6,-1 5,0 4,3 1,0 1,-1 0,0 -1,5 -1,6 0,5 1)))" );
}

BOOST_AUTO_TEST_CASE( testPolygonWithHole ){
	std::string wkt = "POLYGON((11.966308 -10.211022,18.007885 1.872133,39.364158 2.434140,53.554839 -6.557975,43.438710 -22.856183,20.396416 -28.476254,5.643728 -25.525717,13.090323 -20.889158,32.479570 -21.310663,38.521147 -15.831093,46.248746 -9.087007,34.446595 -1.359409,22.784946 -14.988082,11.966308 -10.211022),(20.396416 -1.640412,15.900358 -7.260484,18.007885 -9.508513,22.644444 -9.368011,25.173477 -2.342921,20.396416 -1.640412),(41.050179 -0.797401,40.207168 -2.202419,47.934767 -6.557975,48.496774 -5.433961,41.050179 -0.797401))" ;
	std::auto_ptr< Geometry > gA( io::readWkt( wkt ) );
	std::auto_ptr< Geometry > gB( io::readWkt( "POLYGON((-1 0,0 -1,1 0,0 1,-1 0))" ) );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	BOOST_CHECK_EQUAL( sum->asText(6), "MULTIPOLYGON(((53.554839 -5.557975,39.364158 3.434140,18.007885 2.872133,17.007885 1.872133,10.966308 -10.211022,11.966308 -11.211022,22.784946 -15.988082,23.784946 -14.988082,34.539099 -2.419977,44.939408 -9.229702,38.521147 -14.831093,32.479570 -20.310663,13.090323 -19.889158,5.643728 -24.525717,4.643728 -25.525717,5.643728 -26.525717,20.396416 -29.476254,43.438710 -23.856183,44.438710 -22.856183,54.554839 -6.557975,53.554839 -5.557975),(23.881857 -3.152977,21.997385 -8.387619,18.068659 -8.506671,16.900358 -7.260484,20.575363 -2.666728,23.881857 -3.152977)))" );

	// reverse orientation
	gA->as< Polygon >().reverse() ;
	sum = algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) ;
	BOOST_CHECK_EQUAL( sum->asText(6), "MULTIPOLYGON(((53.554839 -5.557975,39.364158 3.434140,18.007885 2.872133,17.007885 1.872133,10.966308 -10.211022,11.966308 -11.211022,22.784946 -15.988082,23.784946 -14.988082,34.539099 -2.419977,44.939408 -9.229702,38.521147 -14.831093,32.479570 -20.310663,13.090323 -19.889158,5.643728 -24.525717,4.643728 -25.525717,5.643728 -26.525717,20.396416 -29.476254,43.438710 -23.856183,44.438710 -22.856183,54.554839 -6.557975,53.554839 -5.557975),(23.881857 -3.152977,21.997385 -8.387619,18.068659 -8.506671,16.900358 -7.260484,20.575363 -2.666728,23.881857 -3.152977)))" );
}

BOOST_AUTO_TEST_CASE( testMultiPoint ){
	std::auto_ptr< Geometry > gA( io::readWkt( "MULTIPOINT(0 0,5 5)" ) );
	std::auto_ptr< Geometry > gB( io::readWkt( "POLYGON((-1 0,0 -1,1 0,0 1,-1 0))" ) );

	std::auto_ptr< Geometry > sum( algorithm::minkowskiSum( *gA, gB->as< Polygon >() ) );
	BOOST_CHECK_EQUAL( sum->asText(0), "MULTIPOLYGON(((0 1,-1 0,0 -1,1 0,0 1)),((5 6,4 5,5 4,6 5,5 6)))" );
}





BOOST_AUTO_TEST_SUITE_END()

