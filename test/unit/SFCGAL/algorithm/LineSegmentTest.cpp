/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2020 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2020 IGN (http://www.ign.fr)
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

// Boost
#include <boost/test/unit_test.hpp>

// SFCGAL
#include <SFCGAL/LineString.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/lineSegment.h>
#include <SFCGAL/algorithm/covers.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

namespace
{
    void test( const std::string& wkt
             , double start_frac
             , double end_frac
	     , const std::string& expected_wkt
	     )
    {
        std::unique_ptr<Geometry> result
            = algorithm::lineSegment( io::readWkt( wkt )->as<LineString>()
                                    , start_frac
                                    , end_frac
                                    );
        std::unique_ptr<Geometry> expected = io::readWkt( expected_wkt );

        BOOST_CHECK( algorithm::covers3D( *result, *expected ) );
    }
} // ! anonymous namespace

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_LengthSegmentTest )

BOOST_AUTO_TEST_CASE( testIssue130 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.3, 0.7, "LINESTRING Z(0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_SUITE_END()

