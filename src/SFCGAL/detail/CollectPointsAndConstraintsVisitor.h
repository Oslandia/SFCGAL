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
#ifndef _SFCGAL_DETAIL_COLLECTPOINTSANDCONSTRAINTSVISITOR_H_
#define _SFCGAL_DETAIL_COLLECTPOINTSANDCONSTRAINTSVISITOR_H_

#include <vector>
#include <SFCGAL/Exception.h>

namespace SFCGAL {
namespace detail {

    /**
     * Visitor that extract vertices and edges from a Geometry
     */
    template < typename K >
    class CollectPointsAndConstraintsVisitor : public boost::static_visitor<> {
    public:
        CollectPointsAndConstraintsVisitor(
            std::vector< Point<K> > & points,
            std::vector< std::pair< size_t, size_t > > & constraints
        ):
            _points(points),
            _constraints(constraints)
        {

        }

        void operator () ( const Point<K> & g ){
            _points.push_back(g) ;
        }

        void operator () ( const LineString<K> & g ){
            size_t last = 0 ;
            for ( int i = 0; i < g.size(); i++ ){
                size_t index = _points.size() ;
                _points.push_back(g[i]);
                if ( i != 0 ){
                    _constraints.push_back(std::make_pair(last,index));
                }
                last = index ;
            }
        }

        void operator () ( const Triangle<K> & g ){
            size_t last = 0 ;
            for ( int i = 0; i < 3; i++ ){
                size_t index = _points.size() ;
                _points.push_back(g.vertex(i));
                if ( i != 0 ){
                    _constraints.push_back(std::make_pair(last,index));
                }
                last = index ;
            }
        }

        void operator () ( const Polygon<K> & g ){
            for ( const LineString<K> & ring : g ){
                (*this)(ring);
            }
        }
        
        void operator () ( const Solid<K> & g ){
            for ( const MultiPolygon<K> & shell : g ){
                (*this)(shell);
            }
        }

        template < typename GT >
        void operator () ( const Collection<GT> & geometries ){
            for ( const GT & geometry : geometries ){
                (*this)(geometry);
            }
        }
        
        void operator () ( const GeometryCollection<K> & geometries ){
            for ( const Geometry<K> & geometry : geometries ){
                boost::apply_visitor(*this,geometry);
            }
        }

    private:
        std::vector< Point<K> > & _points ;
        std::vector< std::pair< size_t, size_t > > & _constraints ;
    } ;

} // namespace detail
} // namespace SFCGAL

#endif
