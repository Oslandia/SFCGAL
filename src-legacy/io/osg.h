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

#ifndef _SFCGAL_IO_OSG_H_
#define _SFCGAL_IO_OSG_H_

#include <SFCGAL/config.h>

#ifndef SFCGAL_WITH_OSG
#error "SFCGAL is not build with OpenSceneGraph support (see SFCGAL_BUILD_OSG in cmake)"
#endif

#include <string>

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
 * Note that osgviewer command line executable allows the visualization of the resulting files, screenshots and co
 * @warning requires OpenSceneGraph
 * @todo list of geometries?
 */
SFCGAL_API void osgWriteFile( const Geometry& g, const std::string& filepath ) ;

/**
 * @brief [helper] converts a SFCGAL::Geometry to an OSG geometry
 */
SFCGAL_API osg::Geometry* toOsgGeometry( const Geometry& g ) ;


} // namespace io
} // namespace SFCGAL



#endif
