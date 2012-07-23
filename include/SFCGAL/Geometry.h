#ifndef _SFCGAL_GEOMETRY_H_
#define _SFCGAL_GEOMETRY_H_

#include <memory>
#include <string>
#include <sstream>

#include <boost/assert.hpp>


namespace SFCGAL {

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


    class GeometryVisitor ;
    class ConstGeometryVisitor ;
}

namespace SFCGAL {

    /**
     * [OGC/SFA]8.2.3 "A common list of codes for geometric types"
     *
     * @todo solid and triangles as non OGC/SFA geometric types?
     * @warning codes for abstract classes and unimplement classes are hidden
     */
    typedef enum {
//     TYPE_GEOMETRY            = 0, //abstract
       TYPE_POINT               = 1,
       TYPE_LINESTRING          = 2,
       TYPE_POLYGON             = 3,
       TYPE_MULTIPOINT          = 4,
       TYPE_MULTILINESTRING     = 5,
       TYPE_MULTIPOLYGON        = 6,
       TYPE_GEOMETRYCOLLECTION  = 7,
//     TYPE_CIRCULARSTRING      = 8,
//     TYPE_COMPOUNDCURVE       = 9,
//     TYPE_CURVEPOLYGON        = 10,
//     TYPE_MULTICURVE          = 11, //abstract
//     TYPE_MULTISURFACE        = 12, //abstract
//     TYPE_CURVE               = 13, //abstract
//     TYPE_SURFACE             = 14, //abstract
       TYPE_POLYHEDRALSURFACE   = 15,
       TYPE_TIN                 = 16,

       //-- not official codes
       TYPE_TRIANGLE            = 100, //17 in Wikipedia???
       TYPE_SOLID               = 101,
       TYPE_MULTISOLID          = 102
    } GeometryType ;


    /**
     * Declare coordinate types
     * @see SFA 2.8.3 LineStringZ = 1003 ( coordinateType + geometryType)
     */
    typedef enum {
    	COORDINATE_XY   = 0 ,
    	COORDINATE_XYZ  = 1000,
    	COORDINATE_XYM  = 2000,
    	COORDINATE_XYZM = 3000
    } CoordinateType ;

    /**
     * OGC/SFA base geometry class
     *
     * @todo template with CGAL kernel as parameter?
     * @todo store SRID?
     * @todo store coordinate type?
     * @todo store bbox?
     */
    class Geometry {
    public:
       virtual ~Geometry();

       /**
        * returns a deep copy of the geometry
        */
       virtual Geometry *  clone() const = 0 ;

       /**
        * [OGC/SFA]returns the geometry type
        * @warning use CamelCase (LineString, not LINESTRING)
        */
       virtual std::string  geometryType() const = 0 ;
       /**
        * Returns a code corresponding to the type
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
        * @warning suppose no mix of 2D/3D coordinates
        */
       virtual int          coordinateDimension() const = 0 ;
       /**
        * [OGC/SFA]test if geometry is empty
        */
       virtual bool         isEmpty() const = 0 ;

       /**
        * [OGC/SFA]test if geometry is 3d
        * @warning suppose no mix of 2D/3D coordinates
        */
       virtual bool         is3D() const = 0 ;


       /**
        * [OGC/SFA]Indicate if the geometry is simple (~no self-intersections)
        */
       //virtual bool         isSimple() const = 0 ;
       /**
        * [OGC/SFA]Indicate if the geometry is measured
        */
       //virtual bool isMeasured() const = 0 ;


       /**
        * [OGC/SFA]returns the WKT string
        * @numDecimals extension specify fix precision output
        */
       std::string          asText( const int & numDecimals = -1 ) const ;

       /**
        * [OGC/SFA]Returns a polygon representing the BBOX of the geometry
        * @todo In order to adapt to 3D, would be better to define an "Envelope type",
        * otherway would lead to Polygon and PolyhedralSurface
        */
       //std::auto_ptr< Geometry > envelope() const = 0 ;

       /**
        * [OGC/SFA]Return the boundary of the geometry
        */
       virtual Geometry*    boundary() const ;

       /**
        * [OGC/SFA]Returns the identifier of the spatial reference
        * system (-1 if undefined)
        * @warning not defined here
        */
       //inline int SRID() const ;


       /**
        * Test if geometry is of "Derived" type given as template parameter
        * @warning not optimized (slow with dynamic_cast)
        */
       template < typename Derived >
       inline bool is() const
       {
    	   return dynamic_cast< Derived const * >(this) != NULL ;
       }


       /**
        * Downcast helper
        */
       template < typename Derived >
       inline const Derived &  as() const {
            BOOST_ASSERT( is< Derived >() );
            return *static_cast< Derived const * >( this );
       }
       /**
        * Downcast helper
        */
       template < typename Derived >
       inline Derived &        as() {
            BOOST_ASSERT( is< Derived >() );
            return *static_cast< Derived * >( this );
       }


       /**
        * [visitor]dispatch visitor
        */
       virtual void accept( GeometryVisitor & visitor ) = 0 ;
       /**
        * [visitor]dispatch visitor
        */
       virtual void accept( ConstGeometryVisitor & visitor ) const = 0 ;


    protected:
       Geometry();
       Geometry( Geometry const& other );
    };

}

#endif
