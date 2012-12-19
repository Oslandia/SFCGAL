#include <memory>
#include <string>

#include <SFCGAL/all.h>
#include <SFCGAL/Kernel.h>
#include <SFCGAL/io/Serialization.h>
#include <SFCGAL/io/wkt.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_io_SerializationTest )

BOOST_AUTO_TEST_CASE( textTest )
{
	Kernel::Point_2 pt( 2.3, 4.5 );
	Kernel::Point_2 rpt;
	Coordinate pt1( 2.3, 8.9 );
	Coordinate rpt1;
	// 2/3
	CGAL::Gmpq q1( 2, 3 );
	CGAL::Gmpq rq1;

	std::ostringstream ostr;
	boost::archive::text_oarchive arc( ostr );
	arc << pt;
	arc << pt1;
	arc << q1;

	std::string str = ostr.str();

	std::istringstream istr( str );
	boost::archive::text_iarchive iarc( istr );
	iarc >> rpt;
	iarc >> rpt1;
	iarc >> rq1;

	BOOST_CHECK( pt == rpt );
	BOOST_CHECK( pt1 == rpt1 );
	BOOST_CHECK( q1 == rq1 );
}

BOOST_AUTO_TEST_CASE( binaryTest )
{
	Kernel::Point_2 pt( 2.3, 4.5 );
	Kernel::Point_2 rpt;

	std::ostringstream ostr;
	io::BinarySerializer arc( ostr );
	arc << pt;

	std::string str = ostr.str();

	std::istringstream istr( str );
	io::BinaryUnserializer iarc( istr );
	iarc >> rpt;

	BOOST_CHECK( pt == rpt );
}

BOOST_AUTO_TEST_CASE( geometryTest )
{
	std::auto_ptr<Geometry> g1 = io::readWkt( "POINT( 3.4 4.5 5.6 )" );
	std::auto_ptr<Geometry> g2 = io::readWkt( "LINESTRING( 3.4 4.5 5.6, 5 6 8 )" );
	std::auto_ptr<Geometry> g3 = io::readWkt( "TRIANGLE(( 0 0 0, 3.4 5.6 6.7,2 3 4, 0 0 0))" );
	std::auto_ptr<Geometry> g4 = io::readWkt( "POLYGON(( 0 0 0, 1 1 1, 3.4 5.6 6.7,2 3 4, 0 0 0))" );
	std::auto_ptr<Geometry> g5 = io::readWkt( "TIN((( 0 0 0, 3.4 5.6 6.7,2 3 4, 0 0 0)), ((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)))" );
	std::auto_ptr<Geometry> g6 = io::readWkt( "POLYHEDRALSURFACE((( 0 0 0, 3.4 5.6 6.7,2 3 4, 0 0 0)), ((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)))" );
	std::auto_ptr<Geometry> g7 = io::readWkt( "SOLID(((( 0 0 0, 3.4 5.6 6.7,2 3 4, 0 0 0)), ((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0))))" );
	std::auto_ptr<Geometry> g8 = io::readWkt( "MULTIPOINT(( 3.4 4.5 5.6 ))" );
	std::auto_ptr<Geometry> g9 = io::readWkt( "MULTILINESTRING(( 3.4 4.5 5.6, 5 6 8 ))" );
	std::auto_ptr<Geometry> g10 = io::readWkt( "MULTIPOLYGON((( 0 0 0, 1 1 1, 3.4 5.6 6.7,2 3 4, 0 0 0)))" );
	std::auto_ptr<Geometry> g11 = io::readWkt( "MULTISOLID((((( 0 0 0, 3.4 5.6 6.7,2 3 4, 0 0 0)), ((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0)))))" );

	BOOST_CHECK( io::readBinary( io::writeBinary( *g1 ) )->asText() == g1->asText() );
	BOOST_CHECK( io::readBinary( io::writeBinary( *g2 ) )->asText() == g2->asText() );
	BOOST_CHECK( io::readBinary( io::writeBinary( *g3 ) )->asText() == g3->asText() );
	BOOST_CHECK( io::readBinary( io::writeBinary( *g4 ) )->asText() == g4->asText() );
	BOOST_CHECK( io::readBinary( io::writeBinary( *g5 ) )->asText() == g5->asText() );
	BOOST_CHECK( io::readBinary( io::writeBinary( *g6 ) )->asText() == g6->asText() );
	BOOST_CHECK( io::readBinary( io::writeBinary( *g7 ) )->asText() == g7->asText() );
	BOOST_CHECK( io::readBinary( io::writeBinary( *g8 ) )->asText() == g8->asText() );
	BOOST_CHECK( io::readBinary( io::writeBinary( *g9 ) )->asText() == g9->asText() );
	BOOST_CHECK( io::readBinary( io::writeBinary( *g10 ) )->asText() == g10->asText() );
	BOOST_CHECK( io::readBinary( io::writeBinary( *g11 ) )->asText() == g11->asText() );
}

BOOST_AUTO_TEST_SUITE_END()



