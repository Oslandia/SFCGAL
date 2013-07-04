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

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/connection.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;
using namespace SFCGAL::algorithm ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_Connected )

BOOST_AUTO_TEST_CASE( allFine )
{
    std::auto_ptr< Geometry > geom ( io::readWkt(
                                         "POLYHEDRALSURFACE(((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                                   ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                                   ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                                   ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                                   ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                                   ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1)))" ) );

    SurfaceGraph graph( geom->as< PolyhedralSurface >() );
    BOOST_CHECK_MESSAGE( isConnected( graph ) , "not connected" );
    BOOST_CHECK_MESSAGE( isClosed( graph ) , "not closed" );
}

BOOST_AUTO_TEST_CASE( notConnected )
{
    std::auto_ptr< Geometry > geom ( io::readWkt(
                                         "POLYHEDRALSURFACE(((0 0 -1, 0 1 -1, 1 1 -1, 1 0 -1, 0 0 -1)),\
                                   ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                                   ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                                   ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                                   ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),\
                                   ((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1)))" ) );

    SurfaceGraph graph( geom->as< PolyhedralSurface >() );
    BOOST_CHECK_MESSAGE( !isConnected( graph ) , "connected" );
    BOOST_CHECK_MESSAGE( !isClosed( graph ) , "closed" );
}

BOOST_AUTO_TEST_CASE( notClosed )
{
    std::auto_ptr< Geometry > geom ( io::readWkt(
                                         "POLYHEDRALSURFACE(((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),\
                                   ((0 0 0, 0 0 1, 0 1 1, 0 1 0, 0 0 0)),\
                                   ((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),\
                                   ((1 1 1, 0 1 1, 0 0 1, 1 0 1, 1 1 1)),\
                                   ((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)))" ) );

    SurfaceGraph graph( geom->as< PolyhedralSurface >() );
    BOOST_CHECK_MESSAGE( isConnected( graph ) , "not connected" );
    BOOST_CHECK_MESSAGE( !isClosed( graph ) , "closed" );

}

BOOST_AUTO_TEST_SUITE_END()
