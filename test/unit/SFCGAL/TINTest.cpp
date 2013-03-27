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
#include <SFCGAL/Kernel.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/triangulate/triangulatePolygon.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_TINTest )

BOOST_AUTO_TEST_CASE( polyhedronConversionTest )
{
	// two unit squares sharing a common edge (1,0)-(1,1)
	std::string gstr = "POLYHEDRALSURFACE(((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)),"
		"((1 0 0,1 1 0,2 1 0,2 0 0,1 0 0)))";
	// the following surface would generate an exception, since the two polygons have opposite orientations
	// "POLYHEDRALSURFACE(((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)),((2 0 0,2 1 0,1 1 0,1 0 0,2 0 0)))";
	std::auto_ptr<Geometry> g( io::readWkt( gstr ));

	TriangulatedSurface tri;
	triangulate::triangulatePolygon3D( *g, tri );

	std::auto_ptr<CGAL::Polyhedron_3<Kernel> > poly( tri.toPolyhedron_3<Kernel, CGAL::Polyhedron_3<Kernel> >() );
	// we check the two squares share a common edge
	BOOST_CHECK_EQUAL( poly->size_of_facets(), 4U );
	BOOST_CHECK_EQUAL( poly->size_of_vertices(), 6U );
}

BOOST_AUTO_TEST_SUITE_END()




