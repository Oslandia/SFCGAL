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
            = algorithm::lineSegment( io::readWkt( wkt )->as<LineString>()
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

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_LineSegmentTest )

BOOST_AUTO_TEST_CASE( testIssue130 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.3, 0.7, "LINESTRING Z(0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.0, 0.7, "LINESTRING Z(0 0 0, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward2D1 )
{
    test( "LINESTRING(0 0, 0 10)", 0.0, 0.7, "LINESTRING(0 0, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeEnd1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.0, -0.3, "LINESTRING Z(0 0 0, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeEnd2D1 )
{
    test( "LINESTRING(0 0, 0 10)", 0.0, -0.3, "LINESTRING(0 0, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeStart1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -1.0, 0.7, "LINESTRING Z(0 0 0, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeStart2D1 )
{
    test( "LINESTRING(0 0, 0 10)", -1.0, 0.7, "LINESTRING(0 0, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeEnds1 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -1.0,- 0.3, "LINESTRING Z(0 0 0, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeEnds2D1 )
{
    test( "LINESTRING(0 0, 0 10)", -1.0,- 0.3, "LINESTRING(0 0, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.3, 1.0, "LINESTRING Z(0 0 3, 0 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward2D2 )
{
    test( "LINESTRING(0 0, 0 10)", 0.3, 1.0, "LINESTRING(0 3, 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeStart2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -0.7, 1.0, "LINESTRING Z(0 0 3, 0 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeStart2D2 )
{
    test( "LINESTRING(0 0, 0 10)", -0.7, 1.0, "LINESTRING(0 3, 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.3, 1.0, "LINESTRING Z(0 0 3, 0 0 7, 0 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward2D3 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 0.3, 1.0, "LINESTRING(0 3, 0 7, 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeStart3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", -0.7, 1.0, "LINESTRING Z(0 0 3, 0 0 7, 0 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeStart2D3 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", -0.7, 1.0, "LINESTRING(0 3, 0 7, 0 10)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.0, 0.7, "LINESTRING Z(0 0 0, 0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward2D4 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 0.0, 0.7, "LINESTRING(0 0, 0 3, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardStart4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", -1.0, 0.7, "LINESTRING Z(0 0 0, 0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardStart2D4 )
{
    test( "LINESTRING(0 0,  0 3, 0 7, 0 10)", -1.0, 0.7, "LINESTRING(0 0, 0 3, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardEnd4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.0, -0.3, "LINESTRING Z(0 0 0, 0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardEnd2D4 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 0.0, -0.3, "LINESTRING(0 0, 0 3, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardEnds4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", -1.0, -0.3, "LINESTRING Z(0 0 0, 0 0 3, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardEnds2D4 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", -1.0, -0.3, "LINESTRING(0 0, 0 3, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", 0.3, 0.7, "LINESTRING Z(0 0 3, 0 0 5, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForward2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", 0.3, 0.7, "LINESTRING(0 3, 0 5, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeStart5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", -0.7, 0.7, "LINESTRING Z(0 0 3, 0 0 5, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeStart2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", -0.7, 0.7, "LINESTRING(0 3, 0 5, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeEnd5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", 0.3, -0.3, "LINESTRING Z(0 0 3, 0 0 5, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeEnd2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", 0.3, -0.3, "LINESTRING(0 3, 0 5, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeEnds5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", -0.7, -0.3, "LINESTRING Z(0 0 3, 0 0 5, 0 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenForwardNegativeEnds2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", -0.7, -0.3, "LINESTRING(0 3, 0 5, 0 7)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.7, 0.0, "LINESTRING Z(0 0 7, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward2D2 )
{
    test( "LINESTRING(0 0, 0 10)", 0.7, 0.0, "LINESTRING(0 7, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeStart2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -0.3, 0.0, "LINESTRING Z(0 0 7, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeStart2D2 )
{
    test( "LINESTRING(0 0, 0 10)", -0.3, 0.0, "LINESTRING(0 7, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnd2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.7, -1.0, "LINESTRING Z(0 0 7, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnd2D2 )
{
    test( "LINESTRING(0 0, 0 10)", 0.7, -1.0, "LINESTRING(0 7, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnds2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -0.3, -1.0, "LINESTRING Z(0 0 7, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnds2D2 )
{
    test( "LINESTRING(0 0, 0 10)", -0.3, -1.0, "LINESTRING(0 7, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.3, 1.0, "LINESTRING Z(0 0 10, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward2D3 )
{
    test( "LINESTRING(0 0, 0 10)", 0.3, 1.0, "LINESTRING(0 10, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeStart3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", -0.7, 1.0, "LINESTRING Z(0 0 10, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeStart2D3 )
{
    test( "LINESTRING(0 0, 0 10)", -0.7, 1.0, "LINESTRING(0 10, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 1.0, 0.3, "LINESTRING Z(0 0 10, 0 0 7, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward2D4 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 1.0, 0.3, "LINESTRING(0 10, 0 7, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnd4 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 1.0, -0.7, "LINESTRING Z(0 0 10, 0 0 7, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnd2D4 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 1.0, -0.7, "LINESTRING(0 10, 0 7, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.7, 0.0, "LINESTRING Z(0 0 7, 0 0 3, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 0.7, 0.0, "LINESTRING(0 7, 0 3, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeStart5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", -0.3, 0.0, "LINESTRING Z(0 0 7, 0 0 3, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeStart2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", -0.3, 0.0, "LINESTRING(0 7, 0 3, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnd5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", 0.7, -1.0, "LINESTRING Z(0 0 7, 0 0 3, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnd2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", 0.7, -1.0, "LINESTRING(0 7, 0 3, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnds5 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 7, 0 0 10)", -0.3, -1.0, "LINESTRING Z(0 0 7, 0 0 3, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnds2D5 )
{
    test( "LINESTRING(0 0, 0 3, 0 7, 0 10)", -0.3, -1.0, "LINESTRING(0 7, 0 3, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward6 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", 0.7, 0.3, "LINESTRING Z(0 0 7, 0 0 5, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward2D6 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", 0.7, 0.3, "LINESTRING(0 7, 0 5, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeStart6 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", -0.3, 0.3, "LINESTRING Z(0 0 7, 0 0 5, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeStart2D6 )
{
    test( "LINESTRING Z(0 0, 0 3, 0 5, 0 7, 0 10)", -0.3, 0.3, "LINESTRING(0 7, 0 5, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnd6 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", 0.7, -0.7, "LINESTRING Z(0 0 7, 0 0 5, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnd2D6 )
{
    test( "LINESTRING(0 0, 0 3, 0 5, 0 7, 0 10)", 0.7, -0.7, "LINESTRING(0 7, 0 5, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnds6 )
{
    test( "LINESTRING Z(0 0 0, 0 0 3, 0 0 5, 0 0 7, 0 0 10)", -0.3, -0.7, "LINESTRING Z(0 0 7, 0 0 5, 0 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnds2D6 )
{
    test( "LINESTRING Z(0 0, 0 3, 0 5, 0 7, 0 10)", -0.3, -0.7, "LINESTRING(0 7, 0 5, 0 3)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward7 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 1.0, 0.0, "LINESTRING Z(0 0 10, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackward2D7 )
{
    test( "LINESTRING(0 0, 0 10)", 1.0, 0.0, "LINESTRING(0 10, 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnd7 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 1.0, -1.0, "LINESTRING Z(0 0 10, 0 0 0)");
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenBackwardNegativeEnd2D7 )
{
    test( "LINESTRING(0 0, 0 10)", 1.0, -1.0, "LINESTRING(0 10, 0 0)");
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

BOOST_AUTO_TEST_CASE( testLineSegmentOpenEmptySegment2D1 )
{
    test( "LINESTRING(0 0, 0 10)", 0.0, 0.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenEmptySegment2 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 0.5, 0.5, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenEmptySegment2D2 )
{
    test( "LINESTRING(0 0, 0 10)", 0.5, 0.5, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenEmptySegment3 )
{
    test( "LINESTRING Z(0 0 0, 0 0 10)", 1.0, 1.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentOpenEmptySegment2D3 )
{
    test( "LINESTRING(0 0, 0 10)", 1.0, 1.0, "LINESTRING EMPTY", false, true );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosed1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.0
        , 0.7
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosed2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.0
        , 0.7
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeStart1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -1.0
        , 0.7
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeStart2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -1.0
        , 0.7
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnd1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.0
        , -0.3
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnd2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.0
        , -0.3
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnds1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -1.0
        , -0.3
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnds2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -1.0
        , -0.3
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
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

BOOST_AUTO_TEST_CASE( testLineSegmentClosed2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.1
        , 0.7
        , "LINESTRING(1 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeStart2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.9
        , 0.7
        , "LINESTRING Z(1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeStart2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.9
        , 0.7
        , "LINESTRING(1 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnd2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.1
        , -0.3
        , "LINESTRING Z(1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnd2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.1
        , -0.3
        , "LINESTRING(1 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnds2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.9
        , -0.3
        , "LINESTRING Z(1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnds2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.9
        , -0.3
        , "LINESTRING(1 0, 2.5 0, 2.5 2.5, 0.5 2.5)"
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

BOOST_AUTO_TEST_CASE( testLineSegmentClosed2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.3
        , 0.7
        , "LINESTRING(2.5 0.5, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeStart3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.7
        , 0.7
        , "LINESTRING Z(1 1 1, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeStart2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.7
        , 0.7
        , "LINESTRING(2.5 0.5, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnd3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.3
        , -0.3
        , "LINESTRING Z(1 1 1, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnd2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.3
        , -0.3
        , "LINESTRING(2.5 0.5, 2.5 2.5, 0.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnds3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.7
        , -0.3
        , "LINESTRING Z(1 1 1, 1 1 3, 0 1 3, 0 0 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnds2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.7
        , -0.3
        , "LINESTRING(2.5 0.5, 2.5 2.5, 0.5 2.5)"
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

BOOST_AUTO_TEST_CASE( testLineSegmentClosed2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.5
        , 0.8
        , "LINESTRING(2.5 2.5, 0 2.5, 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeStart4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.5
        , 0.8
        , "LINESTRING Z(1 1 3, 0 1 3, 0 0 3, 0 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeStart2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.5
        , 0.8
        , "LINESTRING(2.5 2.5, 0 2.5, 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnd4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.5
        , -0.2
        , "LINESTRING Z(1 1 3, 0 1 3, 0 0 3, 0 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnd2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.5
        , -0.2
        , "LINESTRING(2.5 2.5, 0 2.5, 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnds4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.5
        , -0.2
        , "LINESTRING Z(1 1 3, 0 1 3, 0 0 3, 0 0 2)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeEnds2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.5
        , -0.2
        , "LINESTRING(2.5 2.5, 0 2.5, 0 2)"
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

BOOST_AUTO_TEST_CASE( testLineSegmentClosed2D5 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.0
        , 1.0
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeStart5 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -1.0
        , 1.0
        , "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedNegativeStart2D5 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -1.0
        , 1.0
        , "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
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

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplement2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , 0.0
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeStart1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , 0.0
        , "LINESTRING Z(0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeStart2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , 0.0
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnd1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , -1.0
        , "LINESTRING Z(0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnd2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , -1.0
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnds1 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , -1.0
        , "LINESTRING Z(0 0 3, 0 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnds2D1 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , -1.0
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0)"
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

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplement2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , 0.1
        , "LINESTRING(0.5 2.5, 0 2.5, 0 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeStart2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , 0.1
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeStart2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , 0.1
        , "LINESTRING(0.5 2.5, 0 2.5, 0 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnd2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , -0.9
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnd2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , -0.9
        , "LINESTRING(0.5 2.5, 0 2.5, 0 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnds2 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , -0.9
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnds2D2 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , -0.9
        , "LINESTRING(0.5 2.5, 0 2.5, 0 1)"
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

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplement2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , 0.3
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0, 2.5 0, 2.5 0.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeStart3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , 0.3
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0, 1 1 0, 1 1 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeStart2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , 0.3
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0, 2.5 0, 2.5 0.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnd3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.7
        , -0.7
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0, 1 1 0, 1 1 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnd2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.7
        , -0.7
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0, 2.5 0, 2.5 0.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnds3 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.3
        , -0.7
        , "LINESTRING Z(0 0 3, 0 0 0, 1 0 0, 1 1 0, 1 1 1)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnds2D3 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.3
        , -0.7
        , "LINESTRING(0.5 2.5, 0 2.5, 0 0, 2.5 0, 2.5 0.5)"
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

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplement2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.8
        , 0.5
        , "LINESTRING(0 2, 0 0, 2.5 0, 2.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeStart4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.2
        , 0.5
        , "LINESTRING Z(0 0 2, 0 0 0, 1 0 0, 1 1 0, 1 1 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeStart2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.2
        , 0.5
        , "LINESTRING(0 2, 0 0, 2.5 0, 2.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnd4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 0.8
        , -0.5
        , "LINESTRING Z(0 0 2, 0 0 0, 1 0 0, 1 1 0, 1 1 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnd2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 0.8
        , -0.5
        , "LINESTRING(0 2, 0 0, 2.5 0, 2.5 2.5)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnds4 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , -0.2
        , -0.5
        , "LINESTRING Z(0 0 2, 0 0 0, 1 0 0, 1 1 0, 1 1 3)"
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnds2D4 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , -0.2
        , -0.5
        , "LINESTRING(0 2, 0 0, 2.5 0, 2.5 2.5)"
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

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplement2D5 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 1.0
        , 0.0
        , "LINESTRING EMPTY"
        , false
        , true
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnd5 )
{
    test( "LINESTRING Z(0 0 0, 1 0 0, 1 1 0, 1 1 3, 0 1 3, 0 0 3, 0 0 0)"
        , 1.0
        , -1.0
        , "LINESTRING EMPTY"
        , false
        , true
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentClosedComplementNegativeEnd2D5 )
{
    test( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)"
        , 1.0
        , -1.0
        , "LINESTRING EMPTY"
        , false
        , true
        );
}

BOOST_AUTO_TEST_CASE( testLineSegmentInvalidStartFraction )
{
    BOOST_CHECK_THROW(
        algorithm::lineSegment(
            io::readWkt( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)" )->as<LineString>()
                       , 1.1
                       , 1.0
                       ),
        Exception
    );
}

BOOST_AUTO_TEST_CASE( testLineSegmentInvalidNegativeStartFraction )
{
    BOOST_CHECK_THROW(
        algorithm::lineSegment(
            io::readWkt( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)" )->as<LineString>()
                       , -1.1
                       , 1.0
                       ),
        Exception
    );
}

BOOST_AUTO_TEST_CASE( testLineSegmentInvalidEndFraction )
{
    BOOST_CHECK_THROW(
        algorithm::lineSegment(
            io::readWkt( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)" )->as<LineString>()
                       , 0.0
                       , 1.1
                       ),
        Exception
    );
}

BOOST_AUTO_TEST_CASE( testLineSegmentInvalidNegativeEndFraction )
{
    BOOST_CHECK_THROW(
        algorithm::lineSegment(
            io::readWkt( "LINESTRING(0 0, 2.5 0, 2.5 2.5, 0 2.5, 0 0)" )->as<LineString>()
                       , 0.0
                       , -1.1
                       ),
        Exception
    );
}

BOOST_AUTO_TEST_SUITE_END()

