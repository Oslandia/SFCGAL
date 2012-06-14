#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

#include <SFCGAL/Exception.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/WaveFrontObj.h>


using namespace SFCGAL ;


namespace po = boost::program_options ;

/*
 * Convert a WKT string to obj file in std::cout
 */
int main( int argc, char* argv[] ){
	/*
	 * declare options
	 */
	po::options_description desc("wkt2obj options : ");
	desc.add_options()
	    ("help", "produce help message")
	    ("wkt", po::value< std::string >(), "input wkt")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
	    std::cout << desc << std::endl ;
	    return 0;
	}

	std::string wkt ;
	if ( vm.count("wkt") ) {
		wkt = vm["wkt"].as< std::string >() ;
	} else {
		std::cerr << "missing input wkt" << std::endl;
		std::cout << desc << std::endl ;
		return 1 ;
	}

	std::auto_ptr< Geometry > g( io::readWkt( wkt ) );
	io::WaveFrontObj obj ;
	obj.addGeometry( *g ) ;
	obj.save( std::cout ) ;

	return 0 ;
}

