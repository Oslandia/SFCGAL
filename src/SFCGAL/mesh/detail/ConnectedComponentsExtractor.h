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
#ifndef _SFCGAL_MESH_DETAIL_CONNECTEDCOMPONENTSEXTRACTOR_H_
#define _SFCGAL_MESH_DETAIL_CONNECTEDCOMPONENTSEXTRACTOR_H_

#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <boost/foreach.hpp>

namespace SFCGAL {
namespace mesh {
namespace detail {
    
    /**
     * Count and extract connected components from a mesh
     */
    template < typename Mesh >
    class ConnectedComponentsExtractor {
    public:
        typedef typename boost::graph_traits<Mesh>::face_descriptor  face_descriptor;
        typedef typename Mesh::template Property_map<face_descriptor, std::size_t> fccmap_t;

        ConnectedComponentsExtractor( const Mesh & mesh ):
            _mesh(mesh)
        {
            //TODO external property_map?
            _fccmap = const_cast<Mesh &>(_mesh).template add_property_map<face_descriptor, std::size_t>("f:CC").first;
            _numConnectedComponents = CGAL::Polygon_mesh_processing::connected_components(_mesh,_fccmap);
        }

        /**
         * Get the number of connected components
         */
        std::size_t numConnectedComponents() const {
            return _numConnectedComponents ;
        }
        
        /**
         * Extract triangles for the given connected component id
         */
        template < typename OutputIterator >
        void extractTriangles( const std::size_t & idComponent, OutputIterator out ){
            BOOST_FOREACH(face_descriptor f , faces(_mesh)){
                if ( _fccmap[f] != idComponent ){
                    continue;
                }
                *out = extractTriangle(_mesh,f) ;
                ++out;
            }
        }

    private:
        const Mesh & _mesh ;
        fccmap_t _fccmap ;
        std::size_t _numConnectedComponents;
    };


} // detail
} // mesh
} // SFCGAL


#endif

