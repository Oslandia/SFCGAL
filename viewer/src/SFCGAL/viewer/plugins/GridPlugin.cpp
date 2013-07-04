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
#include <SFCGAL/viewer/plugins/GridPlugin.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/MultiPoint.h>

#include <SFCGAL/viewer/ViewerWindow.h>
#include <SFCGAL/viewer/ViewerWidget.h>

#include <fstream>

#include <SFCGAL/detail/io/OsgFactory.h>

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
    QString filename = QFileDialog::getOpenFileName( NULL, QString( "Open XYZ grid file" ) );

    std::ifstream ifs( filename.toStdString().c_str() );

    if ( ! ifs.good() ) {
        QMessageBox::warning( NULL, QString( "load grid XYZ" ), QString( "can't load file" ) ) ;
    }

    MultiPoint multiPoint ;
    double x, y, z ;

    while ( ! ifs.eof() && ( ifs >> x >> y >> z ) ) {
        if ( z == 0.0 ) {
            continue ;
        }

        multiPoint.addGeometry( Point( x,y,z ) );
    }

    ifs.close();

    //create OSG geometry
    detail::io::OsgFactory factory ;
    osg::Geode* geode = new osg::Geode();
    geode->setName( filename.toStdString() );

    geode->addDrawable( factory.createGeometry( multiPoint ) );

    osgUtil::Optimizer optimizer ;
    optimizer.optimize( geode );

    geode->setCullingActive( false );

    viewerWindow()->viewer()->getScene()->addChild( geode );


    viewerWindow()->viewer()->startAnimation();
}

///
///
///
void GridPlugin::load()
{
    QMenu* pluginMenu = viewerWindow()->menuBar()->addMenu( "GridPlugin" ) ;

    QAction* actionLoadXYZ = pluginMenu->addAction( QString( "&load xyz" ) );
    connect( actionLoadXYZ, SIGNAL( triggered() ), this, SLOT( loadGridXYZ() ) );
}


}//plugins
}//viewer
}//SFCGAL



