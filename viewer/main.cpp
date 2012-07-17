#include <iostream>

#include <QtGui/QApplication>

#include <osgDB/ReadFile>

#include <SFCGAL/viewer/ViewerWidget.h>
#include <SFCGAL/viewer/ViewerWindow.h>

using namespace SFCGAL::viewer ;


int main(int argc, char** argv)
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,argv);

    QApplication app(argc, argv);
    ViewerWidget * viewer = ViewerWidget::createFromArguments( arguments );

    //osg::Node* cow = osgDB::readNodeFile("/home/eplu/3RDPARTY/OpenSceneGraph-Data-3.0.0/cow.osgt") ;
    //cow->setName("cow node");
    //viewer->scene()->addChild( cow  );



	ViewerWindow window( viewer );
	window.show();

	return app.exec();
}
