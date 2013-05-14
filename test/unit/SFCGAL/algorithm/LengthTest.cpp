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
#include <SFCGAL/algorithm/length.h>


using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_LengthTest )

BOOST_AUTO_TEST_CASE( testZeroLength )
{
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("POINT(0.0 0.0)" ) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("LINESTRING EMPTY" ) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("POLYGON((0 0,0 1,1 1,1 0,0 0))" ) ), 0.0 );
}

BOOST_AUTO_TEST_CASE( testZeroLengthVertical )
{
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("LINESTRING(0.0 0.0 0.0,0.0 0.0 1.0)" ) ), 0.0 );
}

BOOST_AUTO_TEST_CASE( testLengthLineString )
{
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("LINESTRING(0.0 0.0,3.0 4.0)" ) ), 5.0 );
	BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt("LINESTRING(0.0 0.0,0.0 1.0,1.0 1.0)" ) ), 2.0 );
}

//-- 3D


BOOST_AUTO_TEST_CASE( test3DZeroLength )
{
	BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt("POINT(0.0 0.0)" ) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt("LINESTRING EMPTY" ) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt("POLYGON((0 0,0 1,1 1,1 0,0 0))" ) ), 0.0 );
}
BOOST_AUTO_TEST_CASE( test3DLengthVertical )
{
	BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt("LINESTRING(0.0 0.0 0.0,0.0 0.0 1.0)" ) ), 1.0 );
}
BOOST_AUTO_TEST_CASE( test3DLengthLineString )
{
	BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt("LINESTRING(0.0 0.0 0.0,0.0 1.0 0.0,0.0 1.0 1.0)" ) ), 2.0 );
}


//-- invalid type 2D

BOOST_AUTO_TEST_CASE( testLength_invalidType )
{
	std::vector< std::string > wkts ;
	wkts.push_back( "POINT(3.0 4.0)" );
	wkts.push_back( "TRIANGLE((0.0 0.0,1.0 0.0,1.0 1.0,0.0 0.0))" );
	wkts.push_back( "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 0.0))" );

	for ( size_t i = 0; i < wkts.size(); i++ ){
		BOOST_TEST_MESSAGE( wkts[i] );
		BOOST_CHECK_EQUAL( algorithm::length( *io::readWkt( wkts[i] ) ), 0.0 );
		BOOST_CHECK_EQUAL( algorithm::length3D( *io::readWkt( wkts[i] ) ), 0.0 );
	}

}
//-- invalid type 3D




BOOST_AUTO_TEST_SUITE_END()

