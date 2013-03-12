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
#include <fstream>

#include <SFCGAL/all.h>
#include <SFCGAL/io/asc.h>

#include "../../../test_config.h"

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_ASCTest )

BOOST_AUTO_TEST_CASE( testReadExample )
{
	std::string filename( SFCGAL_TEST_DIRECTORY );
	filename += "/regress/data/example.asc" ;

	std::ifstream ifs( filename.c_str() );
	BOOST_REQUIRE( ifs.good() ) ;
	std::auto_ptr< Grid > grid( io::readASC( ifs ) ) ;
	BOOST_CHECK_EQUAL( grid->ncols(), 4U );
	BOOST_CHECK_EQUAL( grid->nrows(), 6U );

	BOOST_CHECK_EQUAL( grid->dx(), 50.0 );
	BOOST_CHECK_EQUAL( grid->dy(), 50.0 );

	Envelope limits = grid->limits();
	BOOST_CHECK_EQUAL( limits.xMin(), 0.0 );
	BOOST_CHECK_EQUAL( limits.xMax(), 200.0 );
	BOOST_CHECK_EQUAL( limits.yMin(), 0.0 );
	BOOST_CHECK_EQUAL( limits.yMax(), 300.0 );

	// first line
	BOOST_CHECK( isNaN( grid->z(0,0) ) );
	BOOST_CHECK( isNaN( grid->z(0,1) ) );
	BOOST_CHECK_EQUAL( grid->z(0,2), 5.0 );
	BOOST_CHECK_EQUAL( grid->z(0,3), 2.0 );

	// last line
	BOOST_CHECK_EQUAL( grid->z(5,0), 13.0 );
	BOOST_CHECK_EQUAL( grid->z(5,1), 5.0 );
	BOOST_CHECK_EQUAL( grid->z(5,2), 1.0 );
	BOOST_CHECK( isNaN( grid->z(5,3) ) );

	// pixel is area convention
	Point a00 = grid->point(0,0);
	BOOST_CHECK_EQUAL( a00.x(), 25.0 );
	BOOST_CHECK_EQUAL( a00.y(), 275.0 );
}


BOOST_AUTO_TEST_SUITE_END()




