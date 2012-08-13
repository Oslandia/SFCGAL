#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

#include <SFCGAL/Envelope.h>

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_EnvelopeTest )

//Envelope() ;
BOOST_AUTO_TEST_CASE( defaultConstructor )
{
	Envelope box ;
	BOOST_CHECK( box.boundsN(0).isEmpty() );
	BOOST_CHECK( box.boundsN(1).isEmpty() );
	BOOST_CHECK( box.boundsN(2).isEmpty() );
}

//Envelope( const Coordinate & p ) ;
BOOST_AUTO_TEST_CASE( constructorWithCoordinateXY )
{
	Envelope box( Coordinate(1.0,2.0) );
	BOOST_CHECK( box.boundsN(0) == detail::Interval(1.0) );
	BOOST_CHECK( box.boundsN(1) == detail::Interval(2.0) );
	BOOST_CHECK( box.boundsN(2).isEmpty() );
}

//Envelope( const Coordinate & p1, const Coordinate & p2 ) ;
BOOST_AUTO_TEST_CASE( constructorWithTwoCoordinateXY )
{
	Envelope box( Coordinate(3.0,5.0), Coordinate(1.0,2.0) );
	BOOST_CHECK( box.boundsN(0) == detail::Interval(1.0,3.0) );
	BOOST_CHECK( box.boundsN(1) == detail::Interval(2.0,5.0) );
	BOOST_CHECK( box.boundsN(2).isEmpty() );
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
	Envelope box( Coordinate(0.0,0.0) ) ;
	BOOST_CHECK( ! box.isEmpty() );
}
//bool is3D() const ;
BOOST_AUTO_TEST_CASE( testIs3D )
{
	Envelope box( Coordinate(0.0,0.0,0.0) ) ;
	BOOST_CHECK( box.is3D() );
}
BOOST_AUTO_TEST_CASE( testNotIs3D )
{
	Envelope box( Coordinate(0.0,0.0) ) ;
	BOOST_CHECK( ! box.is3D() );
}
//void expandToInclude( const Coordinate & coordinate ) ;

BOOST_AUTO_TEST_CASE( testExpandToInclude )
{
	Envelope box ;
	BOOST_CHECK( box.isEmpty() );

	box.expandToInclude( Coordinate(0.0,0.0) ) ;
	BOOST_CHECK( ! box.isEmpty() );
	BOOST_CHECK( ! box.is3D() );

	BOOST_CHECK( box.boundsN(0) == detail::Interval(0.0) );
	BOOST_CHECK( box.boundsN(1) == detail::Interval(0.0) );

	box.expandToInclude( Coordinate(0.0,0.0,0.0) ) ;
	BOOST_CHECK( ! box.isEmpty() );
	BOOST_CHECK( box.is3D() );

	BOOST_CHECK( box.boundsN(0) == detail::Interval(0.0) );
	BOOST_CHECK( box.boundsN(1) == detail::Interval(0.0) );
	BOOST_CHECK( box.boundsN(2) == detail::Interval(0.0) );

	box.expandToInclude( Coordinate(1.0,2.0) ) ;
	BOOST_CHECK( box.boundsN(0) == detail::Interval(0.0,1.0) );
	BOOST_CHECK( box.boundsN(1) == detail::Interval(0.0,2.0) );
	BOOST_CHECK( box.boundsN(2) == detail::Interval(0.0) );
}


BOOST_AUTO_TEST_SUITE_END()




