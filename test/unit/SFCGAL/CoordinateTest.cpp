/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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

#include <SFCGAL/Coordinate.h>
#include <SFCGAL/Exception.h>

using namespace boost::unit_test ;
using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_CoordinateTest )

/// Coordinate() ;
BOOST_AUTO_TEST_CASE( testDefaultConstructor )
{
	Coordinate g ;
	BOOST_CHECK( g.isEmpty() );
	BOOST_CHECK_EQUAL( g.x(), 0 );
	BOOST_CHECK_EQUAL( g.y(), 0 );
}

/// Coordinate( const Kernel::FT & x, const Kernel::FT & y ) ;
BOOST_AUTO_TEST_CASE( testXYConstructor )
{
	Coordinate g( 3, 4 );
	BOOST_CHECK( ! g.isEmpty() );
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.x(), 3 );
	BOOST_CHECK_EQUAL( g.y(), 4 );
}


/// Coordinate( const Kernel::FT & x, const Kernel::FT & y, const Kernel::FT & z ) ;
BOOST_AUTO_TEST_CASE( testXYZConstructor )
{
	Coordinate g( 3, 4, 5 );
	BOOST_CHECK( ! g.isEmpty() );
	BOOST_CHECK( g.is3D() );
	BOOST_CHECK_EQUAL( g.x(), 3 );
	BOOST_CHECK_EQUAL( g.y(), 4 );
	BOOST_CHECK_EQUAL( g.z(), 5 );
}

/// Coordinate( const double & x, const double & y, const double & z = NaN() ) ;
BOOST_AUTO_TEST_CASE( testXYConstructorDouble )
{
	Coordinate g( 3.0, 4.0 );
	BOOST_CHECK( ! g.isEmpty() );
	BOOST_CHECK( ! g.is3D() );
	BOOST_CHECK_EQUAL( g.x(), 3 );
	BOOST_CHECK_EQUAL( g.y(), 4 );
}
BOOST_AUTO_TEST_CASE( testXYZConstructorDouble )
{
	Coordinate g( 3.0, 4.0, 5.0 );
	BOOST_CHECK( ! g.isEmpty() );
	BOOST_CHECK( g.is3D() );
	BOOST_CHECK_EQUAL( g.x(), 3 );
	BOOST_CHECK_EQUAL( g.y(), 4 );
	BOOST_CHECK_EQUAL( g.z(), 5 );
}

/// Coordinate( const CGAL::Point_2< K > & other ):
/// Coordinate( const CGAL::Point_3< K > & other ):
/// Coordinate( const Coordinate & other ) ;
BOOST_AUTO_TEST_CASE( testCopyConstructorEmpty )
{
	Coordinate g ;
	Coordinate copy( g );
	BOOST_CHECK( copy.isEmpty() );
}
BOOST_AUTO_TEST_CASE( testCopyConstructorXY )
{
	Coordinate g(3,4) ;
	Coordinate copy( g );
	BOOST_CHECK_EQUAL( copy.x(), 3 );
	BOOST_CHECK_EQUAL( copy.y(), 4 );
}

/// Coordinate& operator = ( const Coordinate & other ) ;
/// ~Coordinate() ;
/// int          coordinateDimension() const ;
BOOST_AUTO_TEST_CASE( testCoordinateDimensionEmpty )
{
	Coordinate g ;
	BOOST_CHECK_EQUAL( g.coordinateDimension(), 0 );
}
BOOST_AUTO_TEST_CASE( testCoordinateDimensionXY )
{
	Coordinate g(3,4) ;
	BOOST_CHECK_EQUAL( g.coordinateDimension(), 2 );
}
BOOST_AUTO_TEST_CASE( testCoordinateDimensionXYZ )
{
	Coordinate g(3,4,5) ;
	BOOST_CHECK_EQUAL( g.coordinateDimension(), 3 );
}

/// bool         isEmpty() const ;
/// bool         is3D() const ;
/// Kernel::FT x() const;
/// Kernel::FT y() const;
/// Kernel::FT z() const;

/// bool operator < ( const Coordinate & other ) const ;
BOOST_AUTO_TEST_CASE( testLessEmpty )
{
	Coordinate gA ;
	Coordinate gB ;
	BOOST_CHECK_THROW( (gA < gB), Exception ) ;
}
BOOST_AUTO_TEST_CASE( testLessXY_XY )
{
	BOOST_CHECK( ! ( Coordinate(0,0) < Coordinate(0,0) ) ) ;
	BOOST_CHECK(   ( Coordinate(0,0) < Coordinate(1,0) ) ) ;
	BOOST_CHECK(   ( Coordinate(1,0) < Coordinate(1,1) ) ) ;
}
BOOST_AUTO_TEST_CASE( testLessXYZ_XYZ )
{
	BOOST_CHECK( ! ( Coordinate(0,0,0) < Coordinate(0,0,0) ) ) ;
	BOOST_CHECK(   ( Coordinate(0,0,0) < Coordinate(1,0,0) ) ) ;
	BOOST_CHECK(   ( Coordinate(1,0,0) < Coordinate(1,1,0) ) ) ;
	BOOST_CHECK( ! ( Coordinate(1,1,0) < Coordinate(1,1,0) ) ) ;
	BOOST_CHECK(   ( Coordinate(1,1,0) < Coordinate(1,1,1) ) ) ;
}
BOOST_AUTO_TEST_CASE( testLessXY_XYZ )
{
	BOOST_CHECK_THROW( (Coordinate(0,0) < Coordinate(0,0,0) ), Exception ) ;
}

/// bool operator == ( const Coordinate & other ) const ;
/// bool operator != ( const Coordinate & other ) const ;
/// inline Kernel::Vector_2 toVector_2() const
/// inline Kernel::Vector_3 toVector_3() const
/// Kernel::Point_2 toPoint_2() const;
/// Kernel::Point_3 toPoint_3() const;


BOOST_AUTO_TEST_SUITE_END()




