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
#ifndef _SFCGAL_DETAIL_COLLECTPOINTSVISITOR_H_
#define _SFCGAL_DETAIL_COLLECTPOINTSVISITOR_H_

#include <vector>
#include <SFCGAL/Exception.h>

namespace SFCGAL {
namespace detail {

    /**
     * Visitor that extract vertices from a Geometry
     */
    template < typename K >
    class CollectPointsVisitor : public boost::static_visitor<> {
    public:
        CollectPointsVisitor(
            std::vector< Coordinate<K> > & points
        ):
            _points(points)
        {

        }

        void operator () ( const Point<K> & g ){
            _points.push_back(g.toPoint_3()) ;
        }

        void operator () ( const LineString<K> & g ){
            for ( int i = 0; i < g.size(); i++ ){
                (*this)(g[i]);
            }
        }

        void operator () ( const Triangle<K> & g ){
            for ( int i = 0; i < 3; i++ ){
                (*this)(g.vertex(i));
            }
        }

        void operator () ( const Polygon<K> & g ){
            for ( const LineString<K> & ring : g ){
                (*this)(ring);
            }
        }
        
        void operator () ( const Solid<K> & g ){
            for ( const PolyhedralSurface<K> & shell : g ){
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
                SFCGAL::apply_visitor(*this,geometry);
            }
        }

    private:
        std::vector< Coordinate<K> > & _points ;
    } ;

} // namespace detail
} // namespace SFCGAL

#endif
