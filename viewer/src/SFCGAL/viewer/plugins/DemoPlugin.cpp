#include <SFCGAL/viewer/plugins/DemoPlugin.h>
#include <SFCGAL/viewer/ViewerWindow.h>
#include <SFCGAL/viewer/ViewerWidget.h>

#include <iostream>

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QInputDialog>

#include <osg/Geode.h>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/OsgFactory.h>
#include <SFCGAL/algorithm/extrude.h>
#include <SFCGAL/algorithm/convexHull.h>


namespace SFCGAL {
namespace viewer {
namespace plugins {

///
///
///
DemoPlugin::DemoPlugin():
	ViewerPlugin()
{

}

///
///
///
QString DemoPlugin::pluginName() const
{
	return QString( "DemoPlugin" );
}

///
///
///
void DemoPlugin::demoExtrude()
{
	QString wkt = QInputDialog::getText( NULL, QString("Read WKT"), QString("Type WKT") );
	if ( wkt.isEmpty() )
		return ;

	//parse wkt
	std::auto_ptr< Geometry > g( io::readWkt( wkt.toStdString() ) );

	//extrude
	double h = QInputDialog::getDouble(NULL,"Height","Type height");
	std::auto_ptr< Geometry > ext( algorithm::extrude(*g,0.,0.,h) );

	//create osg::Geode
	io::OsgFactory factory ;
	osg::Geometry * osgGeometry = factory.createGeometry( *ext );
	osg::Geode* geode = new osg::Geode;
	geode->setName( wkt.toStdString() );
	geode->addDrawable( osgGeometry );

	viewerWindow()->viewer()->getScene()->addChild( geode );
}


///
///
///
void DemoPlugin::demoConvexhull()
{
	QString wkt = QInputDialog::getText( NULL, QString("Read WKT"), QString("Type WKT") );
	if ( wkt.isEmpty() )
		return ;

	//parse wkt
	std::auto_ptr< Geometry > g( io::readWkt( wkt.toStdString() ) );

	//build convexhull
	std::auto_ptr< Geometry > hull( algorithm::convexHull3D(*g) );

	//create osg::Geode
	io::OsgFactory factory ;
	osg::Geometry * osgGeometry = factory.createGeometry( *hull );
	osg::Geode* geode = new osg::Geode;
	geode->setName( wkt.toStdString() );
	geode->addDrawable( osgGeometry );

	viewerWindow()->viewer()->getScene()->addChild( geode );
}

///
///
///
void DemoPlugin::demoSpiral()
{
	std::vector< Point > points ;

	double r     = 0.0 ;
	double z     = 0.0 ;
	double theta = 0.0 ;

	for ( size_t i = 0; i < 1000; i++ ){
		Point p( r*cos(theta), r*sin(theta), z );
		points.push_back( p );

		if ( i < 500 ){
			r += 0.01 ;
		}else{
			r -= 0.01 ;
		}
		z     += 0.01 ;
		theta += 0.1 ;
	}

	LineString g( points );

	/*
	 * Convert to osg::Geometry
	 */
	io::OsgFactory factory ;
	osg::Geometry * osgGeometry = factory.createGeometry( g );


	/*
	 * provide a color to osg::Geometry
     */
	osg::Vec3Array * colors = new osg::Vec3Array();
	colors->push_back( osg::Vec3(1.0f,0.0f,0.0f) );
	osgGeometry->setColorArray( colors );
	osgGeometry->setColorBinding( osg::Geometry::BIND_OVERALL );

	/*
	 * Create a named Geode (Geometric Node)
	 */
	osg::Geode* geode = new osg::Geode;
	geode->setName( "DemoPlugin::demoSpiral" );
	geode->addDrawable( osgGeometry );

	viewerWindow()->viewer()->getScene()->addChild( geode );
}

///
///
///
void DemoPlugin::demoWkt()
{
	QString wkt = QInputDialog::getText( NULL, QString("Read WKT"), QString("Type WKT") );
	if ( wkt.isEmpty() )
		return ;

	//parse
	std::auto_ptr< Geometry > g( io::readWkt( wkt.toStdString() ) );
	io::OsgFactory factory ;

	osg::Geometry * osgGeometry = factory.createGeometry( *g );
	osg::Geode* geode = new osg::Geode;
	geode->setName( wkt.toStdString() );
	geode->addDrawable( osgGeometry );

	viewerWindow()->viewer()->getScene()->addChild( geode );
}


///
///
///
void DemoPlugin::load()
{
	std::cout << "init DemoPlugin" << std::endl;

	QMenu * pluginMenu = viewerWindow()->menuBar()->addMenu("DemoPlugin") ;

	QAction * actionExtrude = pluginMenu->addAction( QString("&extrude") );
	connect( actionExtrude, SIGNAL(triggered()), this, SLOT( demoExtrude() ) );

	QAction * actionConvexhull = pluginMenu->addAction( QString("&convexhull") );
	connect( actionConvexhull, SIGNAL(triggered()), this, SLOT( demoConvexhull() ) );


	QAction * actionReadWkt = pluginMenu->addAction( QString("&read wkt") );
	connect( actionReadWkt, SIGNAL(triggered()), this, SLOT( demoWkt() ) );

	QAction * actionDemoSpiral = pluginMenu->addAction( QString("&create a spiral") );
	connect( actionDemoSpiral, SIGNAL(triggered()), this, SLOT( demoSpiral() ) );
}



}//plugins
}//viewer
}//SFCGAL

