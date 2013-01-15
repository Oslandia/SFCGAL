#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <SFCGAL/Exception.h>

#include <SFCGAL/all.h>

#include <SFCGAL/io/wkt.h>
#include <SFCGAL/algorithm/convexHull.h>

#include <boost/chrono.hpp>

#include <boost/filesystem.hpp>

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
	po::options_description desc("convex hull options : ");
	desc.add_options()
	    ("help", "produce help message")
	    ("progress", "display progress")
	    ("verbose",  "verbose mode")
	    ("filename", po::value< std::string >(), "input filename (id|wkt_[multi]polygon on each line)")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
	    std::cout << desc << std::endl ;
	    return 0;
	}

	bool verbose  = vm.count("verbose") != 0 ;
	bool progress = vm.count("progress") != 0 ;

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

	std::string tri_filename( filename+".convex.wkt" );
	std::ofstream ofs_result( tri_filename.c_str() ) ;
	if ( ! ofs_result.good() ){
		std::cerr << "fail to write : " << tri_filename << std::endl ;
		return 1;
	}

	std::string error_filename( filename+".error.wkt" );
	std::ofstream ofs_error( error_filename.c_str() ) ;
	if ( ! ofs_error.good() ){
		std::cerr << "fail to write : " << error_filename << std::endl ;
		return 1;
	}


	//boost::timer timer ;
	boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

	/*
	 * process file
	 */
	int lineNumber = 0 , numFailed = 0 , numSuccess = 0 ;
	std::string line ;

	while ( std::getline( ifs, line ) ){
		lineNumber++ ;

		boost::algorithm::trim( line );
		if ( line.empty() ){
			continue ;
		}

		if ( verbose ){
			std::cout << "#" << line << std::endl;
			std::cout.flush();
		}


		if ( progress && lineNumber % 1000 == 0 ){
			std::cout.width(12) ;
			boost::chrono::duration<double> elapsed = boost::chrono::system_clock::now() - start;
			std::cout << std::left << lineNumber << "(" << elapsed << " s)"<< std::endl ;
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

		bool failed = true ;

		TriangulatedSurface triangulatedSurface ;
		try {
			std::auto_ptr< Geometry > g;
			g = io::readWkt( wkt ) ;
			std::auto_ptr< Geometry > hull( algorithm::convexHull( *g ) ) ;
			std::auto_ptr< Geometry > hull3D( algorithm::convexHull3D( *g ) ) ;
			failed = false ;
		}catch ( Exception & e ){
			std::cerr << "[Exception]" << id << "|" << e.what() << "|" << wkt << std::endl ;
		}catch ( std::exception & e ){
			std::cerr << "[std::exception]" << id << "|" << e.what() << "|" << wkt << std::endl ;
		}catch ( ... ){
			std::cerr << "[...]" << id << "|" << wkt << std::endl ;
		}

		if ( failed ){
			numFailed++ ;
			ofs_error << line << std::endl ;
		}else{
			numSuccess++ ;
		}

		//output triangulated surface
		ofs_result << id << "|" << failed << "|" << triangulatedSurface.asText(5) << std::endl;
	}//end for each line


	ofs_error.close();
	ofs_result.close();


	boost::chrono::duration<double> elapsed = boost::chrono::system_clock::now() - start;
	std::cout << filename << " complete (" << elapsed << " s)---" << std::endl;
	std::cout << numFailed << " failed /" << (numFailed + numSuccess) << std::endl ;

	if ( numFailed == 0 ){
		//delete empty error file
		boost::filesystem::remove( error_filename );
		return EXIT_SUCCESS ;
	}else{
		return EXIT_FAILURE ;
	}
}

