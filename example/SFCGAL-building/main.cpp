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
#include <SFCGAL/all.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <SFCGAL/Exception.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/detail/generator/building.h>

// optional osg support for outputs
#include <SFCGAL/io/osg.h>

using namespace SFCGAL ;



class InputData {
public:
	InputData( const std::string & wkt_ = "", double wallHeight_ = 5.0, double roofSlope_ = 1.0 ):
		wkt(wkt_),
		roofSlope(roofSlope_),
		wallHeight(wallHeight_)
	{

	}
public:
	std::string wkt ;
	double roofSlope ;
	double wallHeight ;
};


 * @brief read input file
std::vector< InputData > readInputFile( const std::string & filename ){
	std::vector< InputData > result ;

	std::ifstream ifs(filename.c_str());
	if ( ! ifs.good() ){
		BOOST_THROW_EXCEPTION( Exception("can't open file") );
		return result ;
	}

	std::string line ;
	while ( std::getline(ifs,line) ){
		std::istringstream iss( line );

		std::string wkt ;
		std::string wallHeight = "10.0" ;
		std::string roofSlope = "1.0" ;

		std::getline( iss, wkt, '|' ) && std::getline( iss, wallHeight, '|' ) && std::getline( iss, roofSlope, '|' ) ;
		if ( ! wkt.empty() ){
			result.push_back(
				InputData(
					wkt,
					boost::lexical_cast< double >( wallHeight ),
					boost::lexical_cast< double >( roofSlope )
				)
			);
		}
	}
	ifs.close();
	return result ;
}

void processFile( const std::string & filename ){
	std::vector< InputData > data = readInputFile(filename);

	std::auto_ptr< GeometryCollection > result( new GeometryCollection() );

	std::cout << "#" << data.size() << std::endl;
	for ( size_t i = 0; i < data.size(); i++ ){
		std::auto_ptr< Geometry > g( io::readWkt( data[i].wkt ) );
		std::auto_ptr< Geometry > building( generator::building( *g, data[i].wallHeight, data[i].roofSlope ) ) ;
		result->addGeometry( building.release() );
	}

	io::osgWriteFile( *result, "building.osg" );
}



int main( int argc, char* argv[] ){
	if ( argc < 2 ){
		std::cerr << "usage : SFCGAL-building <input-file>" << std::endl ;
		std::cerr << "file format : " << std::endl;
		std::cerr << "<wkt(polygon|multipolygon)|wallHeight>|<roofSlope>" << std::endl;
		return 0 ;
	}

	try {
		processFile( argv[1] ) ;
		return 0 ;
	}catch( Exception & e ){
		std::cerr << e.diagnostic() << std::endl;
		return 1;
	}

	return 0 ;
}
