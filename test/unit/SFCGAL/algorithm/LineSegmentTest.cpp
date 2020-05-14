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
             , bool verify_using_covers = true
             , bool verify_is_empty = false
	     )
    {
        std::unique_ptr<Geometry> result
            = algorithm::lineSegment( io::readWkt( wkt )->as<LineString>()
                                    , start_frac
                                    , end_frac
                                    );
        std::unique_ptr<Geometry> expected = io::readWkt( expected_wkt );
        std::cout << result->asText(1) << std::endl;
        if ( verify_using_covers )
        {
            BOOST_CHECK( algorithm::covers3D( *result, *expected ) );
        }

        if ( verify_is_empty )
        {
            BOOST_CHECK( result->isEmpty() );
        }
    }
} // ! anonymous namespace

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_LineSegmentTest )

BOOST_AUTO_TEST_CASE( testIssue130 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.3, 0.7, "LINESTRING Z(0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.0, 0.7, "LINESTRING Z(0 0 0, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.3, 1.0, "LINESTRING Z(0 0 3, 0 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.3, 1.0, "LINESTRING Z(0 0 3, 0 0 7, 0 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.0, 0.7, "LINESTRING Z(0 0 0, 0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", 0.3, 0.7, "LINESTRING Z(0 0 3, 0 0 5, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.7, 0.0, "LINESTRING Z(0 0 7, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.3, 1.0, "LINESTRING Z(0 0 10, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 1.0, 0.3, "LINESTRING Z(0 0 10, 0 0 7, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.7, 0.0, "LINESTRING Z(0 0 7, 0 0 3, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward6 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", 0.7, 0.3, "LINESTRING Z(0 0 7, 0 0 5, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward7 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 1.0, 0.0, "LINESTRING Z(0 0 10, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOfEmptyLine )
{
    test( "LINESTRING EMPTY", 0.0, 1.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentOfEmptyLine2 )
{
    test( "LINESTRING EMPTY", 0.0, 0.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentOfEmptyLine3 )
{
    test( "LINESTRING EMPTY", 1.0, 0.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenEmptySegment1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.0, 0.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenEmptySegment2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.5, 0.5, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenEmptySegment3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 1.0, 1.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosed1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.0
        , 0.7
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosed2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.1
        , 0.7
        , "LINESTRING Z(1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosed3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.3
        , 0.7
        , "LINESTRING Z(1 1 1, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosed4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.5
        , 0.8
        , "LINESTRING Z(1 1 3, 0 1 3, 0 0 3, 0 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosed5 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.0
        , 1.0
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplement1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , 0.0
        , "LINESTRING Z(0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplement2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , 0.1
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplement3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , 0.3
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0, 1 1 0, 1 1 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplement4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.8
        , 0.5
        , "LINESTRING Z(0 0 2, 0 0 0, 1 0 0, 1 1 0, 1 1 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplement5 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 1.0
        , 0.0
        , "LINESTRING EMPTY"
        , false
        , true
        );
}


BOOST_AUTO_TEST_SUITE_END()

