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
#ifndef _SFCGAL_LINESTRING_H_
#define _SFCGAL_LINESTRING_H_

#include <SFCGAL/kernels.h>
#include <SFCGAL/Point.h>

namespace SFCGAL {
    
    /**
     * A polyline defined by a list of points
     */
    template < typename K >
    using LineString = std::vector< Point<K> > ;
    
    template < typename K >
    const Point<K> & startPoint( const LineString<K> & g ){
        BOOST_ASSERT( ! g.empty() );
        return g.front();
    }

    template < typename K >
    const Point<K> & endPoint( const LineString<K> & g ){
        BOOST_ASSERT( ! g.empty() );    
        return g.back();
    }

} // SFCGAL

#endif
