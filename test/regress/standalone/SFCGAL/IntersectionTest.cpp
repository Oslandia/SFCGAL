/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2018 Oslandia <infos@oslandia.com>
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
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/Geometry.h>
#include <SFCGAL/algorithm/extrude.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/MultiSolid.h>

#include "../../../test_config.h"

#include <boost/test/unit_test.hpp>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_IntersectionTest )

//
// https://trac.osgeo.org/postgis/ticket/4157
BOOST_AUTO_TEST_CASE( test_postgis_4157 )
{
    std::unique_ptr< Geometry > g1( io::readWkt( "POLYGON Z (("
                                               "122395.299 489126.697 8.61546664325712,"
                                               "122389.298 489128.73 8.55588025324629,"
                                               "122391.489 489135.198 8.5526708028059,"
                                               "122397.49 489133.165 8.61225719281685,"
                                               "122395.299 489126.697 8.61546664325712))" ));
    std::unique_ptr< Geometry > g2( io::readWkt( "POLYHEDRALSURFACE Z ((("
                                               "122390.998245685 489133.068537491 0,"
                                               "122391.003145022 489133.066423547 0,"
                                               "122391.003145022 489133.066423547 10,"
                                               "122390.998245685 489133.068537491 10,"
                                               "122390.998245685 489133.068537491 0"
                                               ")),(("
                                               "122391.003145022 489133.066423547 0,"
                                               "122383.269575402 489114.842869866 0,"
                                               "122383.269575402 489114.842869866 10,"
                                               "122391.003145022 489133.066423547 10,"
                                               "122391.003145022 489133.066423547 0"
                                               ")))" ) );

    algorithm::intersection3D( *g1, *g2 );
}

BOOST_AUTO_TEST_CASE( test_sfcgal_issue_200 )
{
  std::unique_ptr< SFCGAL::Geometry > poly1 = SFCGAL::io::readWkt( "POLYGON(( 0 0"
								           ", 1 0"
								           ", 1 1"
								           ", 0 1"
								           ", 0 0"
								           "))"
								 );

  std::unique_ptr< SFCGAL::Geometry > solid1 = SFCGAL::algorithm::extrude( *poly1
									 , 0.0
									 , 0.0
									 , 1.0
									 );

  std::unique_ptr< SFCGAL::Geometry > poly2 = SFCGAL::io::readWkt( "POLYGON(( -1 0.2"
								           ", 1.8 0.2"
								           ", 1.8 0.4"
								           ", -0.8 0.4"
								           ", -0.8 0.6"
								           ", 1.8 0.6"
								           ", 1.8 0.8"
								           ", -1 0.8"
								           ", -1 0.2"
								           "))"
								 );

  std::unique_ptr< SFCGAL::Geometry > solid2 = SFCGAL::algorithm::extrude( *poly2
									 , 0.0
									 , 0.0
									 , 1.0
									 );

  std::unique_ptr< SFCGAL::Geometry > inx = SFCGAL::algorithm::intersection3D( *solid1
									     , *solid2
									     );

  BOOST_CHECK( algorithm::isValid( *inx ) );
  BOOST_CHECK_EQUAL( inx->geometryTypeId(), TYPE_MULTISOLID );
  BOOST_CHECK_EQUAL( inx->as<MultiSolid>().numGeometries(), 2 );
}

BOOST_AUTO_TEST_SUITE_END()
