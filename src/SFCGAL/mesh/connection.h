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

#ifndef _SFCGAL_MESH_CONNECTION_H_
#define _SFCGAL_MESH_CONNECTION_H_

#include <SFCGAL/mesh/detail/ConnectedComponentsExtractor.h>

namespace SFCGAL {
namespace mesh {

    /**
     * test if a surface is connected, the graph should be build beforehand
     * @ingroup detail
     */
    template < typename Mesh >
    bool isConnected( const Mesh& mesh ){
        detail::ConnectedComponentsExtractor<Mesh> extractor(mesh);
        return extractor.numConnectedComponents() < 2 ;
    }

    /**
     * test if a surface is closed, the graph should be build beforehand
     *
     * TODO split non connected Mesh to connected parts?
     *
     * @note the surface may not be connected, eg. two spheres will yield a true result
     * @ingroup detail
     */
    template < typename Mesh >
    bool isClosed( const Mesh& mesh ){
        return CGAL::is_closed(mesh);
    }

} // mesh
} // SFCGAL

#endif
