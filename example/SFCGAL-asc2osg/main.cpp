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

#include <boost/filesystem.hpp>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Grid.h>
#include <SFCGAL/io/asc.h>
#include <SFCGAL/io/osg.h>

using namespace SFCGAL ;

int main( int argc, char* argv[] ){
	if ( argc < 3 ){
		std::cout << "asc2wkt <input-file> <output-file> [-no-zero]" << std::endl ;
		return 1 ;
	}

	std::string inputFile( argv[1] );
	std::string outputFile( argv[2] );

	std::ifstream ifs( argv[1] );
	if ( ! ifs.good() ){
		std::cerr << "can't open input file ('" << argv[1] << "')" << std::endl;
		return 1 ;
	}

	bool filterZeros = false ;
	for ( int i = 1; i < argc; i++ ){
		if ( std::string("-no-zero") == argv[i] ){
			filterZeros = true ;
		}
	}


	try {
		std::cout << "-- loading grid" << std::endl ;
		std::auto_ptr< Grid > grid( io::readASC( ifs ) );

		if ( filterZeros ){
			std::cout << "-- filtering zeros" << std::endl ;
			for ( size_t i = 0; i < grid->nrows(); i++ ){
				for ( size_t j = 0; j < grid->ncols(); j++ ){
					if ( abs(grid->z(i,j)) == 0.0 ){
						grid->z(i,j) = NaN() ;
					}
				}
			}
		}

		std::cout << "-- triangulate grid" << std::endl ;
		std::auto_ptr< TriangulatedSurface > triangulatedSurface( grid->toTrianguledSurface() );
		std::cout << "-- save with osg" << std::endl ;
		io::osgWriteFile( *triangulatedSurface, outputFile );

	} catch ( Exception & e ){
		std::cerr << e.diagnostic() << std::endl ;
		return 1 ;
	} catch ( std::exception& e ){
		std::cerr << e.what() << std::endl ;
		return 1 ;
	}


	return 0 ;
}
