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
#ifndef _SFCGAL_GEOMETRY_H_
#define _SFCGAL_GEOMETRY_H_

#include <SFCGAL/config.h>

#include <boost/shared_ptr.hpp>

#include <memory>
#include <string>
#include <sstream>

#include <boost/assert.hpp>

namespace CGAL {
    class Object;
}

namespace SFCGAL {
	class Geometry ;
    class Point ;
    class LineString ;
    class Polygon ;
    class GeometryCollection ;
    class MultiPoint ;
    class MultiLineString ;
    class MultiPolygon ;

    class Triangle ;
    class TriangulatedSurface ;
    class PolyhedralSurface ;

    //not SFA, appears in GML/CityGML
    class Solid ;
    //not SFA, appears in GML/CityGML
    class MultiSolid ;
    //not SFA, appears in GML/CityGML
    class Grid ;

    class Envelope ;

    class GeometryVisitor ;
    class ConstGeometryVisitor ;
}

namespace SFCGAL {

    /**
     * [OGC/SFA]8.2.3 "A common list of codes for geometric types"
     *
     * @todo solid and triangles as non OGC/SFA geometric types?
     * @warning codes for abstract classes and unimplemented classes are hidden
     * @warning code values have are important for WKB
     *
     * @ingroup public_api
     */
    enum GeometryType {
 //      TYPE_GEOMETRY            = 0, //abstract
       TYPE_POINT               = 1,
       TYPE_LINESTRING          = 2,
       TYPE_POLYGON             = 3,
       TYPE_MULTIPOINT          = 4,
       TYPE_MULTILINESTRING     = 5,
       TYPE_MULTIPOLYGON        = 6,
       TYPE_GEOMETRYCOLLECTION  = 7,
//     TYPE_CIRCULARSTRING      = 8, // not yet supported
//     TYPE_COMPOUNDCURVE       = 9, // not yet supported
//     TYPE_CURVEPOLYGON        = 10, // not yet supported
//     TYPE_MULTICURVE          = 11, //abstract
//     TYPE_MULTISURFACE        = 12, //abstract
//     TYPE_CURVE               = 13, //abstract
//     TYPE_SURFACE             = 14, //abstract
       TYPE_POLYHEDRALSURFACE   = 15,
       TYPE_TRIANGULATEDSURFACE = 16,

       //-- not official codes
       TYPE_TRIANGLE            = 100, //17 in Wikipedia???
       TYPE_SOLID               = 101,
       TYPE_MULTISOLID          = 102
    };


    /**
     * @brief coordinate types (XY, XYZ, XYM, etc.)
     * @see SFA 2.8.3 LineStringZ = 1003 ( coordinateType + geometryType)
     * @ingroup public_api
     */
    typedef enum {
    	COORDINATE_XY   = 0 ,
    	COORDINATE_XYZ  = 1000,
    	COORDINATE_XYM  = 2000,
    	COORDINATE_XYZM = 3000
    } CoordinateType ;

    /**
     * @brief OGC/SFA based Geometry abstract class
     *
     * @todo store bbox?
     *
     * @ingroup public_api
     */
    class SFCGAL_API Geometry {
    public:
       virtual ~Geometry();

       /**
        * @brief Get a deep copy of the geometry
        */
       virtual Geometry *  clone() const = 0 ;

       /**
        * @brief [OGC/SFA]returns the geometry type
        * @warning use CamelCase (LineString, not LINESTRING)
        */
       virtual std::string  geometryType() const = 0 ;
       /**
        * @brief Returns a code corresponding to the type
        * @warning not standard
        */
       virtual GeometryType geometryTypeId() const = 0 ;

       /**
        * [OGC/SFA]Dimension of the Geometry ( 0 : punctual, 1 : curve, ...)
        * @warning empty geometries provide the dimension corresponding to the object
        */
       virtual int          dimension() const = 0 ;
       /**
        * [OGC/SFA]returns the dimension of the coordinates
        * @pre suppose no mix of 2D/3D coordinates
        */
       virtual int          coordinateDimension() const = 0 ;
       /**
        * [OGC/SFA]test if geometry is empty
        */
       virtual bool         isEmpty() const = 0 ;

       /**
        * [OGC/SFA]test if geometry is 3d
        * @pre suppose no mix of 2D/3D coordinates
        */
       virtual bool         is3D() const = 0 ;
       /**
        * [OGC/SFA]test if geometry is measured (has an m)
        * @pre suppose no mix of M/!M points
        */
       virtual bool         isMeasured() const = 0 ;

       //virtual bool         isSimple() const = 0 ;

       /**
        * [OGC/SFA]returns the WKT string
        * @param numDecimals extension specify fix precision output
        */
       std::string          asText( const int & numDecimals = -1 ) const ;

       /**
        * [OGC/SFA]Returns a polygon representing the BBOX of the geometry
        * @todo In order to adapt to 3D, would be better to define an "Envelope type",
        * otherway would lead to Polygon and PolyhedralSurface
        */
       //std::auto_ptr< Geometry > envelope() const = 0 ;
       Envelope              envelope() const ;

       /**
        * @brief [OGC/SFA]Returns the boundary of the geometry
        */
       virtual std::auto_ptr< Geometry > boundary() const ;

       /**
        * @brief Computes the distance to an other geometry
        */
       double distance( const Geometry & other ) const ;
       /**
        * @brief Computes the 3D distance to an other geometry
        */
       double distance3D( const Geometry & other ) const ;


       //-- helpers

       /**
        * @brief round the geometry with a corresponding scale factor
        * @param scale the scale factor (1 corresponds to the nearest integer, 1000 to a 0.001 tolerance)
        */
       void round( const long & scale = 1 ) ;


       /**
        * @brief [OGC/SFA]Gets the number of geometries in a collection of geometries
        * @warning 1 for Point, LineString, Polygon, Triangle
        */
        virtual size_t             numGeometries() const ;
       /**
        * @brief [OGC/SFA]Returns the n-th geometry
        * @warning *this for Point, LineString, Polygon, Triangle
        */
       virtual const Geometry  &  geometryN( size_t const& n ) const ;
       /**
        * @brief [OGC/SFA]Returns the n-th geometry
        * @warning *this for Point, LineString, Polygon, Triangle
        */
       virtual Geometry &          geometryN( size_t const& n ) ;


       /**
        * @brief Tests if geometry is of "Derived" type given as template parameter
        * @warning not optimized (slow with dynamic_cast)
        */
       template < typename Derived >
       inline bool is() const
       {
    	   return dynamic_cast< Derived const * >(this) != NULL ;
       }


       /**
        * @brief Downcast to a "Derived" class
        * @warning performs check if boost assertions are enabled
	* @pre The cast must be doable
        */
       template < typename Derived >
       inline const Derived &  as() const {
            BOOST_ASSERT( is< Derived >() );
            return *static_cast< Derived const * >( this );
       }
       /**
        * @brief Downcast to a "Derived" class
        * @warning performs check if boost assertions are enabled
	* @pre The cast must be doable
        */
       template < typename Derived >
       inline Derived &        as() {
            BOOST_ASSERT( is< Derived >() );
            return *static_cast< Derived * >( this );
       }

       /**
        * @brief [visitor]dispatch visitor
	* @ingroup detail
        */
       virtual void accept( GeometryVisitor & visitor ) = 0 ;
       /**
        * @brief [visitor]dispatch visitor
	* @ingroup detail
        */
       virtual void accept( ConstGeometryVisitor & visitor ) const = 0 ;

       /**
        * Serializer
	* @ingroup detail
        */
       template <class Archive>
       void serialize( Archive& ar, const unsigned int version )
       {
       }
    protected:
       Geometry();
       Geometry( Geometry const& other );
    };

    /**
     * Equality operator
     * @todo only compare coordinate points
     * @pre the two geometries must be valid
     */
    SFCGAL_API bool operator==( const Geometry&, const Geometry& );

} // namespace SFCGAL

#endif
