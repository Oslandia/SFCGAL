#ifndef _SFCGAL_VIEWER_VIEWERWINDOW_H_
#define _SFCGAL_VIEWER_VIEWERWINDOW_H_

#include <QtGui/QMainWindow>


namespace SFCGAL {
namespace viewer {

	class ViewerWidget ;

	/**
	 * osgViewer::Viewer wrapped in a QWidget
	 */
	class ViewerWindow : public QMainWindow {
		Q_OBJECT
	public:
		/**
		 * constructor with a viewer
		 */
		ViewerWindow( ViewerWidget * viewer = 0 );

		/**
		 * get viewer
		 */
		ViewerWidget * viewer() ;
		/**
		 * get viewer
		 */
		void           setViewer( ViewerWidget * viewer ) ;


	public slots:

		void about() ;

		/**
		 * take a screen shot
		 */
		void screenShot() ;

	protected:
		/**
		 * create default menus
		 */
		void createMenus() ;

	protected:
		ViewerWidget * _viewer ;

		QMenu * _menuFile ;
		QMenu * _menuHelp ;
	};

}//viewer
}//SFCGAL


#endif
