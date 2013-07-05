#ifndef _SFCGAL_VIEWER_VIEWERWINDOW_H_
#define _SFCGAL_VIEWER_VIEWERWINDOW_H_

#include <QtGui/QMainWindow>


namespace SFCGAL {
namespace viewer {

class ViewerPlugin ;
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
    ViewerWindow( ViewerWidget* viewer = 0 );

    /**
     * destructor
     */
    ~ViewerWindow() ;

    /**
     * get viewer
     */
    ViewerWidget* viewer() ;
    /**
     * get viewer
     */
    void           setViewer( ViewerWidget* viewer ) ;

    /**
     * add a plugin to the window
     */
    void           addPlugin( ViewerPlugin* plugin ) ;

public slots:

    /**
     * load a file
     */
    void loadFile() ;
    /**
     * save to a file
     */
    void saveFile() ;

    /**
     * display about message
     */
    void about() ;

    /**
     * take a screen shot
     */
    void screenShot() ;

    /**
     * switch to a GIS-oriented manipulator
     */
    void toGISView();

    /**
     * switch to a free view manipulator
     */
    void toFreeView();
protected:
    /**
     * create default menus
     */
    void createMenus() ;

protected:
    ViewerWidget* _viewer ;

    QMenu* _menuFile ;
    QMenu* _menuData ;
    QMenu* _menuHelp ;

    std::vector< ViewerPlugin* > _plugins ;
};

}//viewer
}//SFCGAL


#endif
