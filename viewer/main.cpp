#include <iostream>

#include <QtGui/QApplication>

#include <osgDB/ReadFile>
#include <osgUtil/SmoothingVisitor>

#include <osg/Material>
#include <osgDB/Registry>

#include <SFCGAL/all.h>
#include <SFCGAL/tools/Log.h>

#include <SFCGAL/viewer/ViewerWidget.h>
#include <SFCGAL/viewer/ViewerWindow.h>
#include <SFCGAL/viewer/plugins/DemoPlugin.h>
#include <SFCGAL/viewer/plugins/DataPlugin.h>

#include <QtGui/QFileDialog>



using namespace SFCGAL ;
using namespace SFCGAL::viewer ;


int main(int argc, char** argv)
{
	Logger::get()->setLogLevel( Logger::Debug );

    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,argv);

    QApplication app(argc, argv);

    ViewerWidget * viewer = ViewerWidget::createFromArguments( arguments );
//    osg::Geode* geode = new osg::Geode;
//    //osg::StateSet* status = geode->getOrCreateStateSet();
//    //status->setMode(GL_LIGHTING, osg::StateAttribute::ON);
//
//
//	std::auto_ptr< Geometry > g( io::readWkt("POLYGON((0.0 0.0 0.0,1.0 0.0 0.0,1.0 1.0 0.0,0.0 1.0 0.0,0.0 0.0 0.0),(0.2 0.2 0.0,0.8 0.2 0.0,0.8 0.8 0.0,0.2 0.8 0.0,0.2 0.2 0.0))")  );
//	g->as< Polygon >().ringN(1).reverse();
//	std::auto_ptr< Geometry > ext( algorithm::extrude(*g,0.0,0.0,1.0) );
//
//	io::OsgFactory factory ;
//
//	osg::Geometry * osgGeometry = factory.createGeometry( *ext ) ;
//

// */
//
//	/*
//	 * provide a material
//	 */
//	osg::ref_ptr<osg::StateSet> stateSet( geode->getOrCreateStateSet() );
//	osg::ref_ptr<osg::Material> material( new osg::Material );
//	material->setDiffuse(osg::Material::FRONT,osg::Vec4(1.0f,0.0f,0.0f,1.0f));
//	stateSet->setAttribute( material.release() );
//
//
//	geode->addDrawable( osgGeometry );
//	viewer->getScene()->addChild( geode ) ;


	ViewerWindow window( viewer );
	window.addPlugin( new plugins::DataPlugin );
	window.addPlugin( new plugins::DemoPlugin );
	window.show();

	return app.exec();
}
