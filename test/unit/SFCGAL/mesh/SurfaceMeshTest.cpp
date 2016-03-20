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

#include <SFCGAL/mesh/SurfaceMesh.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

using Mesh = mesh::SurfaceMesh<Epick> ;

BOOST_AUTO_TEST_SUITE( SFCGAL_mesh_SurfaceMeshTest )

BOOST_AUTO_TEST_CASE( testExtractTriangles )
{
    typedef Mesh::Vertex_index vertex_descriptor;
    typedef Mesh::Face_index face_descriptor;
    
    Mesh m;
    // u      x
    // +-----+
    // |    /|
    // |   / |
    // |  /  |
    // | /   |
    // |/    |
    // +-----+
    // v     w
    
    // Add the points as vertices
    vertex_descriptor u = m.add_vertex(Epick::Point_3(0,1,0));
    vertex_descriptor v = m.add_vertex(Epick::Point_3(0,0,0));
    vertex_descriptor w = m.add_vertex(Epick::Point_3(1,0,0));
    vertex_descriptor x = m.add_vertex(Epick::Point_3(1,1,0));
    /*face_descriptor uvx =*/ m.add_face(u,v,x);
    /*face_descriptor vwx =*/ m.add_face(v,w,x);
    
    std::vector< CGAL::Triangle_3<Epick> > triangles;
    mesh::extractTriangles(m,std::back_inserter(triangles));
    BOOST_CHECK_EQUAL(triangles.size(),2U);
}


//MultiSolid


BOOST_AUTO_TEST_SUITE_END()




