/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <iostream>
#include <fstream>

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

#include <SFCGAL/io/osg.h>
#include <SFCGAL/detail/io/OsgFactory.h>

#include <osg/Geometry>
#include <osg/Group>
#include <osg/Geode>

#include <osgDB/WriteFile>

using namespace SFCGAL ;


int main( int argc, char* argv[] )
{
	std::string fileName;
	std::string outputFile;
	if ( argc > 2 ) {
		fileName = argv[1];
		outputFile = argv[2];
	}
	else {
		std::cout << "Argument: input_wkt_file_name output_osg_file_name" << std::endl;
		return 1;
	}
	std::ifstream ifs( fileName.c_str() );

	osg::ref_ptr< osg::Group > osgGroup = new osg::Group;
	
	std::string line;
	int n_lines = 1;

	osg::ref_ptr< osg::Geode > geode = new osg::Geode;
	geode->setName( "geom" );
	geode->setDataVariance( osg::Object::STATIC );

	while (	std::getline( ifs, line ) ) {
		boost::algorithm::trim( line );
		if ( line.empty() ) {
			continue;
		}
		std::cout << n_lines++ << std::endl;
		std::auto_ptr<Geometry> g( io::readWkt( line ) );
		
		SFCGAL::detail::io::OsgFactory factory ;
		
		osg::ref_ptr< osg::Geometry > osgGeometry = factory.createGeometry( *g );
		geode->addDrawable( osgGeometry );	
	}
	osgGroup->addChild( geode );

	osgDB::writeNodeFile( *osgGroup, outputFile.c_str() ) ;
}

