#include <SFCGAL/viewer/plugins/GridPlugin.h>
#include <SFCGAL/viewer/ViewerWindow.h>
#include <SFCGAL/viewer/ViewerWidget.h>

#include <fstream>

#include <SFCGAL/all.h>
#include <SFCGAL/io/OsgFactory.h>

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>

#include <osgUtil/Optimizer>

namespace SFCGAL {
namespace viewer {
namespace plugins {

///
///
///
GridPlugin::GridPlugin()
{

}

///
///
///
QString GridPlugin::pluginName() const
{
	return "GridPlugin";
}

///
///
///
void GridPlugin::loadGridXYZ()
{
	viewerWindow()->viewer()->stopAnimation();
	QString filename = QFileDialog::getOpenFileName( NULL, QString("Open XYZ grid file") );

	std::ifstream ifs( filename.toStdString().c_str() );
	if ( ! ifs.good() ){
		QMessageBox::warning( NULL, QString( "load grid XYZ" ), QString( "can't load file" ) ) ;
	}

	MultiPoint multiPoint ;
	double x, y, z ;
	while ( ! ifs.eof() && ( ifs >> x >> y >> z ) ){
		if ( z == 0.0 )
			continue ;
		multiPoint.addGeometry( Point(x,y,z) );
	}
	ifs.close();

	//create OSG geometry
	io::OsgFactory factory ;
	osg::Geode * geode = new osg::Geode();
	geode->setName( filename.toStdString() );

	geode->addDrawable( factory.createGeometry( multiPoint ) );

	osgUtil::Optimizer optimizer ;
	optimizer.optimize(geode);

	geode->setCullingActive(false);

	viewerWindow()->viewer()->getScene()->addChild( geode );


	viewerWindow()->viewer()->startAnimation();
}

///
///
///
void GridPlugin::load()
{
	QMenu * pluginMenu = viewerWindow()->menuBar()->addMenu("GridPlugin") ;

	QAction * actionLoadXYZ = pluginMenu->addAction( QString("&load xyz") );
	connect( actionLoadXYZ, SIGNAL(triggered()), this, SLOT( loadGridXYZ() ) );
}


}//plugins
}//viewer
}//SFCGAL



