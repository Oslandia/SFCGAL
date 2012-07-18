#include <SFCGAL/viewer/ViewerWindow.h>
#include <SFCGAL/viewer/ViewerWidget.h>

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

#include <osgDB/ReadFile.h>
#include <osgDB/WriteFile.h>

namespace SFCGAL {
namespace viewer {

///
///
///
ViewerWindow::ViewerWindow( ViewerWidget * viewer )
{
	setViewer(viewer);

	createMenus();
}

///
///
///
ViewerWidget * ViewerWindow::viewer()
{
	return _viewer ;
}

///
///
///
void ViewerWindow::setViewer( ViewerWidget * viewer )
{
	_viewer = viewer ;

	QWidget * centralWidget = new QWidget ;
	QVBoxLayout * layout = new QVBoxLayout ;

	if ( _viewer ){
		layout->addWidget( _viewer );
	}

	centralWidget->setLayout( layout );
	setCentralWidget(centralWidget);
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

	std::cout << QDir::currentPath().toStdString() << std::endl;
	QString filename = QFileDialog::getOpenFileName( NULL,"select a file to open", QDir::currentPath() ) ;

	osg::Node* node = osgDB::readNodeFile( filename.toStdString() );
	if ( ! node ){
		QMessageBox::warning( this, "load file error", QString("can't read file...") );
	}else{
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

	std::cout << "save as osgt" << std::endl;
	osgDB::writeNodeFile( *viewer()->getScene(), "scene.osgb" );


	viewer()->startAnimation();
}

///
///
///
void ViewerWindow::createMenus()
{
	/*
	 * File menu
	 */
	_menuFile = menuBar()->addMenu("&File") ;

	//-- load a file
	QAction *actionLoadFile = _menuFile->addAction("&Load a file...");
	connect( actionLoadFile, SIGNAL(triggered()), this, SLOT( loadFile() ) );

	//-- save as file
	QAction *actionSaveFile = _menuFile->addAction("&save to file...");
	connect( actionSaveFile, SIGNAL(triggered()), this, SLOT( saveFile() ) );


	//-- screenshot
	QAction *actionSaveAsImage = _menuFile->addAction("&Save as image...");
	connect( actionSaveAsImage, SIGNAL(triggered()), this, SLOT( screenShot() ) );

	_menuFile->addSeparator();
	//-- exit
	QAction *actionExit = _menuFile->addAction("&Exit");
	connect( actionExit, SIGNAL(triggered()), qApp, SLOT( quit() ) );


	/*
	 * Help menu
	 */
	_menuHelp = menuBar()->addMenu("&Help") ;

	//-- about
	QAction *actionAbout = _menuHelp->addAction("&About");
	connect( actionAbout, SIGNAL(triggered()), this, SLOT( about() ) );
}



}//viewer
}//SFCGAL

