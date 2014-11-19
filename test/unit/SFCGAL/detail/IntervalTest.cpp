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

#include <SFCGAL/detail/Interval.h>
#include <SFCGAL/numeric.h>

using namespace SFCGAL ;
using namespace SFCGAL::detail ;

// always after CGAL
using namespace boost::unit_test ;


BOOST_AUTO_TEST_SUITE( SFCGAL_detail_IntervalTest )

//Interval() ;
BOOST_AUTO_TEST_CASE( testDefaultConstructor )
{
    Interval bound;
    BOOST_CHECK( isNaN( bound.lower() ) );
    BOOST_CHECK( isNaN( bound.upper() ) );
}

//Interval( const double & value ) ;
BOOST_AUTO_TEST_CASE( testConstructorWithValue )
{
    Interval bound( 5.0 );
    BOOST_CHECK_EQUAL( bound.lower(), 5.0 );
    BOOST_CHECK_EQUAL( bound.upper(), 5.0 );
}

//Interval( const double & v1, const double & v2 ) ;
BOOST_AUTO_TEST_CASE( testConstructorWithTwoValues )
{
    Interval bound( 5.0,3.0 );
    BOOST_CHECK_EQUAL( bound.lower(), 3.0 );
    BOOST_CHECK_EQUAL( bound.upper(), 5.0 );
}


//Interval( const Interval & other ) ;
//Interval& operator = ( const Interval & other ) ;
//~Interval() ;

//bool isEmpty() const ;
BOOST_AUTO_TEST_CASE( testEmpty )
{
    Interval bound ;
    BOOST_CHECK( bound.isEmpty() );
}
BOOST_AUTO_TEST_CASE( testNotEmpty )
{
    Interval bound( 5.0 );
    BOOST_CHECK( ! bound.isEmpty() );
}

//inline const double & lower() const { return _lower; }
//inline const double & upper() const { return _upper; }
//inline double         width() const { return _upper - _lower ; }
BOOST_AUTO_TEST_CASE( testWidth )
{
    Interval bound( 4.0,5.0 );
    BOOST_CHECK_EQUAL( bound.width(), 1.0 );
}

//void                  expandBy( const double & d ) ;
BOOST_AUTO_TEST_CASE( testExpandByEmpty )
{
    Interval bound ;
    bound.expandBy( 5.0 ) ;
    BOOST_CHECK( bound.isEmpty() );
}

//void                  expandToInclude( const Interval & other ) ;
BOOST_AUTO_TEST_CASE( testExpandToIncludeInterval_emptyExpanded )
{
    Interval bound ;
    bound.expandToInclude( Interval( 0.0,1.0 ) );
    BOOST_CHECK_EQUAL( bound.lower(), 0.0 );
    BOOST_CHECK_EQUAL( bound.upper(), 1.0 );
}
BOOST_AUTO_TEST_CASE( testExpandToIncludeInterval_expandByEmpty )
{
    Interval bound( 0.0,1.0 );
    bound.expandToInclude( Interval() );
    BOOST_CHECK_EQUAL( bound.lower(), 0.0 );
    BOOST_CHECK_EQUAL( bound.upper(), 1.0 );
}
BOOST_AUTO_TEST_CASE( testExpandToIncludeInterval_classic )
{
    Interval bound( 0.0,1.0 );
    bound.expandToInclude( Interval( -1.0,2.0 ) );
    BOOST_CHECK_EQUAL( bound.lower(),-1.0 );
    BOOST_CHECK_EQUAL( bound.upper(), 2.0 );
}


//void                  expandToInclude( const double & value ) ;
BOOST_AUTO_TEST_CASE( testExpandToIncludeValue_classic1 )
{
    Interval bound( 0.0,1.0 );
    bound.expandToInclude( 2.0 );
    BOOST_CHECK_EQUAL( bound.lower(), 0.0 );
    BOOST_CHECK_EQUAL( bound.upper(), 2.0 );
}
BOOST_AUTO_TEST_CASE( testExpandToIncludeValue_classic2 )
{
    Interval bound( 0.0,1.0 );
    bound.expandToInclude( -2.0 );
    BOOST_CHECK_EQUAL( bound.lower(), -2.0 );
    BOOST_CHECK_EQUAL( bound.upper(),  1.0 );
}

//bool                  intersects( const Interval & other ) const  ;
BOOST_AUTO_TEST_CASE( testIntersects_emptyBehavior )
{
    BOOST_CHECK( ! Interval().intersects( Interval() ) );
    BOOST_CHECK( ! Interval( 0.0,1.0 ).intersects( Interval() ) );
    BOOST_CHECK( ! Interval().intersects( Interval( 0.0,1.0 ) ) );
}

BOOST_AUTO_TEST_CASE( testIntersects_normalBehavior )
{
    BOOST_CHECK( Interval( 0.0,1.0 ).intersects( Interval( -1.0,0.5 ) ) );
    BOOST_CHECK( Interval( 0.0,1.0 ).intersects( Interval( 0.3,0.5 ) ) );
    BOOST_CHECK( Interval( 0.0,1.0 ).intersects( Interval( 0.5,2.0 ) ) );

    BOOST_CHECK( ! Interval( 0.0,1.0 ).intersects( Interval( 2.0,3.0 ) ) );
    BOOST_CHECK( ! Interval( 2.0,3.0 ).intersects( Interval( 0.0,1.0 ) ) );
}
BOOST_AUTO_TEST_CASE( testIntersects_limitBehavior )
{
    BOOST_CHECK( Interval( 0.0,1.0 ).intersects( Interval( 1.0,2.0 ) ) );
    BOOST_CHECK( Interval( 0.0,1.0 ).intersects( Interval( -1.0,0.0 ) ) );

    BOOST_CHECK( Interval( 1.0,2.0 ).intersects( Interval( 0.0,1.0 ) ) );
    BOOST_CHECK( Interval( -1.0,0.0 ).intersects( Interval( 0.0,1.0 ) ) );
}


BOOST_AUTO_TEST_SUITE_END()

