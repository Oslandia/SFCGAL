#ifndef _SFCGAL_VIEWER_VIEWERWIDGET_H_
#define _SFCGAL_VIEWER_VIEWERWIDGET_H_

#include <QtCore/QTimer>
#include <QtGui/QWidget>

#include <osgViewer/Viewer>
#include <osgQt/GraphicsWindowQt>

namespace SFCGAL {
namespace viewer {

	/**
	 * osgViewer::Viewer wrapped in a QWidget
	 */
	class ViewerWidget : public QWidget, public osgViewer::Viewer {
		Q_OBJECT
	public:
		/**
		 * Constructor with a threadingModel
		 */
		ViewerWidget() ;
		/**
		 * Constructor with a threadingModel
		 */
		ViewerWidget(osg::ArgumentParser & arguments) ;

		/**
		 * returns the scene node
		 */
		osg::Group*   getScene() ;

		/**
		 * Get GraphicsWindows
		 */
		osgQt::GraphicsWindowQt* getGraphicsWindowQt() ;


		//-- [QWidget]
		virtual void paintEvent( QPaintEvent* event ) ;

		/**
		 * [factory]create viewer from command line arguments
		 */
		static ViewerWidget* createFromArguments( osg::ArgumentParser & arguments ) ;

		/**
		 * Save image to file
		 */
		void saveImageToFile() ;


		void setCameraToExtent( double left, double right, double bottom, double top, double near, double far );
	public slots :
		/**
		 * start animation
		 */
		void startAnimation() ;
	    /**
		 * stop animation
		 */
		void stopAnimation() ;

	protected:
		QTimer       _timer;
		osg::Camera* _camera ;
		osg::Group*  _scene ;


		/**
		 * [helper]create a camera with the given parameters
		 */
		osg::Camera* createCamera( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false ) ;

	private:
		void initViewer() ;
	};

}//viewer
}//SFCGAL


#endif
