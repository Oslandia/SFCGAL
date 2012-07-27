#define BOOST_TEST_MODULE UnitTestSFCGAL

#define BOOST_TEST_ALTERNATIVE_INIT_API

#include <boost/test/unit_test.hpp>
using namespace boost::unit_test ;

#include <SFCGAL/tools/Log.h>

test_suite* init_unit_test_suite(int, char * * const){
//	std::cerr << "init test suite" << std::endl;
	SFCGAL::Logger::get()->setLogLevel( SFCGAL::Logger::Info );
	return 0;
}







