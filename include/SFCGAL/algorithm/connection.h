/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _SFCGAL_GRAPH_ISCONNECTED_H_
#define _SFCGAL_GRAPH_ISCONNECTED_H_

#include <SFCGAL/all.h>
#include <map>

namespace SFCGAL {
namespace algorithm {


class PolyHedralSurfaceGraph {
public:
    PolyHedralSurfaceGraph( const PolyhedralSurface & s )
    {
        BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    }

    PolyHedralSurfaceGraph( const TriangulatedSurface & tin )
    {
        BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    }

    typedef size_t VertexIndex;
    typedef size_t EdgeIndex;
    typedef std::map< Coordinate, size_t >  coordinate_list ;
};

/**
 * test if a surface is connected, the graph should be build beforehand
 */
inline
bool isConnected( const PolyHedralSurfaceGraph graph )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}

/**
 * test if a surface is closed, the graph should be build beforehand
 */
inline
bool isClosed( const PolyHedralSurfaceGraph graph )
{
    BOOST_THROW_EXCEPTION(Exception("function is not implemented"));
    return true;
}

}
}
#endif
