#include <SFCGAL/viewer/ViewerWindow.h>
#include <SFCGAL/viewer/ViewerWidget.h>

#include <iostream>

#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>


#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QImage>

#include <QtOpenGL/QGLFramebufferObject>
#include <QtOpenGL/QGLWidget>




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
	viewer()->saveImageToFile( QString("test.png") );
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
void ViewerWindow::createMenus()
{
	/*
	 * File menu
	 */
	_menuFile = menuBar()->addMenu("&File") ;

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

