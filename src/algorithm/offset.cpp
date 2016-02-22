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

#include <SFCGAL/algorithm/offset.h>

#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/MultiPolygon.h>

#include <SFCGAL/Exception.h>

#include <SFCGAL/detail/polygonSetToMultiPolygon.h>
#include <SFCGAL/algorithm/isValid.h>


#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>

#include <CGAL/minkowski_sum_2.h>
#include <CGAL/approximated_offset_2.h>
#include <CGAL/offset_polygon_2.h>

typedef CGAL::Polygon_2< SFCGAL::Kernel >            Polygon_2 ;
typedef CGAL::Polygon_with_holes_2< SFCGAL::Kernel > Polygon_with_holes_2 ;
typedef CGAL::Polygon_set_2< SFCGAL::Kernel >        Polygon_set_2 ;

typedef CGAL::Gps_circle_segment_traits_2< SFCGAL::Kernel >    Gps_traits_2;
typedef Gps_traits_2::Curve_2                                  Offset_curve_2 ;
typedef Gps_traits_2::X_monotone_curve_2                       Offset_x_monotone_curve_2 ;
typedef Gps_traits_2::Polygon_2                                Offset_polygon_2;
typedef Gps_traits_2::Polygon_with_holes_2                     Offset_polygon_with_holes_2;
typedef CGAL::General_polygon_set_2< Gps_traits_2 >            Offset_polygon_set_2 ;

#define SFCGAL_OFFSET_ACCURACY 0.0001

#define SFCGAL_OFFSET_ASSERT_FINITE_RADIUS( r ) \
    if ( !std::isfinite(r) ) BOOST_THROW_EXCEPTION( NonFiniteValueException("radius is non finite") );
namespace SFCGAL {
namespace algorithm {

//-- private interface

/**
 * @brief dispatch a geometry
 */
void offset( const Geometry& g, const double& radius, Offset_polygon_set_2& polygonSet ) ;

/**
 * @brief offset for a Point
 */
void offset( const Point& g, const double& radius, Offset_polygon_set_2& polygonSet ) ;
/**
 * @brief offset for a LineString
 */
void offset( const LineString& g, const double& radius, Offset_polygon_set_2& polygonSet ) ;
/**
 * @brief offset for a Polygon
 */
void offset( const Polygon& g, const double& radius, Offset_polygon_set_2& polygonSet ) ;
/**
 * @brief offset for MultiPoint, MultiLineString, MultiPolygon, TriangulatedSurface, PolyhedralSurface
 */
void offsetCollection( const Geometry& g, const double& radius, Offset_polygon_set_2& polygonSet ) ;


//-- helpers

/**
 * @brief approximate an Offset_polygon_2 (filter null segments)
 */
Polygon_2 approximate( const Offset_polygon_2& polygon, const int& n = 0 )
{
    std::list<std::pair<double, double> > pair_list;

    /*
     * iterate X_monotone_curve_2 components
     */
    for ( Offset_polygon_2::Curve_const_iterator it = polygon.curves_begin();
            it != polygon.curves_end(); ++it ) {
        it->approximate( std::back_inserter( pair_list ), n ) ;
    }

    // remove duplicated last point
    pair_list.pop_back() ;

    /*
     * convertr to polygon
     */
    Polygon_2 result ;

    bool isFirst = true ;
    Kernel::Point_2 last ;

    for ( std::list<std::pair<double, double> >::const_iterator it = pair_list.begin(); it != pair_list.end(); ++it ) {
        Kernel::Point_2 point( it->first, it->second ) ;

        if ( isFirst ) {
            isFirst = false ;
        }
        else if ( point == last ) {
            continue ;
        }

        result.push_back( point ) ;
        last = point ;
    }

    return result ;
}

/**
 * @brief approximate an Offset
 */
Polygon_with_holes_2 approximate( const Offset_polygon_with_holes_2& polygon, const int& n = 0 )
{
    Polygon_with_holes_2 result( approximate( polygon.outer_boundary(), n ) );

    for ( Offset_polygon_with_holes_2::Hole_const_iterator it = polygon.holes_begin(); it != polygon.holes_end(); ++it ) {
        result.add_hole( approximate( *it, n ) );
    }

    return result ;
}


/**
 * @brief convert Offset_polygon_set_2 to MultiPolygon
 */
std::auto_ptr< MultiPolygon > polygonSetToMultiPolygon( const Offset_polygon_set_2& polygonSet, const int& n )
{
    std::list<Offset_polygon_with_holes_2> res;
    polygonSet.polygons_with_holes( std::back_inserter( res ) ) ;

    std::auto_ptr< MultiPolygon > result( new MultiPolygon );

    for ( std::list<Offset_polygon_with_holes_2>::const_iterator it = res.begin(); it != res.end(); ++it ) {
        result->addGeometry( new Polygon( approximate( *it, n ) ) );
    }

    return result ;
}


/**
 * @brief helper to create a polygon from a circle
 */
Offset_polygon_2 circleToPolygon( const Kernel::Circle_2& circle )
{
    /*
     * convert the circle into Offset_x_monotone_curve_2 (exactly 2)
     */
    Gps_traits_2 traits;
    Offset_curve_2 curve( circle );

    std::list<CGAL::Object> parts;
    traits.make_x_monotone_2_object()( curve, std::back_inserter( parts ) );
    BOOST_ASSERT( parts.size() == 2U );

    // Construct the polygon.
    Offset_polygon_2 result ;

    for ( std::list<CGAL::Object>::const_iterator it = parts.begin(); it != parts.end(); ++it ) {
        Offset_x_monotone_curve_2 arc;
        CGAL::assign( arc, *it );
        result.push_back( arc );
    }

    return result;
}

/**
 * @brief build Point offset
 */
void offset( const Point& gA, const double& radius, Offset_polygon_set_2& polygonSet )
{
    SFCGAL_OFFSET_ASSERT_FINITE_RADIUS( radius );
    Kernel::Circle_2 circle( gA.toPoint_2<Kernel>(), radius * radius );

    if ( polygonSet.is_empty() ) {
        polygonSet.insert( circleToPolygon( circle ) );
    }
    else {
        polygonSet.join( circleToPolygon( circle ) );
    }
}


/**
 * @brief build LineString offset
 */
void offset( const LineString& lineString, const double& radius, Offset_polygon_set_2& polygonSet )
{
    SFCGAL_OFFSET_ASSERT_FINITE_RADIUS( radius );

    for ( size_t i = 0; i < lineString.numSegments(); i++ ) {
        Polygon_2 P ;
        P.push_back( lineString.pointN( i ).toPoint_2<Kernel>() );
        P.push_back( lineString.pointN( i+1 ).toPoint_2<Kernel>() );
        Offset_polygon_with_holes_2  offset = CGAL::approximated_offset_2( P, radius, SFCGAL_OFFSET_ACCURACY ) ;

        if ( polygonSet.is_empty() ) {
            polygonSet.insert( offset );
        }
        else {
            polygonSet.join( offset );
        }
    }
}


///
///
///
void offset( const Polygon& g, const double& radius, Offset_polygon_set_2& polygonSet )
{
    SFCGAL_OFFSET_ASSERT_FINITE_RADIUS( radius );

    if ( g.isEmpty() ) {
        return ;
    }

    /*
     * Invoke minkowski_sum_2 for exterior ring
     */
    {
        Offset_polygon_with_holes_2  offset = CGAL::approximated_offset_2( g.exteriorRing().toPolygon_2(), radius, SFCGAL_OFFSET_ACCURACY ) ;

        if ( polygonSet.is_empty() ) {
            polygonSet.insert( offset );
        }
        else {
            polygonSet.join( offset );
        }
    }

    /*
     * Compute the Minkowski sum for each segment of the interior rings
     * and perform the union of the result. The result is a polygon, and its holes
     * correspond to the inset.
     *
     */
    if ( g.hasInteriorRings() ) {
        Offset_polygon_set_2 sumInteriorRings ;

        for ( size_t i = 0; i < g.numInteriorRings(); i++ ) {
            offset( g.interiorRingN( i ), radius, sumInteriorRings ) ;
        }

        /*
         * compute the difference for each hole of the resulting polygons
         */
        std::list< Offset_polygon_with_holes_2 > interiorPolygons ;
        sumInteriorRings.polygons_with_holes( std::back_inserter( interiorPolygons ) ) ;

        for ( std::list< Offset_polygon_with_holes_2 >::iterator it_p = interiorPolygons.begin();
                it_p != interiorPolygons.end(); ++it_p ) {

            for ( Offset_polygon_with_holes_2::Hole_iterator it_hole = it_p->holes_begin();
                    it_hole != it_p->holes_end(); ++it_hole ) {

                it_hole->reverse_orientation() ;
                polygonSet.difference( *it_hole ) ;
            } // foreach hole
        }// foreach polygon
    }
}

///
///
///
void offsetCollection( const Geometry& g, const double& radius, Offset_polygon_set_2& polygonSet )
{
    SFCGAL_OFFSET_ASSERT_FINITE_RADIUS( radius );

    for ( size_t i = 0; i < g.numGeometries(); i++ ) {
        offset( g.geometryN( i ), radius, polygonSet );
    }
}

///
///
///
void offset( const Geometry& g, const double& radius, Offset_polygon_set_2& polygonSet )
{
    SFCGAL_OFFSET_ASSERT_FINITE_RADIUS( radius );

    if ( g.isEmpty() ) {
        return ;
    }

    switch ( g.geometryTypeId() ) {
    case TYPE_POINT:
        return offset( g.as< Point >(), radius, polygonSet ) ;

    case TYPE_LINESTRING:
        return offset( g.as< LineString >(), radius, polygonSet ) ;

    case TYPE_POLYGON:
        return offset( g.as< Polygon >(), radius, polygonSet ) ;

    case TYPE_TRIANGLE:
        return offset( g.as< Triangle >().toPolygon(), radius, polygonSet ) ;

    case TYPE_SOLID:
        return offset( g.as< Solid >().exteriorShell(), radius, polygonSet ) ;

    case TYPE_MULTISOLID:
    case TYPE_MULTIPOINT:
    case TYPE_MULTILINESTRING:
    case TYPE_MULTIPOLYGON:
    case TYPE_GEOMETRYCOLLECTION:
    case TYPE_TRIANGULATEDSURFACE:
    case TYPE_POLYHEDRALSURFACE:
        return offsetCollection( g, radius, polygonSet );
    }
}

//-- public interface

///
///
///
std::auto_ptr< MultiPolygon > offset( const Geometry& g, const double& r, NoValidityCheck )
{
    SFCGAL_OFFSET_ASSERT_FINITE_RADIUS( r );
    Offset_polygon_set_2 polygonSet ;
    offset( g, r, polygonSet ) ;
    return polygonSetToMultiPolygon( polygonSet, 8 );
}

std::auto_ptr< MultiPolygon > offset( const Geometry& g, const double& r )
{
    SFCGAL_OFFSET_ASSERT_FINITE_RADIUS( r );
    SFCGAL_ASSERT_GEOMETRY_VALIDITY( g );
    return offset( g, r, NoValidityCheck() );
}

}//namespace algorithm
}//namespace SFCGAL

