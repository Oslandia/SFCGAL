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

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/detail/TestGeometry.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_IsValid )

BOOST_AUTO_TEST_CASE( geometryIsValid )
{
    const std::vector< TestGeometry > testGeometry( createTestGeometries() );
    const std::size_t nbOfTest = testGeometry.size();

    for ( std::size_t t=0; t<nbOfTest; t++ ) {
        const TestGeometry& tg = testGeometry[t];
        //std::cerr << t << ":" << tg._wkt << "\n";
        std::auto_ptr< Geometry > g( io::readWkt( tg.wkt ) );
        Validity v = algorithm::isValid( *g );
        BOOST_CHECK_MESSAGE( v == tg.isValid, ( boost::format( "%d:%s should be %s (%s)%s%s : %s" ) % t % g->geometryType() % ( tg.isValid?"valid":"invalid" ) % tg.comment % ( v?".":", reason: " ) % v.reason() % tg.wkt ) );
    }

}

BOOST_AUTO_TEST_CASE( geometryWithNan )
{
    const double inf = std::numeric_limits<double>::infinity();
    const double quietNaN = std::numeric_limits<double>::quiet_NaN();
    const double sigNaN = std::numeric_limits<double>::signaling_NaN();

    BOOST_CHECK_THROW( Coordinate( quietNaN ,1.0,2.0 ), NonFiniteValueException  );
    BOOST_CHECK_THROW( Coordinate( inf,1.0,2.0 ), NonFiniteValueException  );
    BOOST_CHECK_THROW( Coordinate( sigNaN,1.0,2.0 ), NonFiniteValueException );
}
BOOST_AUTO_TEST_SUITE_END()
