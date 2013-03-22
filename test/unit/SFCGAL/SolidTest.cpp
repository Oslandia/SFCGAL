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
#include <SFCGAL/Kernel.h>

#include <SFCGAL/Solid.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_SolidTest )

BOOST_AUTO_TEST_CASE( solidConversionTest )
{
	// the unit cube where half of a cube has been substracted
	std::string gstr = "SOLID("
		// exterior shell
		"("
		"((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0))," // front face
		"((1 0 0,1 1 0,1 1 1,1 0 1,1 0 0))," // right face
		"((0 1 0,0 1 1,1 1 1,1 1 0,0 1 0))," // top face
		"((0 0 1,0 1 1,0 1 0,0 0 0,0 0 1))," // left face
		"((1 0 1,1 1 1,0 1 1,0 0 1,1 0 1))," // back face
		"((1 0 0,1 0 1,0 0 1,0 0 0,1 0 0))" // bottom face
		"),"
		// interior shell, a.k.a. a hole
		"("
		"((0 0 0,0 0.5 0,0.5 0.5 0,0.5 0 0,0 0 0))," // front face
		"((0.5 0 0,0.5 0.5 0,0.5 0.5 0.5,0.5 0 0.5,0.5 0 0))," // right face
		"((0 0.5 0,0 0.5 0.5,0.5 0.5 0.5,0.5 0.5 0,0 0.5 0))," // top face
		"((0 0 0.5,0 0.5 0.5,0 0.5 0,0 0 0,0 0 0.5))," // left face
		"((0.5 0 0.5,0.5 0.5 0.5,0 0.5 0.5,0 0 0.5,0.5 0 0.5))," // back face
		"((0.5 0 0,0.5 0 0.5,0 0 0.5,0 0 0,0.5 0 0))" // bottom face
		")"
		")";

	std::auto_ptr<Geometry> g( io::readWkt( gstr ));

	const Solid& solid = static_cast<const Solid&>( *g );
	CGAL::Nef_polyhedron_3< Kernel > nef( solid.toNef_polyhedron_3< Kernel >());
	// Here a Nef_polyhedron merges adjacent triangles
	BOOST_CHECK_EQUAL( nef.number_of_facets(), 9U );
}

BOOST_AUTO_TEST_SUITE_END()
