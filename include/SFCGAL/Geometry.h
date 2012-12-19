#ifndef _SFCGAL_GEOMETRY_H_
#define _SFCGAL_GEOMETRY_H_

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
	typedef boost::shared_ptr< Geometry >         GeometryPtr ;
	typedef boost::shared_ptr< const Geometry >   ConstGeometryPtr ;

    class Point ;
	typedef boost::shared_ptr< Point >            PointPtr ;
	typedef boost::shared_ptr< const Point >      ConstPointPtr ;

    class LineString ;
	typedef boost::shared_ptr< LineString >       LineStringPtr ;
	typedef boost::shared_ptr< const LineString > ConstLineStringPtr ;

    class Polygon ;
	typedef boost::shared_ptr< Polygon >          PolygonPtr ;
	typedef boost::shared_ptr< const Polygon >    ConstPolygonPtr ;

    class GeometryCollection ;
	typedef boost::shared_ptr< GeometryCollection >        GeometryCollectionPtr ;
	typedef boost::shared_ptr< const GeometryCollection >  ConstGeometryCollectionPtr ;

    class MultiPoint ;
    typedef boost::shared_ptr< MultiPoint >             MultiPointPtr ;
	typedef boost::shared_ptr< const MultiPoint >       ConstMultiPointPtr ;

    class MultiLineString ;
    typedef boost::shared_ptr< MultiLineString >        MultiLineStringPtr ;
	typedef boost::shared_ptr< const MultiLineString >  ConstMultiLineString ;

    class MultiPolygon ;
    typedef boost::shared_ptr< MultiPolygon >           MultiPolygonPtr ;
	typedef boost::shared_ptr< const MultiPolygon >     ConstMultiPolygon ;


    class Triangle ;
    typedef boost::shared_ptr< Triangle >              TrianglePtr ;
	typedef boost::shared_ptr< const Triangle >        ConstTrianglePtr ;

    class TriangulatedSurface ;
    typedef boost::shared_ptr< TriangulatedSurface >            TriangulatedSurfacePtr ;
	typedef boost::shared_ptr< const TriangulatedSurface >      ConstTriangulatedSurfacePtr ;

    class PolyhedralSurface ;
    typedef boost::shared_ptr< PolyhedralSurface >              PolyhedralSurfacePtr ;
	typedef boost::shared_ptr< const PolyhedralSurface >        ConstPolyhedralSurfacePtr ;

    //not SFA, appears in GML/CityGML
    class Solid ;
    typedef boost::shared_ptr< Solid >             SolidPtr ;
	typedef boost::shared_ptr< const Solid >       ConstSolidPtr ;

    //not SFA, appears in GML/CityGML
    class MultiSolid ;
    typedef boost::shared_ptr< MultiSolid >        MultiSolidPtr ;
	typedef boost::shared_ptr< const MultiSolid >  ConstMultiSolidPtr ;

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
     * @warning codes for abstract classes and unimplement classes are hidden
     * @warning code values have are important for WKB
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
       TYPE_TRIANGULATEDSURFACE = 16,

       //-- not official codes
       TYPE_TRIANGLE            = 100, //17 in Wikipedia???
       TYPE_SOLID               = 101,
       TYPE_MULTISOLID          = 102
    } GeometryType ;


    /**
     * EnvelopeDeclare coordinate types
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
        * Factory function. Returns a SFCGAL::Geometry from a CGAL geometry
        */
       static Geometry* fromCGAL( const CGAL::Object& obj );

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

       //virtual bool         isSimple() const = 0 ;
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
       Envelope             envelope() const ;

       /**
        * [OGC/SFA]Return the boundary of the geometry
        */
       virtual Geometry*    boundary() const ;


       //inline int SRID() const ;

       /**
        * returns the distance to an other geometry
        */
       double distance( const Geometry & other ) const ;
       /**
        * returns the 3D distance to an other geometry
        */
       double distance3D( const Geometry & other ) const ;

       /**
        * [OGC/SFA]Return the number of geometries in a collection of geometries
        * @warning 1 for Point, LineString, Polygon, Triangle
        */
        virtual size_t             numGeometries() const ;
       /**
        * [OGC/SFA]Returns the n-th geometry
        * @warning *this for Point, LineString, Polygon, Triangle
        */
       virtual const Geometry  &  geometryN( size_t const& n ) const ;
       /**
        * [OGC/SFA]Returns the n-th geometry
        * @warning *this for Point, LineString, Polygon, Triangle
        */
       virtual Geometry &          geometryN( size_t const& n ) ;


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

       /**
	* Serializer
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
     */
    bool operator==( const Geometry&, const Geometry& );

}

#endif
