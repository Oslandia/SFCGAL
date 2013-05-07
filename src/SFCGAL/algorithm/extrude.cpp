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
#include <SFCGAL/algorithm/extrude.h>
#include <SFCGAL/Exception.h>
#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/normal.h>
#include <SFCGAL/algorithm/translate.h>
#include <SFCGAL/algorithm/force3D.h>

#include <SFCGAL/tools/Log.h>


namespace SFCGAL {
namespace algorithm {

//-- private interface

LineString *          extrude( const Point & g, const Kernel::Vector_3 & v  ) ;
PolyhedralSurface *   extrude( const LineString & g, const Kernel::Vector_3 & v ) ;
Solid *               extrude( const Polygon & g, const Kernel::Vector_3 & v ) ;
Solid *               extrude( const Triangle & g, const Kernel::Vector_3 & v ) ;

MultiLineString *     extrude( const MultiPoint & g, const Kernel::Vector_3 & v ) ;
PolyhedralSurface *   extrude( const MultiLineString & g, const Kernel::Vector_3 & v ) ;
MultiSolid *          extrude( const MultiPolygon & g, const Kernel::Vector_3 & v ) ;

/**
 * @warning suppose that the TriangulatedSurface is connected
 * @todo take orientation in account
 */
Solid *               extrude( const TriangulatedSurface & g, const Kernel::Vector_3 & v ) ;
/**
 * @warning doesn't take orientation in account
 * @todo take orientation in account
 */
Solid *               extrude( const PolyhedralSurface & g, const Kernel::Vector_3 & v ) ;

/**
 * extrude each geometry in a GeometryCollection
 */
GeometryCollection*   extrude( const GeometryCollection & g, const Kernel::Vector_3 & v ) ;


///
///
///
LineString * extrude( const Point & g, const Kernel::Vector_3 & v )
{
	BOOST_ASSERT( ! g.isEmpty() );

	Kernel::Point_3 a = g.toPoint_3() ;
	Kernel::Point_3 b = a + v ;

	return new LineString( Point( a ), Point( b ) );
}

///
///
///
PolyhedralSurface * extrude( const LineString & g, const Kernel::Vector_3 & v )
{
	BOOST_ASSERT( ! g.isEmpty() );

	std::auto_ptr< PolyhedralSurface > polyhedralSurface( new PolyhedralSurface() );
	for ( size_t i = 0; i < g.numPoints() - 1; i++ ){
		std::auto_ptr< LineString > ring( new LineString ) ;

		Kernel::Point_3 a = g.pointN(i).toPoint_3() ;
		Kernel::Point_3 b = g.pointN(i+1).toPoint_3() ;
		ring->addPoint( new Point(a) );
		ring->addPoint( new Point(b) );
		ring->addPoint( new Point(b + v) );
		ring->addPoint( new Point(a + v) );
		ring->addPoint( new Point(a) );

		polyhedralSurface->addPolygon( new Polygon( ring.release() ) );
	}
	return polyhedralSurface.release() ;
}

///
///
///
Solid * extrude( const Polygon & g, const Kernel::Vector_3 & v )
{
	BOOST_ASSERT( ! g.isEmpty() );

	bool reverseOrientation = ( v * normal3D< Kernel >( g ) ) > 0 ;

	//resulting shell
	PolyhedralSurface polyhedralSurface ;

	// "bottom"
	Polygon bottom(g);
	force3D( bottom ) ;
	if ( reverseOrientation ){
		bottom.reverse();
	}
	polyhedralSurface.addPolygon( bottom );

	// "top"
	Polygon top( bottom );
	top.reverse() ;
	translate(top,v);
	polyhedralSurface.addPolygon( top );

	// exterior ring and interior rings extruded
	for ( size_t i = 0; i < bottom.numRings(); i++ ){
		std::auto_ptr< PolyhedralSurface > boundaryExtruded( extrude( bottom.ringN(i), v ) );
		for ( size_t j = 0; j < boundaryExtruded->numPolygons(); j++ ){
			boundaryExtruded->polygonN(j).reverse() ;
			polyhedralSurface.addPolygon( boundaryExtruded->polygonN(j) ) ;
		}
	}

	return new Solid( polyhedralSurface );
}


///
///
///
Solid *  extrude( const Triangle & g, const Kernel::Vector_3 & v )
{
	BOOST_ASSERT( ! g.isEmpty() );
	return extrude( g.toPolygon(), v );
}

///
///
///
MultiLineString *     extrude( const MultiPoint & g, const Kernel::Vector_3 & v )
{
	std::auto_ptr< MultiLineString > result( new MultiLineString() );
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		result->addGeometry( extrude( g.pointN(i), v ) );
	}
	return result.release() ;
}


///
///
///
PolyhedralSurface *   extrude( const MultiLineString & g, const Kernel::Vector_3 & v )
{
	std::auto_ptr< PolyhedralSurface > result( new PolyhedralSurface() );
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		std::auto_ptr< PolyhedralSurface > extruded( extrude( g.lineStringN(i), v ) );
		for ( size_t j = 0; j < extruded->numPolygons(); j++ ){
			result->addPolygon( extruded->polygonN(j) );
		}
	}
	return result.release() ;
}


///
///
///
MultiSolid *          extrude( const MultiPolygon & g, const Kernel::Vector_3 & v )
{
	std::auto_ptr< MultiSolid > result( new MultiSolid() );
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		result->addGeometry( extrude( g.polygonN(i), v ) );
	}
	return result.release() ;
}


///
///
///
Solid *   extrude( const TriangulatedSurface & g, const Kernel::Vector_3 & v )
{
	std::auto_ptr< Solid > result( new Solid() );

	//bottom and top
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		Triangle bottomPart( g.geometryN(i) );
		force3D( bottomPart );
		bottomPart.reverse() ;
		result->exteriorShell().addPolygon( bottomPart );

		Triangle topPart( g.geometryN(i) );
		force3D( topPart );
		translate( topPart, v );
		result->exteriorShell().addPolygon( topPart );
	}

	//boundary
	std::auto_ptr< Geometry > boundary( g.boundary() ) ;
	BOOST_ASSERT( boundary.get() != NULL );

	// closed surface extruded
	if ( ! boundary->isEmpty() ){
		std::auto_ptr< Geometry > extrudedBoundary( extrude(*boundary, v) ) ;
		BOOST_ASSERT( extrudedBoundary->is< PolyhedralSurface >() );
		result->exteriorShell().addPolygons( extrudedBoundary->as< PolyhedralSurface >() );
	}


	return result.release() ;
}

///
///
///
Solid *   extrude( const PolyhedralSurface & g, const Kernel::Vector_3 & v )
{
	TriangulatedSurface triangulatedSurface ;
	triangulate::triangulatePolygon3D( g, triangulatedSurface );
	return extrude( triangulatedSurface, v ) ;
}

///
///
///
GeometryCollection*   extrude( const GeometryCollection & g, const Kernel::Vector_3 & v )
{
	std::auto_ptr< GeometryCollection > result( new GeometryCollection() ) ;
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		result->addGeometry( extrude( g.geometryN(i), v ).release() );
	}
	return result.release() ;
}

//-- public interface

///
///
///
std::auto_ptr< Geometry > extrude( const Geometry & g, const Kernel::Vector_3 & v )
{
	if ( g.isEmpty() ){
		return std::auto_ptr< Geometry >( new GeometryCollection );
	}

	switch ( g.geometryTypeId() ){
	case TYPE_POINT:
		return std::auto_ptr< Geometry >( extrude( g.as< Point >(), v ));
	case TYPE_LINESTRING:
		return std::auto_ptr< Geometry >( extrude( g.as< LineString >(), v ));
	case TYPE_POLYGON:
		return std::auto_ptr< Geometry >( extrude( g.as< Polygon >(), v ));
	case TYPE_TRIANGLE:
		return std::auto_ptr< Geometry >( extrude( g.as< Triangle >(), v ));
	case TYPE_GEOMETRYCOLLECTION:
		return std::auto_ptr< Geometry >( extrude( g.as< GeometryCollection >(), v ) );
	case TYPE_MULTIPOINT:
		return std::auto_ptr< Geometry >( extrude( g.as< MultiPoint >(), v ));
	case TYPE_MULTILINESTRING:
		return std::auto_ptr< Geometry >( extrude( g.as< MultiLineString >(), v ));
	case TYPE_MULTIPOLYGON:
		return std::auto_ptr< Geometry >( extrude( g.as< MultiPolygon >(), v ));
	case TYPE_TRIANGULATEDSURFACE:
		return std::auto_ptr< Geometry >( extrude( g.as< TriangulatedSurface >(), v ));
	case TYPE_POLYHEDRALSURFACE:
		return std::auto_ptr< Geometry >( extrude( g.as< PolyhedralSurface >(), v ));
	case TYPE_SOLID:
	case TYPE_MULTISOLID:
		//extrusion not available
		break;
	}
	BOOST_THROW_EXCEPTION( Exception(
		( boost::format( "unexpected GeometryType in extrude ('%1%')" ) % g.geometryType() ).str()
	));
}


///
///
///
std::auto_ptr< Geometry > extrude( const Geometry & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	return extrude( g, Kernel::Vector_3(dx,dy,dz) ) ;
}


}//algorithm
}//SFCGAL
