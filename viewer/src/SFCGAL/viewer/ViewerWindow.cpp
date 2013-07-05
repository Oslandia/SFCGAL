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
#include <SFCGAL/viewer/ViewerWindow.h>
#include <SFCGAL/viewer/ViewerWidget.h>
#include <SFCGAL/viewer/ViewerPlugin.h>

#include <SFCGAL/detail/tools/Log.h>

#include <iostream>

#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>

#include <QtGui/QFileDialog>


#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QMessageBox>


#include <QtOpenGL/QGLFramebufferObject>
#include <QtOpenGL/QGLWidget>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/OrbitManipulator>
#include <SFCGAL/viewer/GISManipulator.h>

namespace SFCGAL {
namespace viewer {

///
///
///
ViewerWindow::ViewerWindow( ViewerWidget* viewer )
{
    setViewer( viewer );

    createMenus();
}

///
///
///
ViewerWindow::~ViewerWindow()
{
    for ( std::vector< ViewerPlugin* >::iterator it = _plugins.begin(); it != _plugins.end(); ++it ) {
        ( *it )->unload();
        delete *it ;
    }

    _plugins.clear();
}

///
///
///
ViewerWidget* ViewerWindow::viewer()
{
    return _viewer ;
}

///
///
///
void ViewerWindow::setViewer( ViewerWidget* viewer )
{
    _viewer = viewer ;

    QWidget* centralWidget = new QWidget ;
    QVBoxLayout* layout = new QVBoxLayout ;

    if ( _viewer ) {
        layout->addWidget( _viewer );
    }

    centralWidget->setLayout( layout );
    setCentralWidget( centralWidget );
}


///
///
///
void   ViewerWindow::addPlugin( ViewerPlugin* plugin )
{
    BOOST_ASSERT( plugin != NULL );

    SFCGAL_INFO( boost::format( "loading ViewerPlugin %1%..." ) % plugin->pluginName().toStdString() );

    _plugins.push_back( plugin );
    plugin->_viewerWindow = this ;
    plugin->load() ;
}



///
///
///
//void ViewerWindow::screenShot()
//{
//	QImage image( viewer()->size(), QImage::Format_ARGB32 );
//	QPainter painter( &image );
//	centralWidget()->render( &painter ) ;
//	image.save("test.png","PNG");
//}


///
///
///
void ViewerWindow::screenShot()
{
    viewer()->stopAnimation();
    viewer()->saveImageToFile();
    viewer()->startAnimation();
}



///
///
///
void ViewerWindow::about()
{
    QApplication::aboutQt();
}

///
///
///
void ViewerWindow::loadFile()
{
    viewer()->stopAnimation();

    QString filename = QFileDialog::getOpenFileName( NULL,"select a file to open", QDir::currentPath() ) ;

    osg::Node* node = osgDB::readNodeFile( filename.toStdString() );

    if ( ! node ) {
        QMessageBox::warning( this, "load file error", QString( "can't read file..." ) );
    }
    else {
        node->setName( filename.toStdString() );
        viewer()->getScene()->addChild( node );
    }

    viewer()->startAnimation();
}


///
///
///
void ViewerWindow::saveFile()
{
    viewer()->stopAnimation();

    QString filename = QFileDialog::getOpenFileName( NULL,"select save file name", QDir::currentPath() ) ;

    if ( ! filename.isEmpty() ) {
        osgDB::writeNodeFile( *viewer()->getScene(), filename.toStdString() );
    }

    viewer()->startAnimation();
}

///
///
///
void ViewerWindow::toGISView()
{
    viewer()->setCameraManipulator( new GISManipulator(), /* resetPosition = */ true );
}

///
///
///
void ViewerWindow::toFreeView()
{
    viewer()->setCameraManipulator( new osgGA::OrbitManipulator(), /* resetPosition = */ true );
}

///
///
///
void ViewerWindow::createMenus()
{
    /*
     * File menu
     */
    _menuFile = menuBar()->addMenu( "&File" ) ;

    //-- load a file
    QAction* actionLoadFile = _menuFile->addAction( "&Load a file..." );
    connect( actionLoadFile, SIGNAL( triggered() ), this, SLOT( loadFile() ) );

    //-- save as file
    QAction* actionSaveFile = _menuFile->addAction( "&save to file..." );
    connect( actionSaveFile, SIGNAL( triggered() ), this, SLOT( saveFile() ) );


    //-- screenshot
    QAction* actionSaveAsImage = _menuFile->addAction( "&Save as image..." );
    connect( actionSaveAsImage, SIGNAL( triggered() ), this, SLOT( screenShot() ) );

    _menuFile->addSeparator();
    //-- exit
    QAction* actionExit = _menuFile->addAction( "&Exit" );
    connect( actionExit, SIGNAL( triggered() ), qApp, SLOT( quit() ) );

    /*
     * Display menu
     */
    QMenu* displayMenu = menuBar()->addMenu( "&Display" );
    QAction* gisView = displayMenu->addAction( "GIS view\t0" );
    connect( gisView, SIGNAL( triggered() ), this, SLOT( toGISView() ) );

    QAction* freeView = displayMenu->addAction( "Free view\t1" );
    connect( freeView, SIGNAL( triggered() ), this, SLOT( toFreeView() ) );

    /*
     * Help menu
     */
    _menuHelp = menuBar()->addMenu( "&Help" ) ;

    //-- about
    QAction* actionAbout = _menuHelp->addAction( "&About" );
    connect( actionAbout, SIGNAL( triggered() ), this, SLOT( about() ) );
}



}//viewer
}//SFCGAL

