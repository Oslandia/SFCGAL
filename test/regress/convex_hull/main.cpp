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
#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <SFCGAL/Exception.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>

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
int main( int argc, char* argv[] )
{
    /*
     * declare options
     */
    po::options_description desc( "convex hull options : " );
    desc.add_options()
    ( "help", "produce help message" )
    ( "progress", "display progress" )
    ( "verbose",  "verbose mode" )
    ( "filename", po::value< std::string >(), "input filename (id|wkt_[multi]polygon on each line)" )
    ;

    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );

    if ( vm.count( "help" ) ) {
        std::cout << desc << std::endl ;
        return 0;
    }

    bool verbose  = vm.count( "verbose" ) != 0 ;
    bool progress = vm.count( "progress" ) != 0 ;

    std::string filename ;

    if ( vm.count( "filename" ) ) {
        filename = vm["filename"].as< std::string >() ;
    }
    else {
        std::cerr << "missing input file" << std::endl;
        std::cout << desc << std::endl ;
        return 1 ;
    }

    /*
     * open file
     */
    std::ifstream ifs( filename.c_str() );

    if ( ! ifs.good() ) {
        std::cerr << "fail to open : " << filename << std::endl ;
        return 1;
    }

    std::string tri_filename( filename+".convex.wkt" );
    std::ofstream ofs_result( tri_filename.c_str() ) ;

    if ( ! ofs_result.good() ) {
        std::cerr << "fail to write : " << tri_filename << std::endl ;
        return 1;
    }

    std::string error_filename( filename+".error.wkt" );
    std::ofstream ofs_error( error_filename.c_str() ) ;

    if ( ! ofs_error.good() ) {
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

    while ( std::getline( ifs, line ) ) {
        lineNumber++ ;

        boost::algorithm::trim( line );

        if ( line.empty() ) {
            continue ;
        }

        if ( verbose ) {
            std::cout << "#" << line << std::endl;
            std::cout.flush();
        }


        if ( progress && lineNumber % 1000 == 0 ) {
            std::cout.width( 12 ) ;
            boost::chrono::duration<double> elapsed = boost::chrono::system_clock::now() - start;
            std::cout << std::left << lineNumber << "(" << elapsed << " s)"<< std::endl ;
        }


        std::vector< std::string > tokens ;
        boost::algorithm::split( tokens, line, boost::is_any_of( "|" ) );

        std::string const& wkt = tokens.back() ;
        std::string id ;

        if ( tokens.size() > 1 ) {
            id = tokens.front() ;
        }
        else {
            std::ostringstream oss;
            oss << lineNumber ;
            id = oss.str() ;
        }

        //std::cout << "process " << id << std::endl;

        bool failed = true ;

        std::unique_ptr< Geometry > hull, hull3D ;

        try {
            std::unique_ptr< Geometry > g;
            g = io::readWkt( wkt ) ;
            hull = algorithm::convexHull( *g ) ;
            hull3D = algorithm::convexHull3D( *g ) ;
            failed = false ;
        }
        catch ( Exception& e ) {
            std::cerr << "[Exception]" << id << "|" << e.what() << "|" << wkt << std::endl ;
        }
        catch ( std::exception& e ) {
            std::cerr << "[std::exception]" << id << "|" << e.what() << "|" << wkt << std::endl ;
        }
        catch ( ... ) {
            std::cerr << "[...]" << id << "|" << wkt << std::endl ;
        }

        if ( failed ) {
            numFailed++ ;
            ofs_error << line << std::endl ;
        }
        else {
            numSuccess++ ;
        }

        //output triangulated surface
        ofs_result << id << "|" << failed << "|" << hull->asText( 5 ) << std::endl;
    }//end for each line


    ofs_error.close();
    ofs_result.close();


    boost::chrono::duration<double> elapsed = boost::chrono::system_clock::now() - start;
    std::cout << filename << " complete (" << elapsed << " s)---" << std::endl;
    std::cout << numFailed << " failed /" << ( numFailed + numSuccess ) << std::endl ;

    if ( numFailed == 0 ) {
        //delete empty error file
        boost::filesystem::remove( error_filename );
        return EXIT_SUCCESS ;
    }
    else {
        return EXIT_FAILURE ;
    }
}

