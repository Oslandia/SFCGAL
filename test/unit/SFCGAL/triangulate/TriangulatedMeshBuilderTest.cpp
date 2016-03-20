/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include <boost/test/unit_test.hpp>

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/mesh/SurfaceMesh.h>
#include <SFCGAL/triangulate/TriangulatedMeshBuilder.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;
using namespace SFCGAL::mesh ;
using namespace SFCGAL::triangulate ;

BOOST_AUTO_TEST_SUITE( SFCGAL_triangulate_TriangulatedMeshBuilderTest )

BOOST_AUTO_TEST_CASE( testSurfaceMesh )
{
    std::string wkt = "MULTIPOLYGON(((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0)),((2.0 0.0,3.0 0.0,3.0 1.0,2.0 1.0,2.0 0.0)))" ;
    std::unique_ptr< Geometry<Epeck> > g( io::readWkt<Epeck>( wkt ) );
    MultiPolygon<Epeck> polygons = g->as< MultiPolygon<Epeck> >();
    
    TriangulatedMeshBuilder<Epeck> meshBuilder;
    meshBuilder.addPolygons(polygons.begin(), polygons.end());
    SurfaceMesh<Epeck> mesh;
    meshBuilder.getMesh(mesh);
    BOOST_CHECK_EQUAL(mesh.number_of_faces(),4);
}



BOOST_AUTO_TEST_SUITE_END()




