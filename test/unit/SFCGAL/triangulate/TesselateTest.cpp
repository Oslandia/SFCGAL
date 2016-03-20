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

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/triangulate/tesselate.h>

using namespace SFCGAL ;
using namespace SFCGAL::triangulate ;

// always after CGAL
using namespace boost::unit_test ;

BOOST_AUTO_TEST_SUITE( SFCGAL_triangulate_TesselateTest )


BOOST_AUTO_TEST_CASE( testEmpty )
{
    boost::ptr_vector< Geometry<Epeck> > emptyGeometries;
    emptyGeometries.push_back(new Point<Epeck>());
    emptyGeometries.push_back(new LineString<Epeck>());
    emptyGeometries.push_back(new TriangulatedSurface<Epeck>());
    emptyGeometries.push_back(new MultiSolid<Epeck>());        

    for ( const Geometry<Epeck>& g : emptyGeometries ) {
        BOOST_TEST_MESSAGE( boost::format( "tesselate3D(%s)" ) % io::toWkt<Epeck>(g) );
        std::unique_ptr< Geometry<Epeck> > result( tesselate3D<Epeck>( g ) );
        BOOST_CHECK( result->isEmpty() );
        BOOST_CHECK_EQUAL( g.geometryType(), result->geometryType() );
    }
}

/*
 * test invariants (Point,LineString & co)
 */

BOOST_AUTO_TEST_CASE( testPoint )
{
    std::string wkt = "POINT(3.0 4.0)" ;
    std::unique_ptr< Geometry<Epeck> > g( io::readWkt<Epeck>( wkt ) );
    BOOST_CHECK_EQUAL( io::toWkt<Epeck>(*tesselate3D( *g ), 1), wkt );
}
BOOST_AUTO_TEST_CASE( testLineString )
{
    std::string wkt = "LINESTRING(0.0 0.0,1.0 1.0)" ;
    std::unique_ptr< Geometry<Epeck> > g( io::readWkt<Epeck>( wkt ) );
    BOOST_CHECK_EQUAL( io::toWkt<Epeck>(*tesselate3D( *g ), 1), wkt );
}
BOOST_AUTO_TEST_CASE( testMultiPoint )
{
    std::string wkt = "MULTIPOINT((3.0 4.0),(5.0 6.0))" ;
    std::unique_ptr< Geometry<Epeck> > g( io::readWkt<Epeck>( wkt ) );
    BOOST_CHECK_EQUAL( io::toWkt<Epeck>(*tesselate3D( *g ), 1), wkt );
}
BOOST_AUTO_TEST_CASE( testMultiLineString )
{
    std::string wkt = "MULTILINESTRING((0.0 0.0,1.0 1.0),(1.0 1.0,2.0 2.0))" ;
    std::unique_ptr< Geometry<Epeck> > g( io::readWkt<Epeck>( wkt ) );
    BOOST_CHECK_EQUAL( io::toWkt<Epeck>(*tesselate3D( *g ), 1), wkt );
}


/*
 * test with polygon, MultiPolygon & co
 */
BOOST_AUTO_TEST_CASE( testPolygon )
{
    std::string wkt = "POLYGON((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0))" ;
    std::string wktOut = "TIN(((0.0 1.0 0.0,0.0 0.0 0.0,1.0 0.0 0.0,0.0 1.0 0.0)),((0.0 1.0 0.0,1.0 0.0 0.0,1.0 1.0 0.0,0.0 1.0 0.0)))" ;
    std::unique_ptr< Geometry<Epeck> > g( io::readWkt<Epeck>( wkt ) );
    std::unique_ptr< Geometry<Epeck> > result( tesselate3D( *g ) );
    BOOST_REQUIRE( result->is< TriangulatedSurface<Epeck> >() );
    BOOST_CHECK_EQUAL( result->as< TriangulatedSurface<Epeck> >().size(), 2 );
    BOOST_CHECK_EQUAL( io::toWkt<Epeck>(*result,1), wktOut );
}

BOOST_AUTO_TEST_CASE( testMultiPolygonConnected )
{
    // a single polygon in a multipolygon (ensure connected)
    std::string wkt = "MULTIPOLYGON(((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0)))" ;
    std::string wktOut = "TIN(((1.0 0.0 0.0,0.0 1.0 0.0,0.0 0.0 0.0,1.0 0.0 0.0)),((1.0 1.0 0.0,0.0 1.0 0.0,1.0 0.0 0.0,1.0 1.0 0.0)))" ;
    std::unique_ptr< Geometry<Epeck> > g( io::readWkt<Epeck>( wkt ) );
    std::unique_ptr< Geometry<Epeck> > result( tesselate3D( *g ) );
    BOOST_REQUIRE( result->is< TriangulatedSurface<Epeck> >() );
    BOOST_CHECK_EQUAL( result->as< TriangulatedSurface<Epeck> >().size(), 2 );
    BOOST_CHECK_EQUAL( io::toWkt<Epeck>(*result,1), wktOut );
}


BOOST_AUTO_TEST_CASE( testMultiPolygonNonConnected )
{
    std::string wkt = "MULTIPOLYGON(((0.0 0.0,1.0 0.0,1.0 1.0,0.0 1.0,0.0 0.0)),((2.0 0.0,3.0 0.0,3.0 1.0,2.0 1.0,2.0 0.0)))" ;
    std::string wktOut = "GEOMETRYCOLLECTION(TIN(((1.0 0.0 0.0,0.0 1.0 0.0,0.0 0.0 0.0,1.0 0.0 0.0)),((1.0 1.0 0.0,0.0 1.0 0.0,1.0 0.0 0.0,1.0 1.0 0.0))),TIN(((3.0 0.0 0.0,2.0 1.0 0.0,2.0 0.0 0.0,3.0 0.0 0.0)),((3.0 1.0 0.0,2.0 1.0 0.0,3.0 0.0 0.0,3.0 1.0 0.0))))" ;
    std::unique_ptr< Geometry<Epeck> > g( io::readWkt<Epeck>( wkt ) );
    std::unique_ptr< Geometry<Epeck> > result( tesselate3D( *g ) );
    BOOST_REQUIRE( result->is< GeometryCollection<Epeck> >() );
    BOOST_CHECK_EQUAL( result->as< GeometryCollection<Epeck> >().size(), 2 );
    BOOST_CHECK_EQUAL( io::toWkt<Epeck>(*result,1), wktOut );
}

BOOST_AUTO_TEST_CASE( testPolyhedralSurface )
{
    std::string wkt = "POLYHEDRALSURFACE Z (((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),((0 0 0, 0 1 0, 0 1 1, 0 0 1, 0 0 0)),((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),((1 1 1, 1 0 1, 0 0 1, 0 1 1, 1 1 1)),((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1)))";
    std::string wktOut = "TIN(((1.0 0.0 0.0,0.0 1.0 0.0,0.0 0.0 0.0,1.0 0.0 0.0)),((1.0 1.0 0.0,0.0 1.0 0.0,1.0 0.0 0.0,1.0 1.0 0.0)),((0.0 0.0 0.0,0.0 1.0 1.0,0.0 0.0 1.0,0.0 0.0 0.0)),((0.0 1.0 0.0,0.0 1.0 1.0,0.0 0.0 0.0,0.0 1.0 0.0)),((1.0 0.0 1.0,0.0 0.0 0.0,0.0 0.0 1.0,1.0 0.0 1.0)),((1.0 0.0 0.0,0.0 0.0 0.0,1.0 0.0 1.0,1.0 0.0 0.0)),((0.0 1.0 1.0,1.0 0.0 1.0,0.0 0.0 1.0,0.0 1.0 1.0)),((0.0 1.0 1.0,1.0 1.0 1.0,1.0 0.0 1.0,0.0 1.0 1.0)),((1.0 0.0 0.0,1.0 1.0 1.0,1.0 1.0 0.0,1.0 0.0 0.0)),((1.0 0.0 0.0,1.0 0.0 1.0,1.0 1.0 1.0,1.0 0.0 0.0)),((0.0 1.0 0.0,1.0 1.0 1.0,0.0 1.0 1.0,0.0 1.0 0.0)),((0.0 1.0 0.0,1.0 1.0 0.0,1.0 1.0 1.0,0.0 1.0 0.0)))";
    std::unique_ptr< Geometry<Epeck> > g( io::readWkt<Epeck>( wkt ) );
    std::unique_ptr< Geometry<Epeck> > result( tesselate3D( *g ) );
    BOOST_REQUIRE( result->is< TriangulatedSurface<Epeck> >() );
    BOOST_CHECK_EQUAL( result->as< TriangulatedSurface<Epeck> >().size(), 12 );
    BOOST_CHECK_EQUAL( io::toWkt<Epeck>(*result,1), wktOut );
}

BOOST_AUTO_TEST_CASE( testSolid )
{
    std::string wkt = "SOLID((((0 0 0, 0 1 0, 1 1 0, 1 0 0, 0 0 0)),((0 0 0, 0 1 0, 0 1 1, 0 0 1, 0 0 0)),((0 0 0, 1 0 0, 1 0 1, 0 0 1, 0 0 0)),((1 1 1, 1 0 1, 0 0 1, 0 1 1, 1 1 1)),((1 1 1, 1 0 1, 1 0 0, 1 1 0, 1 1 1)),((1 1 1, 1 1 0, 0 1 0, 0 1 1, 1 1 1))))";
    std::string wktOut = "SOLID((((0.0 1.0 0.0,1.0 0.0 0.0,0.0 0.0 0.0,0.0 1.0 0.0)),((0.0 1.0 0.0,1.0 1.0 0.0,1.0 0.0 0.0,0.0 1.0 0.0)),((0.0 1.0 1.0,0.0 0.0 0.0,0.0 0.0 1.0,0.0 1.0 1.0)),((0.0 1.0 1.0,0.0 1.0 0.0,0.0 0.0 0.0,0.0 1.0 1.0)),((0.0 0.0 0.0,1.0 0.0 1.0,0.0 0.0 1.0,0.0 0.0 0.0)),((0.0 0.0 0.0,1.0 0.0 0.0,1.0 0.0 1.0,0.0 0.0 0.0)),((1.0 0.0 1.0,0.0 1.0 1.0,0.0 0.0 1.0,1.0 0.0 1.0)),((1.0 1.0 1.0,0.0 1.0 1.0,1.0 0.0 1.0,1.0 1.0 1.0)),((1.0 1.0 1.0,1.0 0.0 0.0,1.0 1.0 0.0,1.0 1.0 1.0)),((1.0 0.0 1.0,1.0 0.0 0.0,1.0 1.0 1.0,1.0 0.0 1.0)),((1.0 1.0 1.0,0.0 1.0 0.0,0.0 1.0 1.0,1.0 1.0 1.0)),((1.0 1.0 0.0,0.0 1.0 0.0,1.0 1.0 1.0,1.0 1.0 0.0))))";
    std::unique_ptr< Geometry<Epeck> > g( io::readWkt<Epeck>( wkt ) );
    std::unique_ptr< Geometry<Epeck> > result( tesselate3D( *g ) );
    BOOST_REQUIRE( result->is< Solid<Epeck> >() );
    
    const Solid<Epeck> & solid = result->as< Solid<Epeck> >() ;    
    BOOST_REQUIRE_EQUAL( solid.size(), 1 );
    BOOST_CHECK_EQUAL( solid[0].size(), 12 );
    BOOST_CHECK_EQUAL( io::toWkt<Epeck>(*result,1), wktOut );
}

BOOST_AUTO_TEST_SUITE_END()

