#ifndef _SFCGAL_VIEWER_PLUGINS_GRIDPLUGIN_H_
#define _SFCGAL_VIEWER_PLUGINS_GRIDPLUGIN_H_

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
class GridPlugin : public ViewerPlugin {
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    GridPlugin() ;

    /**
     * Returns the plugin name
     */
    virtual QString pluginName() const ;

public slots :
    /**
     * display data information
     */
    void loadGridXYZ();
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

