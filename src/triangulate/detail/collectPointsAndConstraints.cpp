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

#include <SFCGAL/triangulate/detail/collectPointsAndConstraints.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/Exception.h>

#include <boost/format.hpp>

namespace SFCGAL {
namespace triangulate {
namespace detail {


size_t appendPoint(
    const Point & g,
    std::vector< Kernel::Point_3 > & points
){
    size_t id = points.size() ;
    points.push_back( g.toPoint_3<Kernel>() );
    return id ;
}


void collectPointsAndConstraintsFromLineString(
    const LineString & g,
    std::vector< Kernel::Point_3 > & points,
    std::vector< std::pair< std::size_t, std::size_t > > & constraints
){
    size_t last = 0 ;

    for ( size_t i = 0; i < g.numPoints(); i++ ) {
        size_t vertex = appendPoint( g.pointN( i ), points );

        if ( i != 0 ) {
            constraints.push_back(std::make_pair(last, vertex));
        }

        last = vertex ;
    }
}



void collectPointsAndConstraintsFromTriangle( 
    const Triangle& g,
    std::vector< Kernel::Point_3 > & points,
    std::vector< std::pair< std::size_t, std::size_t > > & constraints    
)
{
    size_t last = 0 ;

    for ( size_t i = 0; i < 4; i++ ) {
        size_t vertex = appendPoint( g.vertex( i ), points );
        
        if ( i != 0 ) {
            constraints.push_back(std::make_pair(last, vertex));
        }

        last = vertex ;
    }
}


void collectPointsAndConstraintsFromPolygon(
    const Polygon & g,
    std::vector< Kernel::Point_3 > & points,
    std::vector< std::pair< std::size_t, std::size_t > > & constraints
){
    for ( size_t i = 0; i < g.numRings(); i++ ) {
        collectPointsAndConstraintsFromLineString(g.ringN(i),points,constraints);
    }
}


void collectPointsAndConstraintsFromCollection(
    const GeometryCollection & g,
    std::vector< Kernel::Point_3 > & points,
    std::vector< std::pair< std::size_t, std::size_t > > & constraints
){
    for ( GeometryCollection::const_iterator it = g.begin(); it != g.end(); ++it ){
        // traverse
        collectPointsAndConstraints(*it,points,constraints);
    }
}



///
///
///
void collectPointsAndConstraints( 
    const Geometry & g,
    std::vector< Kernel::Point_3 > & points,
    std::vector< std::pair< std::size_t, std::size_t > > & constraints
)
{
    if ( g.isEmpty() ) {
        return;
    }

    switch ( g.geometryTypeId() ) {
    case TYPE_POINT:
        appendPoint( g.as< Point >(), points );
        return ;

    case TYPE_LINESTRING:
        collectPointsAndConstraintsFromLineString( 
            g.as< LineString >(), 
            points,
            constraints
        );
        return ;

    case TYPE_POLYGON:
        collectPointsAndConstraintsFromPolygon( 
            g.as< Polygon >(), 
            points,
            constraints
        );
        return ;

    case TYPE_TRIANGLE:
        collectPointsAndConstraintsFromTriangle( 
            g.as< Triangle >(), 
            points,
            constraints
        );
        return ;

    case TYPE_MULTIPOINT:
    case TYPE_MULTILINESTRING:
    case TYPE_MULTIPOLYGON:
    case TYPE_POLYHEDRALSURFACE:
    case TYPE_TRIANGULATEDSURFACE:
    case TYPE_GEOMETRYCOLLECTION:
        collectPointsAndConstraintsFromCollection( 
            g.as< GeometryCollection >(), 
            points,
            constraints
        );
        return ;

    case TYPE_SOLID:
    case TYPE_MULTISOLID:
        // note: we can't have à valid geom in 2D that comes from à solid
        // since a solid closed and thus self-intersects once projected
        BOOST_THROW_EXCEPTION(
            InappropriateGeometryException(
                ( boost::format( "can't process 2DZ triangulation for type '%1%'" ) % g.geometryType() ).str()
            )
        );
    }
}

    
} // triangulate
} // triangulate
} // SFCGAL
