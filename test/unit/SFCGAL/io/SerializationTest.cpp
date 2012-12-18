#include <memory>
#include <string>

#include <SFCGAL/all.h>
#include <SFCGAL/Kernel.h>
#include <SFCGAL/io/Serialization.h>

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

using namespace SFCGAL ;

BOOST_AUTO_TEST_SUITE( SFCGAL_io_WktReaderTest )

BOOST_AUTO_TEST_CASE( textTest )
{
	Kernel::Point_2 pt( 2.3, 4.5 );
	Kernel::Point_2 rpt;

	std::ostringstream ostr;
	boost::archive::text_oarchive arc( ostr );
	arc << pt;

	std::string str = ostr.str();

	std::istringstream istr( str );
	boost::archive::text_iarchive iarc( istr );
	iarc >> rpt;

	BOOST_CHECK( pt == rpt );
}

BOOST_AUTO_TEST_CASE( binaryTest )
{
	Kernel::Point_2 pt( 2.3, 4.5 );
	Kernel::Point_2 rpt;

	std::ostringstream ostr;
	boost::archive::binary_oarchive arc( ostr );
	arc << pt;

	std::string str = ostr.str();

	std::istringstream istr( str );
	boost::archive::binary_iarchive iarc( istr );
	iarc >> rpt;

	BOOST_CHECK( pt == rpt );
}

BOOST_AUTO_TEST_SUITE_END()



