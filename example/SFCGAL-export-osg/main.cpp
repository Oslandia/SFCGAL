#include <iostream>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>

#include <SFCGAL/algorithm/translate.h>
#include <SFCGAL/algorithm/offset.h>
#include <SFCGAL/algorithm/extrude.h>

#include <SFCGAL/detail/io/OsgFactory.h>



#include <osg/Geometry>
#include <osg/Group>
#include <osg/Geode>

#include <osgDB/WriteFile>

using namespace SFCGAL ;

int main( int argc, char* argv[] ){
	detail::io::OsgFactory factory ;
	std::auto_ptr< Geometry > g( io::readWkt("MULTILINESTRING((-2.705287 -6.782348, -2.052957 -4.368728, -0.356900 -3.912097, 0.980376 -4.857975, 1.339158 -5.966935, 0.328047 -7.043280, -1.987724 -8.706720, -1.922491 -10.076613, -1.726792 -11.087724, 0.882527 -12.457616, 3.491846 -12.490233, 4.242025 -11.120341, 3.720161 -9.489516, 2.480735 -9.098118, -0.519982 -9.130735, -0.291667 -8.413172, 0.915143 -7.075896, 1.958871 -6.619265, 3.883244 -6.358333, 5.677151 -6.651882, 6.231631 -7.238978, 6.329480 -8.413172, 5.481452 -9.326434, 4.829122 -9.717832, 4.698656 -10.207079, 4.861738 -10.826792, 5.350986 -11.381272, 6.329480 -11.479122, 8.514785 -11.479122, 10.112993 -11.022491, 10.700090 -10.533244, 12.265681 -8.967652, 12.493996 -7.858692, 12.526613 -7.108513, 12.233065 -5.999552, 10.765323 -5.412455, 8.319086 -5.249373, 6.133781 -5.184140, 5.057437 -5.021057, 4.926971 -4.401344, 5.253136 -3.814247, 6.786111 -3.422849, 7.699373 -3.879480, 8.090771 -4.531810, 8.938799 -4.629659, 11.156720 -4.597043, 11.287186 -3.031452, 13.766039 -0.487366, 17.908333 -0.389516, 20.615502 -4.075179, 18.267115 -10.598477, 26.975717 -13.109946))") );

	osg::ref_ptr< osg::Group > root = new osg::Group ;

	// save base
	{
		osg::Geometry * osgGeometry = factory.createGeometry( *g );
		osg::Geode* geode = new osg::Geode;
		geode->setName( "base" );
		geode->addDrawable( osgGeometry );
		root->addChild( geode );
	}

	//buffer & extrude
	{
		double radius = 0.7 ;
		std::auto_ptr< Geometry > buffer( algorithm::offset( *g, radius ) );
		algorithm::translate( *buffer, Kernel::Vector_3( 0.0, 0.0, radius ) );

		std::auto_ptr< Geometry > extruded( algorithm::extrude(*buffer,3.0,3.0,3.0) );

		osg::Geometry * osgGeometry = factory.createGeometry( *extruded );
		osg::Geode* geode = new osg::Geode;
		geode->setName( "extruded" );
		geode->addDrawable( osgGeometry );

		root->addChild( geode );
	}


	osgDB::writeNodeFile( *root, "SFCGAL-export-osg.3ds" ) ;
	osgDB::writeNodeFile( *root, "SFCGAL-export-osg.osg" ) ;
	return 0 ;
}
