#ifndef _SFCGAL_VIEWER_VIEWERPLUGIN_H_
#define _SFCGAL_VIEWER_VIEWERPLUGIN_H_

#include <QtCore/QObject>

namespace SFCGAL {
namespace viewer {

	class ViewerWindow ;

	/**
	 * osgViewer::Viewer wrapped in a QWidget
	 */
	class ViewerPlugin : public QObject {
		Q_OBJECT

		friend class ViewerWindow ;
	public:
		/**
		 * Default constructor
		 */
		ViewerPlugin() ;

		/**
		 * Returns the plugin name
		 */
		virtual QString pluginName() const = 0 ;

		/**
		 * returns the window
		 */
		ViewerWindow * viewerWindow() ;

	protected:
		/**
		 * Load plugin
		 */
		virtual void load() = 0 ;
		/**
		 * Unload plugin
		 */
		virtual void unload() ;
	private:
		ViewerWindow * _viewerWindow ;
	};

}//viewer
}//SFCGAL


#endif
