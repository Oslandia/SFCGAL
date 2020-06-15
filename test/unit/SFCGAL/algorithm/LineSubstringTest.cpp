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

// Boost
#include <boost/test/unit_test.hpp>

// SFCGAL
#include <SFCGAL/LineString.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/lineSubstring.h>
#include <SFCGAL/algorithm/covers.h>
#include <SFCGAL/Exception.h>

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
            = algorithm::lineSubstring( io::readWkt( wkt )->as<LineString>()
                                      , start_frac
                                      , end_frac
                                      );
        std::unique_ptr<Geometry> expected = io::readWkt( expected_wkt );
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

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_LineSubstringTest )

BOOST_AUTO_TEST_CASE( testIssue130 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.3, 0.7, "LINESTRING Z(0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForward1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.0, 0.7, "LINESTRING Z(0 0 0, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForward2D1 )
{
    test( "LINESTRING(0 0, 0 10)", 0.0, 0.7, "LINESTRING(0 0, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeEnd1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.0, -0.3, "LINESTRING Z(0 0 0, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeEnd2D1 )
{
    test( "LINESTRING(0 0, 0 10)", 0.0, -0.3, "LINESTRING(0 0, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeStart1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -1.0, 0.7, "LINESTRING Z(0 0 0, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeStart2D1 )
{
    test( "LINESTRING(0 0, 0 10)", -1.0, 0.7, "LINESTRING(0 0, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeEnds1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -1.0,- 0.3, "LINESTRING Z(0 0 0, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeEnds2D1 )
{
    test( "LINESTRING(0 0, 0 10)", -1.0,- 0.3, "LINESTRING(0 0, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForward2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.3, 1.0, "LINESTRING Z(0 0 3, 0 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForward2D2 )
{
    test( "LINESTRING(0 0, 0 10)", 0.3, 1.0, "LINESTRING(0 3, 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeStart2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -0.7, 1.0, "LINESTRING Z(0 0 3, 0 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeStart2D2 )
{
    test( "LINESTRING(0 0, 0 10)", -0.7, 1.0, "LINESTRING(0 3, 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForward3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.3, 1.0, "LINESTRING Z(0 0 3, 0 0 7, 0 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForward2D3 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 0.3, 1.0, "LINESTRING(0 3, 0 7, 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeStart3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", -0.7, 1.0, "LINESTRING Z(0 0 3, 0 0 7, 0 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeStart2D3 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", -0.7, 1.0, "LINESTRING(0 3, 0 7, 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForward4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.0, 0.7, "LINESTRING Z(0 0 0, 0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForward2D4 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 0.0, 0.7, "LINESTRING(0 0, 0 3, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardStart4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", -1.0, 0.7, "LINESTRING Z(0 0 0, 0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardStart2D4 )
{
    test( "LINESTRING(0 0,  0 3, 0 7, 0 10)", -1.0, 0.7, "LINESTRING(0 0, 0 3, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardEnd4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.0, -0.3, "LINESTRING Z(0 0 0, 0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardEnd2D4 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 0.0, -0.3, "LINESTRING(0 0, 0 3, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardEnds4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", -1.0, -0.3, "LINESTRING Z(0 0 0, 0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardEnds2D4 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", -1.0, -0.3, "LINESTRING(0 0, 0 3, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForward5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", 0.3, 0.7, "LINESTRING Z(0 0 3, 0 0 5, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForward2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", 0.3, 0.7, "LINESTRING(0 3, 0 5, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeStart5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", -0.7, 0.7, "LINESTRING Z(0 0 3, 0 0 5, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeStart2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", -0.7, 0.7, "LINESTRING(0 3, 0 5, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeEnd5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", 0.3, -0.3, "LINESTRING Z(0 0 3, 0 0 5, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeEnd2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", 0.3, -0.3, "LINESTRING(0 3, 0 5, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeEnds5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", -0.7, -0.3, "LINESTRING Z(0 0 3, 0 0 5, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenForwardNegativeEnds2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", -0.7, -0.3, "LINESTRING(0 3, 0 5, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.7, 0.0, "LINESTRING Z(0 0 7, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward2D2 )
{
    test( "LINESTRING(0 0, 0 10)", 0.7, 0.0, "LINESTRING(0 7, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeStart2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -0.3, 0.0, "LINESTRING Z(0 0 7, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeStart2D2 )
{
    test( "LINESTRING(0 0, 0 10)", -0.3, 0.0, "LINESTRING(0 7, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnd2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.7, -1.0, "LINESTRING Z(0 0 7, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnd2D2 )
{
    test( "LINESTRING(0 0, 0 10)", 0.7, -1.0, "LINESTRING(0 7, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnds2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -0.3, -1.0, "LINESTRING Z(0 0 7, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnds2D2 )
{
    test( "LINESTRING(0 0, 0 10)", -0.3, -1.0, "LINESTRING(0 7, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.3, 1.0, "LINESTRING Z(0 0 10, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward2D3 )
{
    test( "LINESTRING(0 0, 0 10)", 0.3, 1.0, "LINESTRING(0 10, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeStart3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -0.7, 1.0, "LINESTRING Z(0 0 10, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeStart2D3 )
{
    test( "LINESTRING(0 0, 0 10)", -0.7, 1.0, "LINESTRING(0 10, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 1.0, 0.3, "LINESTRING Z(0 0 10, 0 0 7, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward2D4 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 1.0, 0.3, "LINESTRING(0 10, 0 7, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnd4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 1.0, -0.7, "LINESTRING Z(0 0 10, 0 0 7, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnd2D4 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 1.0, -0.7, "LINESTRING(0 10, 0 7, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.7, 0.0, "LINESTRING Z(0 0 7, 0 0 3, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 0.7, 0.0, "LINESTRING(0 7, 0 3, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeStart5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", -0.3, 0.0, "LINESTRING Z(0 0 7, 0 0 3, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeStart2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", -0.3, 0.0, "LINESTRING(0 7, 0 3, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnd5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.7, -1.0, "LINESTRING Z(0 0 7, 0 0 3, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnd2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 0.7, -1.0, "LINESTRING(0 7, 0 3, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnds5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", -0.3, -1.0, "LINESTRING Z(0 0 7, 0 0 3, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnds2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", -0.3, -1.0, "LINESTRING(0 7, 0 3, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward6 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", 0.7, 0.3, "LINESTRING Z(0 0 7, 0 0 5, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward2D6 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", 0.7, 0.3, "LINESTRING(0 7, 0 5, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeStart6 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", -0.3, 0.3, "LINESTRING Z(0 0 7, 0 0 5, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeStart2D6 )
{
    test( "LINESTRING Z(0 0, 0 3, 0 5, 0 7, 0 10)", -0.3, 0.3, "LINESTRING(0 7, 0 5, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnd6 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", 0.7, -0.7, "LINESTRING Z(0 0 7, 0 0 5, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnd2D6 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", 0.7, -0.7, "LINESTRING(0 7, 0 5, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnds6 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", -0.3, -0.7, "LINESTRING Z(0 0 7, 0 0 5, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnds2D6 )
{
    test( "LINESTRING Z(0 0, 0 3, 0 5, 0 7, 0 10)", -0.3, -0.7, "LINESTRING(0 7, 0 5, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward7 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 1.0, 0.0, "LINESTRING Z(0 0 10, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackward2D7 )
{
    test( "LINESTRING(0 0, 0 10)", 1.0, 0.0, "LINESTRING(0 10, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnd7 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 1.0, -1.0, "LINESTRING Z(0 0 10, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenBackwardNegativeEnd2D7 )
{
    test( "LINESTRING(0 0, 0 10)", 1.0, -1.0, "LINESTRING(0 10, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSubstringOfEmptyLine )
{
    test( "LINESTRING EMPTY", 0.0, 1.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSubstringOfEmptyLine2 )
{
    test( "LINESTRING EMPTY", 0.0, 0.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSubstringOfEmptyLine3 )
{
    test( "LINESTRING EMPTY", 1.0, 0.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenEmptySegment1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.0, 0.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenEmptySegment2D1 )
{
    test( "LINESTRING(0 0, 0 10)", 0.0, 0.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenEmptySegment2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.5, 0.5, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenEmptySegment2D2 )
{
    test( "LINESTRING(0 0, 0 10)", 0.5, 0.5, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenEmptySegment3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 1.0, 1.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSubstringOpenEmptySegment2D3 )
{
    test( "LINESTRING(0 0, 0 10)", 1.0, 1.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosed1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.0
        , 0.7
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosed2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.0
        , 0.7
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeStart1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -1.0
        , 0.7
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeStart2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -1.0
        , 0.7
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnd1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.0
        , -0.3
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnd2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.0
        , -0.3
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnds1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -1.0
        , -0.3
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnds2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -1.0
        , -0.3
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosed2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.1
        , 0.7
        , "LINESTRING Z(1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosed2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.1
        , 0.7
        , "LINESTRING(1 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeStart2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.9
        , 0.7
        , "LINESTRING Z(1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeStart2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.9
        , 0.7
        , "LINESTRING(1 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnd2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.1
        , -0.3
        , "LINESTRING Z(1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnd2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.1
        , -0.3
        , "LINESTRING(1 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnds2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.9
        , -0.3
        , "LINESTRING Z(1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnds2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.9
        , -0.3
        , "LINESTRING(1 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosed3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.3
        , 0.7
        , "LINESTRING Z(1 1 1, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosed2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.3
        , 0.7
        , "LINESTRING(2.5 0.5, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeStart3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.7
        , 0.7
        , "LINESTRING Z(1 1 1, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeStart2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.7
        , 0.7
        , "LINESTRING(2.5 0.5, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnd3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.3
        , -0.3
        , "LINESTRING Z(1 1 1, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnd2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.3
        , -0.3
        , "LINESTRING(2.5 0.5, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnds3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.7
        , -0.3
        , "LINESTRING Z(1 1 1, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnds2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.7
        , -0.3
        , "LINESTRING(2.5 0.5, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosed4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.5
        , 0.8
        , "LINESTRING Z(1 1 3, 0 1 3, 0 0 3, 0 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosed2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.5
        , 0.8
        , "LINESTRING(2.5 2.5, 0 2.5, 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeStart4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.5
        , 0.8
        , "LINESTRING Z(1 1 3, 0 1 3, 0 0 3, 0 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeStart2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.5
        , 0.8
        , "LINESTRING(2.5 2.5, 0 2.5, 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnd4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.5
        , -0.2
        , "LINESTRING Z(1 1 3, 0 1 3, 0 0 3, 0 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnd2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.5
        , -0.2
        , "LINESTRING(2.5 2.5, 0 2.5, 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnds4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.5
        , -0.2
        , "LINESTRING Z(1 1 3, 0 1 3, 0 0 3, 0 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeEnds2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.5
        , -0.2
        , "LINESTRING(2.5 2.5, 0 2.5, 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosed5 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.0
        , 1.0
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosed2D5 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.0
        , 1.0
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeStart5 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -1.0
        , 1.0
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedNegativeStart2D5 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -1.0
        , 1.0
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplement1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , 0.0
        , "LINESTRING Z(0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplement2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , 0.0
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeStart1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , 0.0
        , "LINESTRING Z(0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeStart2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , 0.0
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnd1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , -1.0
        , "LINESTRING Z(0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnd2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , -1.0
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnds1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , -1.0
        , "LINESTRING Z(0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnds2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , -1.0
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplement2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , 0.1
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplement2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , 0.1
        , "LINESTRING(0.5 2.5, 0 2.5, 0 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeStart2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , 0.1
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeStart2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , 0.1
        , "LINESTRING(0.5 2.5, 0 2.5, 0 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnd2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , -0.9
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnd2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , -0.9
        , "LINESTRING(0.5 2.5, 0 2.5, 0 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnds2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , -0.9
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnds2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , -0.9
        , "LINESTRING(0.5 2.5, 0 2.5, 0 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplement3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , 0.3
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0, 1 1 0, 1 1 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplement2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , 0.3
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0, 2.5 0, 2.5 0.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeStart3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , 0.3
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0, 1 1 0, 1 1 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeStart2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , 0.3
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0, 2.5 0, 2.5 0.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnd3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , -0.7
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0, 1 1 0, 1 1 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnd2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , -0.7
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0, 2.5 0, 2.5 0.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnds3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , -0.7
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0, 1 1 0, 1 1 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnds2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , -0.7
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0, 2.5 0, 2.5 0.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplement4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.8
        , 0.5
        , "LINESTRING Z(0 0 2, 0 0 0, 1 0 0, 1 1 0, 1 1 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplement2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.8
        , 0.5
        , "LINESTRING(0 2, 0 0, 2.5 0, 2.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeStart4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.2
        , 0.5
        , "LINESTRING Z(0 0 2, 0 0 0, 1 0 0, 1 1 0, 1 1 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeStart2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.2
        , 0.5
        , "LINESTRING(0 2, 0 0, 2.5 0, 2.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnd4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.8
        , -0.5
        , "LINESTRING Z(0 0 2, 0 0 0, 1 0 0, 1 1 0, 1 1 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnd2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.8
        , -0.5
        , "LINESTRING(0 2, 0 0, 2.5 0, 2.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnds4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.2
        , -0.5
        , "LINESTRING Z(0 0 2, 0 0 0, 1 0 0, 1 1 0, 1 1 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnds2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.2
        , -0.5
        , "LINESTRING(0 2, 0 0, 2.5 0, 2.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplement5 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 1.0
        , 0.0
        , "LINESTRING EMPTY"
        , false
        , true
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplement2D5 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 1.0
        , 0.0
        , "LINESTRING EMPTY"
        , false
        , true
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnd5 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 1.0
        , -1.0
        , "LINESTRING EMPTY"
        , false
        , true
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringClosedComplementNegativeEnd2D5 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 1.0
        , -1.0
        , "LINESTRING EMPTY"
        , false
        , true
        );
}

BOOST_AUTO_TEST_CASE( testLineSubstringInvalidStartFraction )
{
    BOOST_CHECK_THROW(
        algorithm::lineSubstring(
            io::readWkt( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)" )->as<LineString>()
                       , 1.1
                       , 1.0
                       ),
        Exception
    );
}

BOOST_AUTO_TEST_CASE( testLineSubstringInvalidNegativeStartFraction )
{
    BOOST_CHECK_THROW(
        algorithm::lineSubstring(
            io::readWkt( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)" )->as<LineString>()
                       , -1.1
                       , 1.0
                       ),
        Exception
    );
}

BOOST_AUTO_TEST_CASE( testLineSubstringInvalidEndFraction )
{
    BOOST_CHECK_THROW(
        algorithm::lineSubstring(
            io::readWkt( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)" )->as<LineString>()
                       , 0.0
                       , 1.1
                       ),
        Exception
    );
}

BOOST_AUTO_TEST_CASE( testLineSubstringInvalidNegativeEndFraction )
{
    BOOST_CHECK_THROW(
        algorithm::lineSubstring(
            io::readWkt( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)" )->as<LineString>()
                       , 0.0
                       , -1.1
                       ),
        Exception
    );
}

BOOST_AUTO_TEST_SUITE_END()

