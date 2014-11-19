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
    ViewerWidget( osg::ArgumentParser& arguments ) ;

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
    static ViewerWidget* createFromArguments( osg::ArgumentParser& arguments ) ;

    /**
     * Save image to file
     */
    void saveImageToFile() ;


    void setCameraToExtent( const osg::BoundingBox& );
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
