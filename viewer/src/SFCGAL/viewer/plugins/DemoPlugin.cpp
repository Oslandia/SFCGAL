#include <SFCGAL/viewer/plugins/DemoPlugin.h>
#include <SFCGAL/viewer/ViewerWindow.h>

#include <iostream>

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>


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
	std::cout << "demo extrude" << std::endl;
}


///
///
///
void DemoPlugin::init()
{
	std::cout << "init DemoPlugin" << std::endl;

	QMenu * pluginMenu = viewerWindow()->menuBar()->addMenu("DemoPlugin") ;
	QAction * actionExtrude = pluginMenu->addAction( QString("&extrude") );
	connect( actionExtrude, SIGNAL(triggered()), this, SLOT( demoExtrude() ) );
}



}//plugins
}//viewer
}//SFCGAL

