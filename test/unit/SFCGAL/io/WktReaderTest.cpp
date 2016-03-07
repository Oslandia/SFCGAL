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
#include <memory>
#include <string>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;
using namespace SFCGAL::io ;


BOOST_AUTO_TEST_SUITE( SFCGAL_io_WktReaderTest )

//-- WKT POINT

BOOST_AUTO_TEST_CASE( pointEmpty )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "POINT EMPTY" ) );
    BOOST_CHECK( g->isEmpty() );
}

BOOST_AUTO_TEST_CASE( pointXY )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "POINT(4.0 6.0)" ) );
    BOOST_CHECK( g->is< Point<Epeck> >() );
    BOOST_CHECK( ! g->isEmpty() );

    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().x(), 4.0 );
    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().y(), 6.0 );
}

BOOST_AUTO_TEST_CASE( pointXYZ_implicit )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "POINT(4.0 5.0 6.0)" ) );
    BOOST_CHECK( g->is< Point<Epeck> >() );
    BOOST_CHECK( ! g->isEmpty() );

    BOOST_CHECK( g->is3D() );
    BOOST_CHECK( ! g->isMeasured() );

    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().x(), 4.0 );
    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().y(), 5.0 );
    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().z(), 6.0 );
}

BOOST_AUTO_TEST_CASE( pointXYZ_explicit )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "POINT Z(4.0 5.0 6.0)" ) );
    BOOST_CHECK( g->is< Point<Epeck> >() );
    BOOST_CHECK( ! g->isEmpty() );

    BOOST_CHECK( g->is3D() );
    BOOST_CHECK( ! g->isMeasured() );

    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().x(), 4.0 );
    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().y(), 5.0 );
    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().z(), 6.0 );
}


BOOST_AUTO_TEST_CASE( pointXYM_explicit )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "POINT M(4.0 5.0 6.0)" ) );
    BOOST_CHECK( g->is< Point<Epeck> >() );
    BOOST_CHECK( ! g->isEmpty() );

    BOOST_CHECK( ! g->is3D() );
    BOOST_CHECK( g->isMeasured() );

    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().x(), 4.0 );
    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().y(), 5.0 );
    BOOST_CHECK_EQUAL( g->as< Point<Epeck> >().m(), 6.0 );
}

//-- WKT LINESTRING


BOOST_AUTO_TEST_CASE( lineStringEmpty )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "LINESTRING EMPTY" ) );
    BOOST_CHECK( g->is< LineString<Epeck> >() );
    BOOST_CHECK( g->isEmpty() );
}

BOOST_AUTO_TEST_CASE( lineString_twoPoints )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "LINESTRING(0.0 0.0,1.0 1.0)" ) );
    BOOST_CHECK( g->is< LineString<Epeck> >() );
    BOOST_CHECK( ! g->isEmpty() );
    BOOST_CHECK_EQUAL( g->as< LineString<Epeck> >().numPoints(), 2U );
}

BOOST_AUTO_TEST_CASE( lineString_twoPoints3D )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "LINESTRING(0.0 0.0 0.0,1.0 1.0 1.0)" ) );
    BOOST_CHECK( g->is< LineString<Epeck> >() );
    BOOST_CHECK( ! g->isEmpty() );
    BOOST_REQUIRE_EQUAL( g->as< LineString<Epeck> >().numPoints(), 2U );
    BOOST_CHECK( g->as< LineString<Epeck> >().pointN( 0 ).is3D() );
    BOOST_CHECK( g->as< LineString<Epeck> >().pointN( 1 ).is3D() );
}

//-- WKT POLYGON


BOOST_AUTO_TEST_CASE( polygonEmpty )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "POLYGON EMPTY" ) );
    BOOST_CHECK( g->is< Polygon<Epeck> >() );
    BOOST_CHECK( g->isEmpty() );
}


// 4 points polygon (triangle)
BOOST_AUTO_TEST_CASE( polygonWithFourPoints )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "POLYGON((0 0,1 0,1 1,0 0))" ) );
    BOOST_CHECK( g->is< Polygon<Epeck> >() );
    BOOST_CHECK( ! g->isEmpty() );
    BOOST_CHECK_EQUAL( g->as< Polygon<Epeck> >().exteriorRing().numPoints(), 4U );
}




//-- WKT MULTIPOINT

BOOST_AUTO_TEST_CASE( multiPointEmpty )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "MULTIPOINT EMPTY" ) );
    BOOST_CHECK( g->is< MultiPoint<Epeck> >() );
    BOOST_CHECK( g->isEmpty() );
}

BOOST_AUTO_TEST_CASE( multiPointEmpty2 )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "MULTIPOINT(0 0,1 1,EMPTY)" ) );
    //BOOST_CHECK( g->asText() == "MULTIPOINT((0/1 0/1),(1/1 1/1))" );
    BOOST_CHECK( g->is< MultiPoint<Epeck> >() );
    BOOST_CHECK( g->as< MultiPoint<Epeck> >().numGeometries() == 2 );
}

BOOST_AUTO_TEST_CASE( multiPointEmpty3 )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "MULTIPOINT(EMPTY,EMPTY)" ) );
//    BOOST_CHECK( g->asText() == "MULTIPOINT EMPTY" );
    BOOST_CHECK( g->is< MultiPoint<Epeck> >() );
    BOOST_CHECK( g->isEmpty() );
}

//-- WKT MULTILINESTRING

BOOST_AUTO_TEST_CASE( multiLineStringEmpty )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "MULTILINESTRING EMPTY" ) );
    BOOST_CHECK( g->is< MultiLineString<Epeck> >() );
    BOOST_CHECK( g->isEmpty() );
}


//-- WKT MULTIPOLYGON

BOOST_AUTO_TEST_CASE( multiPolygonEmpty )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "MULTIPOLYGON EMPTY" ) );
    BOOST_CHECK( g->is< MultiPolygon<Epeck> >() );
    BOOST_CHECK( g->isEmpty() );
}



//-- WKT GEOMETRYCOLLECTION

BOOST_AUTO_TEST_CASE( geometryCollectionEmpty )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "GEOMETRYCOLLECTION EMPTY" ) );
    BOOST_CHECK( g->is< GeometryCollection<Epeck> >() );
    BOOST_CHECK( g->isEmpty() );
}



//-- WKT TRIANGULATEDSURFACE

BOOST_AUTO_TEST_CASE( triangulatedSurface_Empty )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "TIN EMPTY" ) );
    BOOST_CHECK( g->is< TriangulatedSurface<Epeck> >() );
    BOOST_CHECK( g->isEmpty() );
}

BOOST_AUTO_TEST_CASE( triangulatedSurface_fourTriangles )
{
    std::string wkt = "TIN("
                      "((0 0 0, 0 0 1, 0 1 0, 0 0 0)),"
                      "((0 0 0, 0 1 0, 1 0 0, 0 0 0)),"
                      "((0 0 0, 1 0 0, 0 0 1, 0 0 0)),"
                      "((1 0 0, 0 1 0, 0 0 1, 1 0 0))"
                      ")";
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( wkt ) );
    BOOST_CHECK( g->is< TriangulatedSurface<Epeck> >() );
    BOOST_CHECK( ! g->isEmpty() );

    BOOST_CHECK_EQUAL( g->as< TriangulatedSurface<Epeck> >().numGeometries(), 4U );
}

BOOST_AUTO_TEST_CASE( wkt_exactTest )
{
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "LINESTRING(2/3 3/2,5/4 2/3)" ) );
    BOOST_CHECK( g->is< LineString<Epeck> >() );
    BOOST_CHECK( ! g->isEmpty() );
    BOOST_REQUIRE_EQUAL( g->as< LineString<Epeck> >().numPoints(), 2U );
    Epeck::Exact_kernel::FT x = CGAL::exact( g->as<LineString<Epeck>>().pointN( 0 ).x() );
    Epeck::Exact_kernel::FT y = CGAL::exact( g->as<LineString<Epeck>>().pointN( 0 ).y() );
    BOOST_CHECK_EQUAL( x.numerator(), 2 );
    BOOST_CHECK_EQUAL( x.denominator(), 3 );
    BOOST_CHECK_EQUAL( y.numerator(), 3 );
    BOOST_CHECK_EQUAL( y.denominator(), 2 );
}

BOOST_AUTO_TEST_CASE( charArrayRead )
{
    char str[] = "LINESTRING(0.0 0.0,1.0 1.0)";
    std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( str, strlen( str ) ) );
    BOOST_CHECK( g->is< LineString<Epeck> >() );
    BOOST_CHECK( ! g->isEmpty() );
    BOOST_CHECK_EQUAL( g->as< LineString<Epeck> >().numPoints(), 2U );
}

BOOST_AUTO_TEST_CASE( wktExtraCharacters )
{
    bool threw = false;
    try
    {
        std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( "POINT(0 0)POINT(1 0)" ) );
    }
    catch ( WktParseException& e )
    {
        std::string err( e.what() );
        BOOST_CHECK_EQUAL( err, "Extra characters in WKT: POINT(1 0)" );
        threw = true;
    }
    BOOST_CHECK( threw );

    threw = false;
    try
    {
        char str[] = "POINT(0 0)POINT(1 0)";
        std::unique_ptr< Geometry<Epeck> > g( readWkt<Epeck>( str, strlen( str ) ) );
    }
    catch ( WktParseException& e )
    {
        std::string err( e.what() );
        BOOST_CHECK_EQUAL( err, "Extra characters in WKT: POINT(1 0)" );
        threw = true;
    }
    BOOST_CHECK( threw );
}


BOOST_AUTO_TEST_SUITE_END()



