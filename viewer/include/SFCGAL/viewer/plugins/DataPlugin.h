#ifndef _SFCGAL_VIEWER_PLUGINS_DATAPLUGIN_H_
#define _SFCGAL_VIEWER_PLUGINS_DATAPLUGIN_H_

#include <SFCGAL/viewer/ViewerPlugin.h>


namespace SFCGAL {
namespace viewer {
namespace plugins {

	/**
	 * data management plugin
	 */
	class DataPlugin : public ViewerPlugin {
		Q_OBJECT
	public:
		/**
		 * Default constructor
		 */
		DataPlugin() ;

		/**
		 * Returns the plugin name
		 */
		virtual QString pluginName() const ;

	public slots :
		/**
		 * display data information
		 */
		void displayInformations();
	protected:
		/**
		 * init plugin
		 */
		virtual void load() ;
	};

}//plugins
}//viewer
}//SFCGAL


#endif

