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

#include <SFCGAL/types.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/mesh/SurfaceMesh.h>
#include <SFCGAL/mesh/connection.h>
#include <SFCGAL/triangulate/TriangulatedMeshBuilder.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;
using namespace SFCGAL::mesh ;
using namespace SFCGAL::triangulate ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_Connected )

BOOST_AUTO_TEST_CASE( allFine )
{
    std::unique_ptr< Geometry<Epeck> > geom( io::readWkt<Epeck>(
                                         "POLYHEDRALSURFACE(((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                                   ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                                   ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                                   ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                                   ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                                   ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1)))" ) );

    SurfaceMesh<Epeck> mesh;
    TriangulatedMeshBuilder<Epeck> meshBuilder;
    meshBuilder.addPolygons(
        geom->as< PolyhedralSurface<Epeck> >().begin(),
        geom->as< PolyhedralSurface<Epeck> >().end()
    );
    meshBuilder.getMesh(mesh);

    BOOST_CHECK_MESSAGE( isConnected( mesh ) , "not connected" );
    BOOST_CHECK_MESSAGE( isClosed( mesh ) , "not closed" );
}


BOOST_AUTO_TEST_CASE( notConnected )
{
    std::unique_ptr< Geometry<Epeck> > geom ( io::readWkt<Epeck>(
                                         "POLYHEDRALSURFACE(((0 0 -1, 0 1 -1, 1 1 -1, 1 0 -1, 0 0 -1)),\
                                   ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                                   ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                                   ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                                   ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                                   ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1)))" ) );

    SurfaceMesh<Epeck> mesh;
    TriangulatedMeshBuilder<Epeck> meshBuilder;
    meshBuilder.addPolygons(
       geom->as< PolyhedralSurface<Epeck> >().begin(),
       geom->as< PolyhedralSurface<Epeck> >().end()
    );
    meshBuilder.getMesh(mesh);

    BOOST_CHECK_MESSAGE( !isConnected( mesh ) , "connected" );
    BOOST_CHECK_MESSAGE( !isClosed( mesh ) , "closed" );
}

BOOST_AUTO_TEST_CASE( notClosed )
{
    std::unique_ptr< Geometry<Epeck> > geom ( io::readWkt<Epeck>(
                                         "POLYHEDRALSURFACE(((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                                   ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                                   ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                                   ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                                   ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)))" ) );

   SurfaceMesh<Epeck> mesh;
   TriangulatedMeshBuilder<Epeck> meshBuilder;
   meshBuilder.addPolygons(
      geom->as< PolyhedralSurface<Epeck> >().begin(),
      geom->as< PolyhedralSurface<Epeck> >().end()
   );
   meshBuilder.getMesh(mesh);
   
    BOOST_CHECK_MESSAGE( isConnected( mesh ) , "not connected" );
    BOOST_CHECK_MESSAGE( !isClosed( mesh ) , "closed" );

}


BOOST_AUTO_TEST_SUITE_END()
