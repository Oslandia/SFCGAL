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
#include <SFCGAL/algorithm/ConsistentOrientationBuilder.h>
#include <SFCGAL/algorithm/orientation.h>


using namespace SFCGAL ;
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_ConsistentOrientationBuilderTest )

BOOST_AUTO_TEST_CASE( testOppositeTriangle )
{
	algorithm::ConsistentOrientationBuilder builder ;
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point( 1.0, 0.0, 0.0 ),
			Point( 0.0, 1.0, 0.0 )
		)
	);
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point(-1.0, 0.0, 0.0 ),
			Point( 0.0, 1.0, 0.0 )
		)
	);
	TriangulatedSurface triangulatedSurface = builder.buildTriangulatedSurface();
	BOOST_CHECK_EQUAL( triangulatedSurface.numGeometries(), 2U );
	BOOST_CHECK( algorithm::hasConsistentOrientation3D( triangulatedSurface ) );
}

BOOST_AUTO_TEST_CASE( testFourTriangle )
{
	algorithm::ConsistentOrientationBuilder builder ;
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point( 1.0, 0.0, 0.0 ),
			Point( 0.0, 1.0, 0.0 )
		)
	);
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point(-1.0, 0.0, 0.0 ),
			Point( 0.0, 1.0, 0.0 )
		)
	);
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point( 1.0, 0.0, 0.0 ),
			Point( 0.0,-1.0, 0.0 )
		)
	);
	builder.addTriangle(
		Triangle(
			Point( 0.0, 0.0, 0.0 ),
			Point(-1.0, 0.0, 0.0 ),
			Point( 0.0,-1.0, 0.0 )
		)
	);
	TriangulatedSurface triangulatedSurface = builder.buildTriangulatedSurface();
	BOOST_CHECK_EQUAL( triangulatedSurface.numGeometries(), 4U );
	BOOST_CHECK( algorithm::hasConsistentOrientation3D( triangulatedSurface ) );
}


BOOST_AUTO_TEST_SUITE_END()

