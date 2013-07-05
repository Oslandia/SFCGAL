#ifndef _SFCGAL_VIEWER_PLUGINS_PSQLPLUGIN_H_
#define _SFCGAL_VIEWER_PLUGINS_PSQLPLUGIN_H_

#ifndef Q_MOC_RUN
#include <boost/scoped_ptr.hpp>
#include <osg/Geode>
#endif

#include <QtGui/QDockWidget>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QListWidget>
#include <QtGui/QLineEdit>

#include <SFCGAL/viewer/ViewerPlugin.h>

namespace SFCGAL {
namespace viewer {
namespace plugins {

class PSQLPluginImpl;
/**
 * data management plugin
 */
class PSQLPlugin : public ViewerPlugin {
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    PSQLPlugin() ;

    ~PSQLPlugin();

    /**
     * Returns the plugin name
     */
    virtual QString pluginName() const ;

    /**
     * Connect to the database
     */
    void connect( const std::string& dbOptions );

    void disconnect();

    /**
     * Do the PostGIS query and returns an osg::Geode
     */
    osg::Geode* doQuery( const std::string& );

    /**
     * Do the PostGIS queries for all layers
     */
    void computeGeometries();

    /**
     * Display all visible layers
     */
    void display();
public slots :
    /**
     * display data information
     */
    void launchConsole();

    void saveLayers();
    void loadLayers();
    void onRefresh();
protected:
    /**
     * init plugin
     */
    virtual void load() ;

private:
    PSQLPluginImpl* impl_;
};


///
/// SQL editor for a layer
class SQLConsole : public QDockWidget {
    Q_OBJECT
public:
    SQLConsole( PSQLPlugin* plugin );

    //
    // connect to a QListWidgetItem storing the sql query and the resulting geometry
    void connectItem( QListWidgetItem* );

    //
    //
    void disconnectItem();

public slots:
    void onQuery();
    void onReconnect();
private:
    QPlainTextEdit* sqlEdit_;
    QPushButton* okBtn_;
    QPushButton* connectBtn_;
    QLineEdit* dbOptions_;

    QListWidgetItem* item_;

    PSQLPlugin* plugin_;
};

class LayersWidget : public QDockWidget {
    Q_OBJECT
public:
    LayersWidget( PSQLPlugin* plugin, SQLConsole* );

    QListWidget* widget() {
        return listWidget_;
    }

    void addLayer( const QString& name, bool visible, const QString& dbOptions, const QString& query );
public slots:
    void onAdd();
    void onClick( const QModelIndex& idx );

    void onEdit();
    void onRemove();
    void onZoomToLayer();
private:
    QListWidget* listWidget_;
    QPushButton* addBtn_;
    SQLConsole* console_;

    PSQLPlugin* plugin_;
};

}//plugins
}//viewer
}//SFCGAL


#endif

