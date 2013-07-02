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
#include <SFCGAL/Grid.h>
#include <SFCGAL/TriangulatedSurface.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_GridTest )

///Grid();

BOOST_AUTO_TEST_CASE( defaultConstructor )
{
	Grid g;
	BOOST_CHECK( g.isEmpty() ) ;
	BOOST_CHECK_EQUAL( g.pixelConvention(), PIXEL_IS_POINT );
	BOOST_CHECK_EQUAL( g.ncols(), 0U );
	BOOST_CHECK_EQUAL( g.nrows(), 0U );
}


///Grid(
//	const size_t & nrows,
//	const size_t & ncols,
//	const double& fillValue = NaN(),
//	const Envelope & limits = Envelope(0.0,1.0,0.0,1.0),
//	const PixelConvention & pixelType = PIXEL_IS_POINT
//);

BOOST_AUTO_TEST_CASE( constructorWithSize )
{
	Grid g( 4, 2, 5.0 );
	BOOST_CHECK_EQUAL( g.pixelConvention(), PIXEL_IS_POINT );
	BOOST_CHECK_EQUAL( g.nrows(), 4U );
	BOOST_CHECK_EQUAL( g.ncols(), 2U );
	for ( size_t i = 0; i < g.nrows(); i++ ){
		for ( size_t j = 0; j < g.ncols(); j++ ){
			BOOST_CHECK_EQUAL( g.z(i,j), 5.0 );
		}
	}
}

//Grid(
//	const ublas::matrix< double > & data,
//	const Envelope & limits = Envelope(0.0,1.0,0.0,1.0),
//	const PixelConvention & pixelType = PIXEL_IS_POINT
//);

BOOST_AUTO_TEST_CASE( constructorWithMatrix )
{
	detail::ublas::matrix< double > data = detail::ublas::identity_matrix< double >( 5 );
	Grid g( data );
	BOOST_CHECK_EQUAL( g.pixelConvention(), PIXEL_IS_POINT );
	BOOST_CHECK_EQUAL( g.nrows(), 5U );
	BOOST_CHECK_EQUAL( g.ncols(), 5U );
	for ( size_t i = 0; i < g.nrows(); i++ ){
		for ( size_t j = 0; j < g.ncols(); j++ ){
			if ( i == j ){
				BOOST_CHECK_EQUAL( g.z(i,j), 1.0 );
			}else{
				BOOST_CHECK_EQUAL( g.z(i,j), 0.0 );
			}
		}
	}
}



//Grid( const Grid & other );
//Grid& operator = ( const Grid & other );
//~Grid();
//
//inline bool isEmpty() const
//
//inline Point point( const size_t & row, const size_t & col ) const ;

BOOST_AUTO_TEST_CASE( testPoint_PixelIsPoint )
{
	Grid g( 5, 5, 0.0, Envelope(0.0,4.0,0.0,4.0) );
	g.setPixelConvention( PIXEL_IS_POINT );
	BOOST_CHECK_EQUAL( g.point(0,0).x(), 0.0 );
	BOOST_CHECK_EQUAL( g.point(0,0).y(), 4.0 );

	BOOST_CHECK_EQUAL( g.point(1,1).x(), 1.0 );
	BOOST_CHECK_EQUAL( g.point(1,1).y(), 3.0 );

	BOOST_CHECK_EQUAL( g.point(4,4).x(), 4.0 );
	BOOST_CHECK_EQUAL( g.point(4,4).y(), 0.0 );
}

BOOST_AUTO_TEST_CASE( testPoint_PixelIsArea )
{
	Grid g( 4, 4, 0.0, Envelope(0.0,4.0,0.0,4.0) );
	g.setPixelConvention( PIXEL_IS_AREA );
	BOOST_CHECK_EQUAL( g.point(0,0).x(), 0.5 );
	BOOST_CHECK_EQUAL( g.point(0,0).y(), 3.5 );

	BOOST_CHECK_EQUAL( g.point(1,1).x(), 1.5 );
	BOOST_CHECK_EQUAL( g.point(1,1).y(), 2.5 );

	BOOST_CHECK_EQUAL( g.point(3,3).x(), 3.5 );
	BOOST_CHECK_EQUAL( g.point(3,3).y(), 0.5 );
}


//inline double dx() const ;
//inline double dy() const ;
BOOST_AUTO_TEST_CASE( testDxDy_PixelIsPoint )
{
	Grid g(4,2);
	g.setPixelConvention(PIXEL_IS_POINT);
	BOOST_CHECK_EQUAL( g.dx(), 1.0 );
	BOOST_CHECK_EQUAL( g.dy(), 1.0/3.0 );
}
BOOST_AUTO_TEST_CASE( testDxDy_PixelIsArea )
{
	Grid g(4,2);
	g.setPixelConvention(PIXEL_IS_AREA);
	BOOST_CHECK_EQUAL( g.dx(), 0.50 );
	BOOST_CHECK_EQUAL( g.dy(), 0.25 );
}

//
//inline const double & z( const size_t& row, const size_t& col ) const ;
//inline double & z( const size_t& row, const size_t& col ) ;
//
//inline const size_t nrows() const
//inline const size_t ncols() const
//inline const Envelope & limits() const
//inline void setLimits( const Envelope & limits )
//inline const PixelConvention & pixelConvention() const
//inline void setPixelConvention( const PixelConvention & pixelConvention )
//inline ublas::matrix< double > & data()
//inline const ublas::matrix< double > & data() const


//std::auto_ptr< TriangulatedSurface > toTrianguledSurface() const ;
BOOST_AUTO_TEST_CASE( testToTriangulatedSurface )
{
	Grid g(2,2);
	g.z(0,0) = 0.0 ; g.z(0,1) = 1.0 ;
	g.z(1,0) = 2.0 ; g.z(1,1) = 3.0 ;

	g.setPixelConvention( PIXEL_IS_POINT );
	BOOST_CHECK_EQUAL( g.toTrianguledSurface()->asText(1), "TIN M(((0.0 0.0 2.0,1.0 0.0 3.0,1.0 1.0 1.0,0.0 0.0 2.0)),((0.0 0.0 2.0,1.0 1.0 1.0,0.0 1.0 0.0,0.0 0.0 2.0)))" );
	g.setPixelConvention( PIXEL_IS_AREA );
	BOOST_CHECK_EQUAL( g.toTrianguledSurface()->asText(1), "TIN M(((0.2 0.2 2.0,0.8 0.2 3.0,0.8 0.8 1.0,0.2 0.2 2.0)),((0.2 0.2 2.0,0.8 0.8 1.0,0.2 0.8 0.0,0.2 0.2 2.0)))" );
}


BOOST_AUTO_TEST_SUITE_END()




