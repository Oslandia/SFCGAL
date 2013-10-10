/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
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

