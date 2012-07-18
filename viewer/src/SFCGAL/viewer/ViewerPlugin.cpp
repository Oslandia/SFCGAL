#include <SFCGAL/viewer/ViewerPlugin.h>

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


}//viewer
}//SFCGAL


