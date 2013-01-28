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
#include <boost/test/unit_test.hpp>

#include "../../../test_config.h"

#include <fstream>
#include <boost/format.hpp>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/algorithm/area.h>
#include <SFCGAL/Transform.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;


class RotateCoordinate : public Transform {
public:

	virtual void transform( Point & p )
	{
		BOOST_ASSERT( ! p.isEmpty() );
		p = Point(
			p.is3D() ? p.z() : 0.0,
			p.x(),
			p.y()
		);
	}

};


BOOST_AUTO_TEST_SUITE( SFCGAL_AreaTest )

/**
 * Triangulate polygon and make some checks
 * @ŧodo Check inPolygon.area3D() == outPolygon.area3D();
 */
BOOST_AUTO_TEST_CASE( testComputeArea )
{
	std::string filename( SFCGAL_TEST_DIRECTORY );
	filename += "/regress/data/AreaTest.txt" ;

	std::ifstream ifs( filename.c_str() );
	BOOST_REQUIRE( ifs.good() ) ;

	std::string line;
	while ( std::getline( ifs, line ) ){
		if ( line[0] == '#' || line.empty() )
			continue ;

		std::istringstream iss(line);

		std::string id ;
		iss >> id ;

		double expectedArea ;
		iss >> expectedArea ;

		std::string inputWkt ;
		std::getline( iss, inputWkt ) ;

		std::auto_ptr< Geometry > g( io::readWkt(inputWkt) );
		double area = algorithm::area3D( *g ) ;
		BOOST_TEST_MESSAGE( boost::format("area( '%1%' ) = %2%") % inputWkt % area );

		RotateCoordinate rotateCoordinate ;
		g->accept(rotateCoordinate);
		double areaRotate = algorithm::area3D( *g ) ;

		//check area == areaRotate
		BOOST_CHECK_CLOSE( area, areaRotate, 0.5 );
		//check area == expectedArea
		BOOST_CHECK_CLOSE( area, expectedArea, 0.5 );
	}
}


BOOST_AUTO_TEST_SUITE_END()




