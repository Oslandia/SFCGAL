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

#include <SFCGAL/Envelope.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Solid.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_EnvelopeTest )

//Envelope() ;
BOOST_AUTO_TEST_CASE( defaultConstructor )
{
    Envelope box ;
    BOOST_CHECK( box.boundsN( 0 ).isEmpty() );
    BOOST_CHECK( box.boundsN( 1 ).isEmpty() );
    BOOST_CHECK( box.boundsN( 2 ).isEmpty() );
}

//Envelope( const Coordinate & p ) ;
BOOST_AUTO_TEST_CASE( constructorWithCoordinateXY )
{
    Envelope box( Coordinate( 1.0,2.0 ) );
    BOOST_CHECK( box.boundsN( 0 ) == detail::Interval( 1.0 ) );
    BOOST_CHECK( box.boundsN( 1 ) == detail::Interval( 2.0 ) );
    BOOST_CHECK( box.boundsN( 2 ).isEmpty() );
}

//Envelope( const Coordinate & p1, const Coordinate & p2 ) ;
BOOST_AUTO_TEST_CASE( constructorWithTwoCoordinateXY )
{
    Envelope box( Coordinate( 3.0,5.0 ), Coordinate( 1.0,2.0 ) );
    BOOST_CHECK( box.boundsN( 0 ) == detail::Interval( 1.0,3.0 ) );
    BOOST_CHECK( box.boundsN( 1 ) == detail::Interval( 2.0,5.0 ) );
    BOOST_CHECK( box.boundsN( 2 ).isEmpty() );
}

//Envelope( const Envelope& other ) ;
//Envelope& operator = ( const Envelope& other ) ;
//~Envelope() ;

//bool isEmpty() const ;
BOOST_AUTO_TEST_CASE( testIsEmpty )
{
    Envelope box ;
    BOOST_CHECK( box.isEmpty() );
}
BOOST_AUTO_TEST_CASE( testIsNotEmpty )
{
    Envelope box( Coordinate( 0.0,0.0 ) ) ;
    BOOST_CHECK( ! box.isEmpty() );
}
//bool is3D() const ;
BOOST_AUTO_TEST_CASE( testIs3D )
{
    Envelope box( Coordinate( 0.0,0.0,0.0 ) ) ;
    BOOST_CHECK( box.is3D() );
}
BOOST_AUTO_TEST_CASE( testNotIs3D )
{
    Envelope box( Coordinate( 0.0,0.0 ) ) ;
    BOOST_CHECK( ! box.is3D() );
}
//void expandToInclude( const Coordinate & coordinate ) ;

BOOST_AUTO_TEST_CASE( testExpandToInclude )
{
    Envelope box ;
    BOOST_CHECK( box.isEmpty() );

    box.expandToInclude( Coordinate( 0.0,0.0 ) ) ;
    BOOST_CHECK( ! box.isEmpty() );
    BOOST_CHECK( ! box.is3D() );

    BOOST_CHECK( box.boundsN( 0 ) == detail::Interval( 0.0 ) );
    BOOST_CHECK( box.boundsN( 1 ) == detail::Interval( 0.0 ) );

    box.expandToInclude( Coordinate( 0.0,0.0,0.0 ) ) ;
    BOOST_CHECK( ! box.isEmpty() );
    BOOST_CHECK( box.is3D() );

    BOOST_CHECK( box.boundsN( 0 ) == detail::Interval( 0.0 ) );
    BOOST_CHECK( box.boundsN( 1 ) == detail::Interval( 0.0 ) );
    BOOST_CHECK( box.boundsN( 2 ) == detail::Interval( 0.0 ) );

    box.expandToInclude( Coordinate( 1.0,2.0 ) ) ;
    BOOST_CHECK( box.boundsN( 0 ) == detail::Interval( 0.0,1.0 ) );
    BOOST_CHECK( box.boundsN( 1 ) == detail::Interval( 0.0,2.0 ) );
    BOOST_CHECK( box.boundsN( 2 ) == detail::Interval( 0.0 ) );
}


//std::unique_ptr< LineString > toRing() const ;
BOOST_AUTO_TEST_CASE( testToRing )
{
    Envelope box( 0.0,1.0,2.0,3.0 );
    BOOST_CHECK_EQUAL( box.toRing()->asText( 0 ), "LINESTRING(0 2,1 2,1 3,0 3,0 2)" );
}

//std::unique_ptr< Polygon >    toPolygon() const ;
BOOST_AUTO_TEST_CASE( testToPolygon )
{
    Envelope box( 0.0,1.0,2.0,3.0,4.0,5.0 );
    BOOST_CHECK_EQUAL( box.toSolid()->asText( 0 ), "SOLID((((0 2 4,0 3 4,1 3 4,1 2 4,0 2 4)),((0 2 5,1 2 5,1 3 5,0 3 5,0 2 5)),((0 2 4,1 2 4,1 2 5,0 2 5,0 2 4)),((1 3 4,0 3 4,0 3 5,1 3 5,1 3 4)),((1 2 4,1 3 4,1 3 5,1 2 5,1 2 4)),((0 2 4,0 2 5,0 3 5,0 3 4,0 2 4))))" );
}


BOOST_AUTO_TEST_SUITE_END()




