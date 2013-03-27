/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <SFCGAL/viewer/plugins/PSQLPlugin.h>
#include <SFCGAL/viewer/ViewerWindow.h>
#include <SFCGAL/viewer/ViewerWidget.h>
#include <SFCGAL/io/OsgFactory.h>
#include <SFCGAL/io/wkt.h>

#include <SFCGAL/all.h>

#include <osg/Geode>

#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QMenuBar>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>

#include "db.h"

namespace SFCGAL {
namespace viewer {
namespace plugins {

	struct LayerInfo
	{
		LayerInfo() : query(""), geode(0) {}
		std::string dbOptions;
		std::string query;
		osg::ref_ptr<osg::Geode> geode;
	};

	SQLConsole::SQLConsole( PSQLPlugin* plugin ) : QDockWidget( NULL ), plugin_(plugin)
	{
		setMinimumSize( QSize( 300, 50 ));
		setWindowTitle( "SQL Console" );
		
		QWidget* dockWidgetContents = new QWidget();
		dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
		QVBoxLayout* verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
		verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

		QLabel* lbl1 = new QLabel("DB options");
		verticalLayout_2->addWidget( lbl1 );

		QHBoxLayout* hbox2 = new QHBoxLayout();
		dbOptions_ = new QLineEdit( dockWidgetContents );
		hbox2->addWidget( dbOptions_ );

		connectBtn_ = new QPushButton( dockWidgetContents );
		connectBtn_->setText("&Connect");
		hbox2->addWidget( connectBtn_ );
		verticalLayout_2->addItem( hbox2 );

		QLabel* lbl2 = new QLabel("SQL");
		verticalLayout_2->addWidget( lbl2 );

		sqlEdit_ = new QPlainTextEdit(dockWidgetContents);
		sqlEdit_->setObjectName(QString::fromUtf8("sqlEdit"));
		sqlEdit_->setMinimumSize(QSize(200, 0));
		
		verticalLayout_2->addWidget(sqlEdit_);
		
		QHBoxLayout* horizontalLayout = new QHBoxLayout();
		horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
		QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		
		horizontalLayout->addItem(horizontalSpacer);
		
		okBtn_ = new QPushButton(dockWidgetContents);
		okBtn_->setObjectName(QString::fromUtf8("okBtn"));
		okBtn_->setText( "&Edit" );
		
		horizontalLayout->addWidget(okBtn_);
		
		verticalLayout_2->addLayout(horizontalLayout);
		
		setWidget(dockWidgetContents);
		
		dbOptions_->setEnabled( false );
		sqlEdit_->setEnabled( false );
		okBtn_->setEnabled( false );

		// connect signal
		connect( okBtn_, SIGNAL( clicked()), this, SLOT( onQuery() ));
		connect( connectBtn_, SIGNAL( clicked()), this, SLOT( onReconnect() ));
	}
	
	void SQLConsole::connectItem( QListWidgetItem* item )
	{
		item_ = item;
		dbOptions_->setEnabled( true );
		connectBtn_->setEnabled( true );
		sqlEdit_->setEnabled( true );
		okBtn_->setText( "&Ok" );
		okBtn_->setEnabled( true );
		// disconnect from the db
		disconnect();

		sqlEdit_->clear();
		LayerInfo* linfo = (LayerInfo*)item_->data( Qt::UserRole ).value<void *>();

		dbOptions_->setText( QString::fromStdString(linfo->dbOptions) );
		sqlEdit_->appendPlainText( QString::fromStdString(linfo->query) );
	}

	void SQLConsole::disconnectItem()
	{
		item_ = 0;
		dbOptions_->setEnabled( false );
		sqlEdit_->setEnabled( false );
		okBtn_->setText( "&Edit" );
		okBtn_->setEnabled( false );
		connectBtn_->setEnabled( false );
	}

	void SQLConsole::onReconnect()
	{
		plugin_->disconnect();
		plugin_->connect( dbOptions_->text().toStdString() );
	}

	void SQLConsole::onQuery()
	{
		plugin_->connect( dbOptions_->text().toStdString() );
		osg::Geode* geode = plugin_->doQuery( sqlEdit_->toPlainText().toStdString() );
		
		// store the query and its result
		LayerInfo* linfo = (LayerInfo*)item_->data( Qt::UserRole ).value<void *>();
		linfo->query = sqlEdit_->toPlainText().toStdString();
		linfo->dbOptions = dbOptions_->text().toStdString();
		linfo->geode = osg::ref_ptr<osg::Geode>( geode );

		// refresh the display
		plugin_->display();
	}

	LayersWidget::LayersWidget( PSQLPlugin* plugin, SQLConsole* console ) : console_(console), plugin_(plugin)
	{
		setMinimumSize( QSize( 100, 50 ));

		QWidget* dockWidgetContents = new QWidget();
		dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
		QVBoxLayout* verticalLayout = new QVBoxLayout(dockWidgetContents);
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		listWidget_ = new QListWidget(dockWidgetContents);
		listWidget_->setObjectName(QString::fromUtf8("listWidget"));

		addBtn_ = new QPushButton( dockWidgetContents );
		addBtn_->setText( "&Add" );

		verticalLayout->addWidget( addBtn_ );
		verticalLayout->addWidget( listWidget_ );

		setWidget(dockWidgetContents);
		setWindowTitle("Layers");

		// connect signal
		connect( addBtn_, SIGNAL( clicked()), this, SLOT( onAdd() ));

		//
		connect( listWidget_, SIGNAL( pressed(const QModelIndex&) ), this, SLOT( onClick(const QModelIndex&) ) );
	}

	void LayersWidget::addLayer( const QString& name, bool visible, const QString& dbOptions, const QString& query )
	{
		QListWidgetItem* item = new QListWidgetItem( name );
		item->setFlags( item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable );
		if ( visible ) {
			item->setCheckState( Qt::Checked );
		}
		else {
			item->setCheckState( Qt::Unchecked );
		}
		LayerInfo* linfo = new LayerInfo;
		linfo->dbOptions = dbOptions.toStdString();
		linfo->query = query.toStdString();
		item->setData( Qt::UserRole, qVariantFromValue((void*)linfo) );
		listWidget_->addItem( item );
	}

	void LayersWidget::onAdd()
	{
		addLayer( "layer", true, "", "" );
	}

	void LayersWidget::onClick( const QModelIndex& idx )
	{
		if ( QApplication::mouseButtons() & Qt::RightButton ) {
			QMenu* menu = new QMenu;
			QAction* removeAction = menu->addAction("&Remove");
			connect( removeAction, SIGNAL( triggered() ), this, SLOT( onRemove() ) );
			QAction* zoomAction = menu->addAction("&Zoom to layer extent");
			connect( zoomAction, SIGNAL( triggered() ), this, SLOT( onZoomToLayer() ) );

			menu->popup( QCursor::pos() );
		}
		else {
			// left button
			onEdit();
		}
	}

	void LayersWidget::onEdit()
	{
		//		std::cout << "edit " << listWidget_->currentRow() << std::endl;
		console_->connectItem( listWidget_->item( listWidget_->currentRow() ) );
	}

	void LayersWidget::onRemove()
	{
		console_->disconnectItem();

		int current = listWidget_->currentRow();
		QListWidgetItem* widget = listWidget_->item( current );
		listWidget_->removeItemWidget( widget );
		delete widget;
	}

	void LayersWidget::onZoomToLayer()
	{
		int current = listWidget_->currentRow();
		QListWidgetItem* item = listWidget_->item( current );
		LayerInfo* linfo = (LayerInfo *)item->data( Qt::UserRole ).value<void *>();
		if ( linfo->geode ) {
			const osg::BoundingBox& bbox = linfo->geode->getBoundingBox();
			std::cout << bbox.xMin() << " ; " << bbox.xMax() << " " << bbox.yMin() << " ; " << bbox.yMax() << " " << bbox.zMin() << " ; " << bbox.zMax() << std::endl;
			std::cout << bbox.center()[0] << " " << bbox.center()[1] << " " << bbox.center()[2] << std::endl;
			plugin_->viewerWindow()->viewer()->setCameraToExtent( bbox );
		}
	}

	std::string wkbToWkt( const std::string& wkb, const Db::Connection& db )
	{
		// wkb is a hex textual representation of a binary array
		std::string q = "SELECT ST_AsText(ST_GeomFromEWKB('\\x" + wkb + "'))";
		// temporary : use a PostGIS query to convert from wkb to wkt
		Db::Result r = db.exec( q );
		if ( r.size() == 1 ) {
			return r[0][0].as<std::string>();
		}
		return "";
	}


	struct Color
	{
		Color( float r, float g, float b ) : r(r), g(g), b(b) {}
		float r, g, b;
	};
	
	//
	// The Pimpl class for this plugin
	struct PSQLPluginImpl
	{
		PSQLPluginImpl( PSQLPlugin* super ) : geometry_oid_(0), console(0), layers(0), isConnected(false), super_(super)
		{
			colors.push_back( Color(1.0, 0.0, 0.0 ));
			colors.push_back( Color(0.0, 1.0, 0.0 ));
			colors.push_back( Color(0.0, 0.0, 1.0 ));
			colors.push_back( Color(1.0, 1.0, 0.0 ));
			colors.push_back( Color(0.0, 1.0, 1.0 ));
			colors.push_back( Color(1.0, 0.0, 1.0 ));
		}
		unsigned geometry_oid_;
		std::vector<Color> colors;
		Db::Connection db;
		SQLConsole* console;
		LayersWidget* layers;
		bool isConnected;
		
		PSQLPlugin* super_;
	};
	
	///
	///
	///
	PSQLPlugin::PSQLPlugin() : impl_( new PSQLPluginImpl( this ) )
	{
		
	}
	
	
	PSQLPlugin::~PSQLPlugin()
	{
		delete impl_;
	}
	
	void PSQLPlugin::launchConsole()
	{
		impl_->console = new SQLConsole( this );
		viewerWindow()->addDockWidget( Qt::BottomDockWidgetArea, impl_->console );
		impl_->layers = new LayersWidget( this, impl_->console );
		viewerWindow()->addDockWidget( Qt::LeftDockWidgetArea, impl_->layers );
	}

	void PSQLPlugin::connect( const std::string& db_options )
	{
		if ( impl_->isConnected )
			return;

		try
		{
			impl_->db.connect( db_options );
		}
		catch ( std::exception& e ) {
			std::cerr << e.what() << std::endl;
			return;
		}

		//
		// get geometry type oid
		Db::Result r = impl_->db.exec("SELECT oid FROM pg_type WHERE typname='geometry'");
		if ( r.size() == 0 ) {
			std::cerr << "Not a PostGIS database" << std::endl;
			return;
		}
		impl_->geometry_oid_ = r[0][0].as<unsigned>();
		impl_->isConnected = true;
	}

	void PSQLPlugin::disconnect()
	{
		impl_->isConnected = false;
	}

	osg::Geode* PSQLPlugin::doQuery( const std::string& query )
	{
		Db::Result r;
		try {
			r = impl_->db.exec( query );
		}
		catch ( std::runtime_error& e ) {
			std::cerr << e.what() << std::endl;
			return 0;
		}
		
		io::OsgFactory factory;
		osg::ref_ptr<osg::Geode> geode( new osg::Geode );
		
		try {
			
			std::cout << "# of rows: " << r.size() << std::endl;
			for ( size_t i = 0; i < r.size(); ++i ) {
				for ( size_t j = 0; j < r.columns(); ++j ) {
					std::string geomWkt;
					if ( PQftype( r.result(), j ) == impl_->geometry_oid_ ) {
						geomWkt = wkbToWkt( r[i][j].as<std::string>(), impl_->db );
					} else {
						geomWkt = r[i][j].as<std::string>();
					}
					std::auto_ptr<Geometry> g  = (io::readWkt( geomWkt ));
					osg::Geometry* osgGeo = factory.createGeometry( *g );
					
					osg::Vec4Array* colors = new osg::Vec4Array;
					// Each column has a color
					const Color& c = impl_->colors[ j % impl_->colors.size() ];
					colors->push_back(osg::Vec4(c.r, c.g, c.b, 1.0f));
					osgGeo->setColorArray(colors);
					osgGeo->setColorBinding( osg::Geometry::BIND_OVERALL );
					
					geode->addDrawable( osgGeo );
				}
			}
			
		} catch (std::exception& e ) {
			std::cerr << e.what() << std::endl;
			return 0;
		}
		return geode.release();
	}

	void PSQLPlugin::computeGeometries()
	{
		// iterate through the layers
		QListWidget* listWidget = impl_->layers->widget();
		for ( int i = 0; i < listWidget->count(); ++i ) {
			QListWidgetItem* item = listWidget->item( i );
			LayerInfo* linfo = (LayerInfo *)item->data( Qt::UserRole ).value<void *>();

			disconnect();
			connect( linfo->dbOptions );
			linfo->geode = osg::ref_ptr<osg::Geode>( doQuery( linfo->query ) );
		}		
	}
	
	void PSQLPlugin::display()
	{
		if ( impl_->layers == 0 ) {
			return;
		}

		osg::Group* root = viewerWindow()->viewer()->getScene();
		// remove all children
		root->removeChildren( 0, root->getNumChildren() );

		// iterate through the layers
		QListWidget* listWidget = impl_->layers->widget();
		for ( int i = 0; i < listWidget->count(); ++i ) {
			QListWidgetItem* item = listWidget->item( i );
			LayerInfo* linfo = (LayerInfo *)item->data( Qt::UserRole ).value<void *>();
			bool visible = item->data( Qt::CheckStateRole).toBool();
			std::cout << "visible = " << visible << std::endl;
			if ( visible && linfo->geode != 0 ) {
				root->addChild( linfo->geode );
			}
		}
	}

	void PSQLPlugin::onRefresh()
	{
		display();
	}
///
///
///
QString PSQLPlugin::pluginName() const
{
	return "PSQLPlugin";
}

	void PSQLPlugin::saveLayers()
	{
		if ( impl_->layers == 0 ) {
			launchConsole();
		}

		viewerWindow()->viewer()->stopAnimation();
		QString filename = QFileDialog::getSaveFileName( NULL, QString("Choose a layers file to save to") );

		QFile file( filename );
		file.open( QIODevice::ReadWrite );
		QDataStream stream( &file );

		// iterate through the layers
		QListWidget* listWidget = impl_->layers->widget();

		// write the number of layers
		stream << listWidget->count();

		for ( int i = 0; i < listWidget->count(); ++i ) {
			QListWidgetItem* item = listWidget->item( i );
			QString name = item->text();
			LayerInfo* linfo = (LayerInfo *)item->data( Qt::UserRole ).value<void *>();
			bool visible = item->data( Qt::CheckStateRole).toBool();

			// write the layer's data
			stream << name << visible << QString::fromStdString(linfo->dbOptions) << QString::fromStdString(linfo->query);
		}

		file.flush();
		viewerWindow()->viewer()->startAnimation();
	}

	void PSQLPlugin::loadLayers()
	{
		if ( impl_->layers == 0 ) {
			launchConsole();
		}
		
		viewerWindow()->viewer()->stopAnimation();
		QString filename = QFileDialog::getOpenFileName( NULL, QString("Choose file to load layers from") );
		QFile file( filename );
		file.open( QIODevice::ReadOnly );
		QDataStream str( &file );
		
		// iterate through the layers
		QListWidget* listWidget = impl_->layers->widget();
		
		// clear the list
		listWidget->clear();
		
		// number of layers
		int count;
		str >> count;
		
		for ( int i = 0; i < count; ++i ) {
		        QString name, dbOptions, query;
			bool visible;
				
			// read the layer's data
			str >> name >> visible >> dbOptions >> query;
			impl_->layers->addLayer( name, visible, dbOptions, query );
		}
			      
		file.flush();

		//
		computeGeometries();
		display();

		impl_->console->disconnectItem();
		viewerWindow()->viewer()->startAnimation();
	}

void PSQLPlugin::load()
{
	QMenu * pluginMenu = viewerWindow()->menuBar()->addMenu("PostGIS") ;

	QAction * action = pluginMenu->addAction( QString("&SQL console") );
	QObject::connect( action, SIGNAL(triggered()), this, SLOT( launchConsole() ) );

	QAction * saveAction = pluginMenu->addAction( QString("&Save layers") );
	QObject::connect( saveAction, SIGNAL(triggered()), this, SLOT( saveLayers() ) );

	QAction * loadAction = pluginMenu->addAction( QString("&Load layers") );
	QObject::connect( loadAction, SIGNAL(triggered()), this, SLOT( loadLayers() ) );

	QAction * refreshAction = pluginMenu->addAction( QString("&Refresh display\tF5") );
	refreshAction->setShortcut( QKeySequence("F5") );
	QObject::connect( refreshAction, SIGNAL(triggered()), this, SLOT( onRefresh() ) );
}


}//plugins
}//viewer
}//SFCGAL



