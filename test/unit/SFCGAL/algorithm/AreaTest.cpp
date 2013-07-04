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
#include <SFCGAL/algorithm/area.h>

#include <SFCGAL/detail/tools/Registry.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_AreaTest )


BOOST_AUTO_TEST_CASE( testEmpty2D3D )
{
	tools::Registry & registry = tools::Registry::instance() ;
	std::vector< std::string > typeNames = tools::Registry::instance().getGeometryTypes();
	for ( size_t i = 0; i < typeNames.size(); i++ ){
		BOOST_TEST_MESSAGE( typeNames[i] ) ;

		std::auto_ptr< Geometry > g( registry.newGeometryByTypeName( typeNames[i] ) ) ;
		BOOST_REQUIRE( g.get() != NULL ) ;
		BOOST_CHECK_EQUAL( algorithm::area( *g ), 0.0 );
		BOOST_CHECK_EQUAL( algorithm::area3D( *g ), 0.0 );
	}
}



BOOST_AUTO_TEST_CASE( testSignedArea2D_lineString )
{
	LineString lineString ;
	lineString.addPoint( Point(0.0,0.0) );
	lineString.addPoint( Point(1.0,0.0) );
	lineString.addPoint( Point(1.0,1.0) );
	lineString.addPoint( Point(0.0,1.0) );
	lineString.addPoint( lineString.startPoint() );

	BOOST_CHECK_EQUAL( algorithm::signedArea( lineString ),  1.0 );
	lineString.reverse() ;
	BOOST_CHECK_EQUAL( algorithm::signedArea( lineString ), -1.0 );
}

BOOST_AUTO_TEST_CASE( testSignedArea2D_triangle )
{
	Triangle triangle(
		Point(0.0,0.0),
		Point(1.0,0.0),
		Point(1.0,1.0)
	) ;

	BOOST_CHECK_EQUAL( algorithm::signedArea( triangle ),  0.5 );
	triangle.reverse() ;
	BOOST_CHECK_EQUAL( algorithm::signedArea( triangle ), -0.5 );
}




// must return 0.0
BOOST_AUTO_TEST_CASE( testPoint2D3D )
{
	BOOST_CHECK_EQUAL( algorithm::area( Point(3.0,4.0) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::area3D( Point(3.0,4.0,5.0) ), 0.0 );
}
// must return 0.0
BOOST_AUTO_TEST_CASE( testLineString2D3D )
{
	BOOST_CHECK_EQUAL( algorithm::area( LineString(Point(0.0,0.0),Point(1.0,1.0)) ), 0.0 );
	BOOST_CHECK_EQUAL( algorithm::area3D( LineString(Point(0.0,0.0,0.0),Point(1.0,1.0,1.0)) ), 0.0 );
}

// must return 0.0
BOOST_AUTO_TEST_CASE( testArea2D_PolygonWithHoleWithBadOrientation )
{
	Polygon polygon ;

	// exterior ring
	{
		LineString ring ;
		ring.addPoint( Point(0.0,0.0) );
		ring.addPoint( Point(5.0,0.0) );
		ring.addPoint( Point(5.0,5.0) );
		ring.addPoint( Point(0.0,5.0) );
		ring.addPoint( ring.startPoint() );

		polygon.setExteriorRing( ring );
	}

	// hole 1
	{
		LineString ring ;
		ring.addPoint( Point(1.0,1.0) );
		ring.addPoint( Point(2.0,1.0) );
		ring.addPoint( Point(2.0,2.0) );
		ring.addPoint( Point(1.0,2.0) );
		ring.addPoint( ring.startPoint() );

		polygon.addRing( ring );
	}

	// hole 2
	{
		LineString ring ;
		ring.addPoint( Point(3.0,3.0) );
		ring.addPoint( Point(4.0,3.0) );
		ring.addPoint( Point(4.0,4.0) );
		ring.addPoint( Point(3.0,4.0) );
		ring.addPoint( ring.startPoint() );

		polygon.addRing( ring );
	}

	// 5x5 - 1 - 1 = 23
	BOOST_CHECK_EQUAL( algorithm::area3D( polygon ), 23.0 );
}



BOOST_AUTO_TEST_CASE( testArea3D_Triangle1 )
{
	Triangle triangle( Point(0.0,0.0,0.0), Point(0.0,0.0,1.0), Point(0.0,1.0, 0.0) );
	BOOST_CHECK_EQUAL( algorithm::area3D( triangle ), 0.5 );
}

BOOST_AUTO_TEST_CASE( testArea3D_Triangle2 )
{
	Triangle triangle( Point(0.0,0.0,0.0), Point(0.0,0.0,4.0), Point(0.0,4.0, 0.0) );
	BOOST_CHECK_EQUAL( algorithm::area3D( triangle ), 8.0 );
}

BOOST_AUTO_TEST_CASE( testArea2D_Triangle )
{
	Triangle triangle1( Point(0.0,0.0), Point(4.0,0.0), Point(4.0,4.0) );
	// the same, inverted
	Triangle triangle2( Point(0.0,0.0), Point(0.0,4.0), Point(4.0,4.0) );
	BOOST_CHECK_EQUAL( algorithm::area( triangle1 ), 8.0 );
	BOOST_CHECK_EQUAL( algorithm::area( triangle2 ), 8.0 );
}

BOOST_AUTO_TEST_CASE( testArea3D_Square1x1 )
{
	std::auto_ptr< Geometry > g( io::readWkt( "POLYGON((0.0 0.0 0.0,0.0 0.0 1.0,0.0 1.0 1.0,0.0 1.0 0.0,0.0 0.0 0.0))" ) );
	BOOST_CHECK_EQUAL( g->asText(1), "POLYGON((0.0 0.0 0.0,0.0 0.0 1.0,0.0 1.0 1.0,0.0 1.0 0.0,0.0 0.0 0.0))" );
	BOOST_CHECK_CLOSE( algorithm::area3D( *g ), 1.0, 1e-10 );
}

BOOST_AUTO_TEST_CASE( testArea3D_Square4X4 )
{
	std::string wkt( "POLYGON((0.0 0.0 0.0,0.0 0.0 4.0,0.0 4.0 4.0,0.0 4.0 0.0,0.0 0.0 0.0))" );
	std::auto_ptr< Geometry > g( io::readWkt( wkt ) );
	BOOST_CHECK_CLOSE( algorithm::area3D( *g ), 16.0, 1e-10 );
}

BOOST_AUTO_TEST_CASE( testArea3D_Square4X4WithHole )
{
	std::string wkt( "POLYGON((0.0 0.0 0.0,0.0 0.0 4.0,0.0 4.0 4.0,0.0 4.0 0.0,0.0 0.0 0.0),(0.0 2.0 2.0,0.0 3.0 2.0,0.0 3.0 3.0,0.0 2.0 3.0,0.0 2.0 2.0))" );
	std::auto_ptr< Geometry > g( io::readWkt( wkt ) );
	BOOST_CHECK_CLOSE( algorithm::area3D( *g ), 15.0, 1e-10 );
}


BOOST_AUTO_TEST_SUITE_END()

