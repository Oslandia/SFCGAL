#ifndef _SFCGAL_IO_OSG_H_
#define _SFCGAL_IO_OSG_H_

#include <SFCGAL/config.h>

#ifndef SFCGAL_WITH_OSG
#error "SFCGAL is not build with OpenSceneGraph support (see SFCGAL_BUILD_OSG in cmake)"
#endif


namespace osg {
	class Geometry ;
}

namespace SFCGAL {
	class Geometry ;
}


namespace SFCGAL {
namespace io {

	/**
	 * @brief [helper] write an OSG geometry in a file. The format depends on the extension (http://www.openscenegraph.org/projects/osg/wiki/Support/UserGuides/Plugins)
	 *
	 * Note that osgviewer command line executable allows the visualization of the resulting files, screenshots and co
	 *
	 * @warning requires OpenSceneGraph
	 *
	 * @todo list of geometrie?
	 */
	void osgWriteFile( const Geometry & g, const std::string& filepath ) ;

} // namespace io
} // namespace SFCGAL



#endif
