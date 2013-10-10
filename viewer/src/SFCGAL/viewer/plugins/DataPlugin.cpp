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
    return QString( "DataPlugin" );
}

///
///
///
void DataPlugin::displayNodeInformation( osg::Node* node, std::ostream& s, const size_t& depth )
{
    std::string indent ;

    for ( size_t i = 0; i < depth; i++ ) {
        indent += ' ';
    }

    s << indent << "Node[" << node->getName() << "]" << std::endl;
    osg::Group* group = node->asGroup() ;

    if ( group ) {
        for ( size_t i = 0; i < group->getNumChildren(); i++ ) {
            s << indent << "child[" << i << "]" << std::endl;
            displayNodeInformation( group->getChild( i ), s, depth + 1 ) ;
        }
    }
    else {
        osg::Geode* geode = dynamic_cast< osg::Geode* >( node ) ;
        s << indent << "Geode[" << geode->getBoundingBox().xMin() << " " << geode->getBoundingBox().xMax() << " " ;
        s << geode->getBoundingBox().yMin() << " " << geode->getBoundingBox().yMax() << " " ;
        s << geode->getBoundingBox().zMin() << " " << geode->getBoundingBox().zMax() << "]" << std::endl;
    }
}

///
///
///
void DataPlugin::displayInformations()
{
    displayNodeInformation( viewerWindow()->viewer()->getScene(), std::cout );
}

///
///
///
void DataPlugin::load()
{
    QMenu* pluginMenu = viewerWindow()->menuBar()->addMenu( "Data" ) ;

    QAction* actionDisplayInformations = pluginMenu->addAction( QString( "&display informations" ) );
    connect( actionDisplayInformations, SIGNAL( triggered() ), this, SLOT( displayInformations() ) );
}



}//plugins
}//viewer
}//SFCGAL

