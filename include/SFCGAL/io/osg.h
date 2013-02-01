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
	 * @brief create an OSG geometry
	 * @warning requires OpenSceneGraph
	 * @return new osg::Geometry
	 */
	osg::Geometry * toOsgGeometry( const Geometry & g ) ;


} // namespace io
} // namespace SFCGAL



#endif
