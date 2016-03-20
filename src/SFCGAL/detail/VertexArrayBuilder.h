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
#ifndef _SFCGAL_DETAIL_VERTEXARRAYBUILDER_H_
#define _SFCGAL_DETAIL_VERTEXARRAYBUILDER_H_

#include <vector>
#include <map>

namespace SFCGAL {
namespace detail {

    /**
     * VertexArrayBuilder is an helper to index Geometry
     *  coordinates. It mainly focus the use of the CGAL's 
     *  Polygon Mesh Processing package.
     *
     * template parameter CT is either a CGAL::Point_2<K> or a CGAL::Point_3<K>
     */
    template < typename CT >
    class VertexArrayBuilder {
    public:
        VertexArrayBuilder( std::vector< CT > & vertices ):
            _vertices(vertices)
        {
            for ( size_t i = 0; i < vertices.size(); i++ ){
                _vertexMap.insert(std::make_pair(_vertices[i],i));
            }
        }

        /**
         * Add a point
         * @return the vertex index
         */
        size_t addVertex( const CT & coordinate ){
            auto insertResult = _vertexMap.insert(
                std::make_pair(coordinate,_vertices.size())
            );
            if ( insertResult.second ){
                _vertices.push_back(coordinate);
            }
            return insertResult.first->second;
        }

    private:
        std::vector< CT > & _vertices ;
        std::map< CT, size_t > _vertexMap ;
    } ;


} // namespace detail
} // namespace SFCGAL


#endif

