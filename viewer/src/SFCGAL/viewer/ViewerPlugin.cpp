#include <SFCGAL/viewer/ViewerPlugin.h>

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
void ViewerPlugin::unload()
{

}


}//viewer
}//SFCGAL


