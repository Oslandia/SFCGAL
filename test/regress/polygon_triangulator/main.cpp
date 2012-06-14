#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <SFCGAL/Exception.h>

#include <SFCGAL/all.h>

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/triangulate.h>

#include <boost/timer.hpp>

using namespace SFCGAL ;


namespace po = boost::program_options ;

/*
 * Triangulate each polygon in an input file containing lines in the following format :
 * <id> "|" ( <wkt polygon> | <wkt multipolygon> )
 */
int main( int argc, char* argv[] ){
	/*
	 * declare options
	 */
	po::options_description desc("polygon triangulator options : ");
	desc.add_options()
	    ("help", "produce help message")
	    ("filename", po::value< std::string >(), "input filename (id|wkt_[multi]polygon on each line)")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
	    std::cout << desc << std::endl ;
	    return 0;
	}

	std::string filename ;
	if ( vm.count("filename") ) {
	    filename = vm["filename"].as< std::string >() ;
	} else {
		std::cerr << "missing input file" << std::endl;
		std::cout << desc << std::endl ;
		return 1 ;
	}

	/*
	 * open file
	 */
	std::ifstream ifs( filename.c_str() );
	if ( ! ifs.good() ){
		std::cerr << "fail to open : " << filename << std::endl ;
		return 1;
	}

	std::ofstream ofs( "test-regress-polygon_triangulator.wkt" ) ;
	if ( ! ofs.good() ){
		std::cerr << "fail to write : " << "test-regress-polygon_triangulator.wkt" << std::endl ;
		return 1;
	}


	boost::timer timer ;

	/*
	 * process file
	 */
	int lineNumber = 0 ;
	std::string line ;

	timer.restart();
	while ( std::getline( ifs, line ) ){
		lineNumber++ ;
//		std::cout << line << std::endl ;
		boost::algorithm::trim( line );
		if ( line.empty() ){
			continue ;
		}

		if ( lineNumber % 10000 == 0 ){
			std::cout.width(12) ;
			std::cout << std::left << lineNumber << "(" << timer.elapsed() << " s)"<< std::endl ;
		}



		std::vector< std::string > tokens ;
		boost::algorithm::split( tokens, line, boost::is_any_of("|") );

		std::string const& wkt = tokens.back() ;
		std::string id ;
		if ( tokens.size() > 1 ){
			id = tokens.front() ;
		}else{
			std::ostringstream oss;
			oss << lineNumber ;
			id = oss.str() ;
		}

		//std::cout << "process " << id << std::endl;

		std::auto_ptr< Geometry > g;
		try {
			g = io::readWkt( wkt ) ;
		}catch( Exception & e ){
			std::cerr << "[WKT-ERROR]" << wkt << std::endl ;
			continue ;
		}

		TriangulatedSurface triangulatedSurface ;
		try {
			algorithm::triangulate( *g, triangulatedSurface ) ;
		}catch ( Exception & e ){
			std::cerr << "[Exception]" << id << "|" << g->asText() << "(" << e.what() << ")" << std::endl ;
			continue ;
		}catch ( std::exception & e ){
			std::cerr << "[std::exception]" << id << "|" << g->asText() << "(" << e.what() << ")" << std::endl ;
			continue ;
		}catch ( ... ){
			std::cerr << "[unknown]" << id << "|" << g->asText() << "(unknown)" << std::endl ;
			continue ;
		}

		//output triangulated surface
		ofs << id << "|" << triangulatedSurface.asText() << std::endl;
	}



	return 0 ;
}

