#include <SFCGAL/viewer/ViewerWidget.h>

#include <iostream>
#include <memory>

#include <QtGui/QVBoxLayout>


#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>


#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/SphericalManipulator>


#include <osgUtil/Optimizer>
#include <osg/CoordinateSystemNode>

#include <osg/Switch>
#include <osgText/Text>



namespace SFCGAL {
namespace viewer {


///
///
///
ViewerWidget::ViewerWidget():
	QWidget(),
	osgViewer::Viewer()
{
	initViewer();
}

///
///
///
ViewerWidget::ViewerWidget(osg::ArgumentParser & arguments) :
	QWidget(),
	osgViewer::Viewer(arguments)
{
	initViewer();
}

///
///
///
void ViewerWidget::initViewer()
{
	_camera = createCamera(0,0,100,100) ;
	setCamera( _camera );

	_scene = new osg::Group();
	setSceneData(_scene);

	osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( _camera->getGraphicsContext() );

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget( gw ? gw->getGLWidget() : NULL );
	setLayout( layout );

	setMinimumSize(300,300);

	connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
	startAnimation();
}


///
///
///
osg::Group* ViewerWidget::getScene()
{
	return _scene ;
}

///
///
///
osgQt::GraphicsWindowQt* ViewerWidget::getGraphicsWindowQt()
{
	return dynamic_cast<osgQt::GraphicsWindowQt*>( _camera->getGraphicsContext() ) ;
}



///
///
///
osg::Camera* ViewerWidget::createCamera( int x, int y, int w, int h, const std::string& name, bool windowDecoration )
{
	osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->windowName = name;
	traits->windowDecoration = windowDecoration;
	traits->x = x;
	traits->y = y;
	traits->width = w;
	traits->height = h;
	traits->doubleBuffer = true;
	traits->alpha = ds->getMinimumNumAlphaBits();
	traits->stencil = ds->getMinimumNumStencilBits();
	traits->sampleBuffers = ds->getMultiSamples();
	traits->samples = ds->getNumMultiSamples();

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );

	camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
	camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
	camera->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
	return camera.release();
}

///
///
///
void ViewerWidget::paintEvent( QPaintEvent* event )
{
	frame();
}

///
///
///
ViewerWidget* ViewerWidget::createFromArguments( osg::ArgumentParser & arguments )
{
	arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
	arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" is the standard OpenSceneGraph example which loads and visualises 3d models.");
	arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] filename ...");
	arguments.getApplicationUsage()->addCommandLineOption("--image <filename>","Load an image and render it on a quad");
	arguments.getApplicationUsage()->addCommandLineOption("--dem <filename>","Load an image/DEM and render it on a HeightField");
	arguments.getApplicationUsage()->addCommandLineOption("--login <url> <username> <password>","Provide authentication information for http file access.");

	std::auto_ptr< ViewerWidget > viewer( new ViewerWidget( arguments ) );

	unsigned int helpType = 0;
	if ((helpType = arguments.readHelpType()))
	{
		arguments.getApplicationUsage()->write(std::cout, helpType);
		return NULL ;
	}

	// report any errors if they have occurred when parsing the program arguments.
	if (arguments.errors())
	{
		arguments.writeErrorMessages(std::cout);
		return NULL ;
	}


	std::string url, username, password;
	while(arguments.read("--login",url, username, password))
	{
		if (!osgDB::Registry::instance()->getAuthenticationMap())
		{
			osgDB::Registry::instance()->setAuthenticationMap(new osgDB::AuthenticationMap);
			osgDB::Registry::instance()->getAuthenticationMap()->addAuthenticationDetails(
				url,
				new osgDB::AuthenticationDetails(username, password)
			);
		}
	}

	// set up the camera manipulators.
	{
		osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

		keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
		keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
		keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
		keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );
		keyswitchManipulator->addMatrixManipulator( '5', "Orbit", new osgGA::OrbitManipulator() );
		keyswitchManipulator->addMatrixManipulator( '6', "FirstPerson", new osgGA::FirstPersonManipulator() );
		keyswitchManipulator->addMatrixManipulator( '7', "Spherical", new osgGA::SphericalManipulator() );

		std::string pathfile;
		double animationSpeed = 1.0;
		while(arguments.read("--speed",animationSpeed) ) {}
		char keyForAnimationPath = '8';
		while (arguments.read("-p",pathfile))
		{
			osgGA::AnimationPathManipulator* apm = new osgGA::AnimationPathManipulator(pathfile);
			if (apm || !apm->valid())
			{
				apm->setTimeScale(animationSpeed);

				unsigned int num = keyswitchManipulator->getNumMatrixManipulators();
				keyswitchManipulator->addMatrixManipulator( keyForAnimationPath, "Path", apm );
				keyswitchManipulator->selectMatrixManipulator(num);
				++keyForAnimationPath;
			}
		}

		viewer->setCameraManipulator( keyswitchManipulator.get() );
	}

	// add the state manipulator
	viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );

	// add the thread model handler
	viewer->addEventHandler(new osgViewer::ThreadingHandler);

	// add the window size toggle handler
	//viewer->addEventHandler(new osgViewer::WindowSizeHandler);

	// add the stats handler
	viewer->addEventHandler(new osgViewer::StatsHandler);

	// add the help handler
	viewer->addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));

	// add the record camera path handler
	viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);

	// add the LOD Scale handler
	viewer->addEventHandler(new osgViewer::LODScaleHandler);

	// add the screen capture handler
	viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);

	// load the data
	osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFiles(arguments);
	// optimize the scene graph, remove redundant nodes and state etc.
	if ( loadedModel.get() ){
		osgUtil::Optimizer optimizer;
		optimizer.optimize( loadedModel.get() ) ;
		viewer->getScene()->addChild( loadedModel.get() );
	}

	// any option left unread are converted into errors to write out later.
	arguments.reportRemainingOptionsAsUnrecognized();


	/// TODO remove
	viewer->setThreadingModel( osgViewer::Viewer::SingleThreaded ) ;
	//viewer->setThreadingModel( osgViewer::Viewer::ThreadPerCamera );
	//viewer->setThreadingModel( osgViewer::Viewer::DrawThreadPerContext ) ;

	return viewer.release() ;
}


///
///
///
void ViewerWidget::saveImageToFile()
{
	osgViewer::ScreenCaptureHandler::WriteToFile * writeToFileOperation = new osgViewer::ScreenCaptureHandler::WriteToFile(
		"screenshot",
		"png",
		osgViewer::ScreenCaptureHandler::WriteToFile::SEQUENTIAL_NUMBER
	);

	osgViewer::ScreenCaptureHandler * captureHandler = new osgViewer::ScreenCaptureHandler( writeToFileOperation );
	captureHandler->captureNextFrame( *this );
}

///
///
///
void ViewerWidget::startAnimation()
{
	_timer.start( 20 ) ;
}

///
///
///
void ViewerWidget::stopAnimation()
{
	_timer.stop();
}

}//viewer
}//SFCGAL
