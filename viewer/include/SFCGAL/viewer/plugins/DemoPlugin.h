#ifndef _SFCGAL_VIEWER_PLUGINS_DEMOPLUGIN_H_
#define _SFCGAL_VIEWER_PLUGINS_DEMOPLUGIN_H_

#include <SFCGAL/viewer/ViewerPlugin.h>


namespace SFCGAL {
namespace viewer {
namespace plugins {

/**
 * demo plugin
 */
class DemoPlugin : public ViewerPlugin {
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    DemoPlugin() ;

    /**
     * Returns the plugin name
     */
    virtual QString pluginName() const ;

public slots :
    /**
     * demo extrude
    */
    void demoSpiral();
    /**
     * demo extrude
     */
    void demoWkt();
    /**
     * demo extrude
     */
    void demoExtrude();
    /**
     * demo convexHull
     */
    void demoConvexhull();
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

