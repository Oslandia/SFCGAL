#include <SFCGAL/viewer/ViewerPlugin.h>
#include <SFCGAL/viewer/ViewerWidget.h>
#include <SFCGAL/viewer/ViewerWindow.h>

#include <iostream>

namespace SFCGAL {
namespace viewer {

///
///
///
ViewerPlugin::ViewerPlugin():
	_viewerWindow(0)
{

}


///
///
///
ViewerWindow * ViewerPlugin::viewerWindow()
{
	return _viewerWindow ;
}

///
///
///
ViewerWidget * ViewerPlugin::viewer()
{
	return _viewerWindow->viewer() ;
}

///
///
///
void ViewerPlugin::unload()
{

}


}//viewer
}//SFCGAL


