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

#include <SFCGAL/Kernel.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/transform/ForceZOrderPoints.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_transform_ForceZOrderPointsTest )

BOOST_AUTO_TEST_CASE( simple )
{
	std::auto_ptr<Geometry> g1 = io::readWkt( "POLYGON((0 0,0 1,1 1,1 0,0 0))" );

	const Polygon& p = g1->as<Polygon>();
	BOOST_CHECK( ! p.isCounterClockWiseOriented() );

	transform::ForceZOrderPoints forceZ;
	g1->accept( forceZ );

	BOOST_CHECK( g1->is3D() );
	BOOST_CHECK( g1->as<Polygon>().isCounterClockWiseOriented() );
}



BOOST_AUTO_TEST_SUITE_END()




