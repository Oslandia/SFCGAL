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
#include <iostream>

#include <QtGui/QApplication>

#include <osgDB/ReadFile>
#include <osgUtil/SmoothingVisitor>

#include <osg/Material>
#include <osgDB/Registry>

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>
#include <SFCGAL/detail/tools/Log.h>

#include <SFCGAL/viewer/ViewerWidget.h>
#include <SFCGAL/viewer/ViewerWindow.h>
#include <SFCGAL/viewer/plugins/DemoPlugin.h>
#include <SFCGAL/viewer/plugins/DataPlugin.h>
#include <SFCGAL/viewer/plugins/GridPlugin.h>
#include <SFCGAL/viewer/plugins/PSQLPlugin.h>
#include <QtGui/QFileDialog>



using namespace SFCGAL ;
using namespace SFCGAL::viewer ;


int main( int argc, char** argv )
{
    Logger::get()->setLogLevel( Logger::Debug );

    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments( &argc,argv );

    QApplication app( argc, argv );

    ViewerWidget* viewer = ViewerWidget::createFromArguments( arguments );
//    osg::Geode* geode = new osg::Geode;
//    //osg::StateSet* status = geode->getOrCreateStateSet();
//    //status->setMode(GL_LIGHTING, osg::StateAttribute::ON);
//
//
//	std::auto_ptr< Geometry > g( io::readWkt("POLYGON((0.0 0.0 0.0,1.0 0.0 0.0,1.0 1.0 0.0,0.0 1.0 0.0,0.0 0.0 0.0),(0.2 0.2 0.0,0.8 0.2 0.0,0.8 0.8 0.0,0.2 0.8 0.0,0.2 0.2 0.0))")  );
//	g->as< Polygon >().ringN(1).reverse();
//	std::auto_ptr< Geometry > ext( algorithm::extrude(*g,0.0,0.0,1.0) );
//
//	detail::io::OsgFactory factory ;
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
    window.addPlugin( new plugins::DataPlugin ) ;
    window.addPlugin( new plugins::DemoPlugin ) ;
    window.addPlugin( new plugins::GridPlugin ) ;
    window.addPlugin( new plugins::PSQLPlugin ) ;
    window.show();

    return app.exec();
}
