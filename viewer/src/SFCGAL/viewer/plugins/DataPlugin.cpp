#include <SFCGAL/viewer/plugins/DataPlugin.h>
#include <SFCGAL/viewer/ViewerWindow.h>
#include <SFCGAL/viewer/ViewerWidget.h>

#include <iostream>

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QInputDialog>


namespace SFCGAL {
namespace viewer {
namespace plugins {

///
///
///
DataPlugin::DataPlugin()
{

}

///
///
///
QString DataPlugin::pluginName() const
{
	return QString("DataPlugin");
}

///
///
///
void DataPlugin::displayInformations()
{
	osg::Group * scene = viewerWindow()->viewer()->getScene() ;
	std::cout << "num children : " << scene->getNumChildren() << std::endl;
	for ( size_t i = 0; i < scene->getNumChildren(); i++ ){
		osg::Node* node = scene->getChild(i) ;
		if ( ! node )
			continue ;
		std::cout << "[" << i << "]" << node->getName() << std::endl; //<< node->getName() << std::endl ;
	}
}

///
///
///
void DataPlugin::load()
{
	QMenu * pluginMenu = viewerWindow()->menuBar()->addMenu("Data") ;

	QAction * actionDisplayInformations = pluginMenu->addAction( QString("&display informations") );
	connect( actionDisplayInformations, SIGNAL(triggered()), this, SLOT( displayInformations() ) );
}



}//plugins
}//viewer
}//SFCGAL

