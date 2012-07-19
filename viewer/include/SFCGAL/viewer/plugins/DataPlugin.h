#ifndef _SFCGAL_VIEWER_PLUGINS_DATAPLUGIN_H_
#define _SFCGAL_VIEWER_PLUGINS_DATAPLUGIN_H_

#include <SFCGAL/viewer/ViewerPlugin.h>

namespace osg {
	class Node ;
}

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


		static void displayNodeInformation( osg::Node* node, std::ostream & s, const size_t & depth = 0 ) ;

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

