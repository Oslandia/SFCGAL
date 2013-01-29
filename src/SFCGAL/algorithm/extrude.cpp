/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
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

#include <SFCGAL/tools/Log.h>


namespace SFCGAL {
namespace algorithm {

///
///
///
std::auto_ptr< Geometry > extrude( const Geometry & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	SFCGAL_INFO( boost::format( "extrude(%1%,%2%,%3%,%4%)" ) % g.asText(1) % dx % dy % dz );
	switch ( g.geometryTypeId() ){
	case TYPE_POINT:
		return std::auto_ptr< Geometry >( extrude( g.as< Point >(), dx, dy, dz ));
	case TYPE_LINESTRING:
		return std::auto_ptr< Geometry >( extrude( g.as< LineString >(), dx, dy, dz ));
	case TYPE_POLYGON:
		return std::auto_ptr< Geometry >( extrude( g.as< Polygon >(), dx, dy, dz ));
	case TYPE_TRIANGLE:
		return std::auto_ptr< Geometry >( extrude( g.as< Triangle >(), dx, dy, dz ));
	case TYPE_GEOMETRYCOLLECTION:
		return std::auto_ptr< Geometry >( extrude( g.as< GeometryCollection >(), dx, dy, dz ) );
	case TYPE_MULTIPOINT:
		return std::auto_ptr< Geometry >( extrude( g.as< MultiPoint >(), dx, dy, dz ));
	case TYPE_MULTILINESTRING:
		return std::auto_ptr< Geometry >( extrude( g.as< MultiLineString >(), dx, dy, dz ));
	case TYPE_MULTIPOLYGON:
		return std::auto_ptr< Geometry >( extrude( g.as< MultiPolygon >(), dx, dy, dz ));
	case TYPE_TRIANGULATEDSURFACE:
		return std::auto_ptr< Geometry >( extrude( g.as< TriangulatedSurface >(), dx, dy, dz ));
	case TYPE_POLYHEDRALSURFACE:
		return std::auto_ptr< Geometry >( extrude( g.as< PolyhedralSurface >(), dx, dy, dz ));
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
LineString * extrude( const Point & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	BOOST_ASSERT( ! g.isEmpty() );
	BOOST_ASSERT( g.is3D() );

	Point translated( g ) ;
	translate( translated, dx, dy, dz );
	return new LineString(
		g,
		translated
	);
}

///
///
///
PolyhedralSurface * extrude( const LineString & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	BOOST_ASSERT( ! g.isEmpty() );
	BOOST_ASSERT( g.is3D() );

	std::auto_ptr< PolyhedralSurface > polyhedralSurface( new PolyhedralSurface() );
	for ( size_t i = 0; i < g.numPoints() - 1; i++ ){
		std::vector< Point > ring ;
		ring.push_back( g.pointN(i) );

		ring.push_back( g.pointN(i+1) );

		ring.push_back( g.pointN(i+1) );
		translate( ring.back(), dx, dy, dz );

		ring.push_back( g.pointN(i) );
		translate( ring.back(), dx, dy, dz );

		ring.push_back( g.pointN(i) );
		polyhedralSurface->addPolygon( Polygon( ring ) );
	}
	return polyhedralSurface.release() ;
}

///
///
///
Solid * extrude( const Polygon & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	BOOST_ASSERT( ! g.isEmpty() );
	BOOST_ASSERT( g.is3D() );

	bool reverseOrientation = ( Kernel::Vector_3( dx, dy, dz ) * normal3D< Kernel >( g ) ) > 0 ;

	//resulting shell
	PolyhedralSurface polyhedralSurface ;

	// "bottom"
	Polygon bottom(g);
	if ( reverseOrientation ){
		bottom.reverse();
	}
	polyhedralSurface.addPolygon( bottom );

	// "top"
	Polygon top( bottom );
	top.reverse() ;
	translate(top,dx,dy,dz);
	polyhedralSurface.addPolygon( top );

	// exterior ring and interior rings extruded
	for ( size_t i = 0; i < bottom.numRings(); i++ ){
		std::auto_ptr< PolyhedralSurface > boundaryExtruded( extrude( bottom.ringN(i), dx, dy, dz ) );
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
Solid *  extrude( const Triangle & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	BOOST_ASSERT( ! g.isEmpty() );
	BOOST_ASSERT( g.is3D() );
	return extrude( g.toPolygon(), dx, dy, dz );
}

///
///
///
MultiLineString *     extrude( const MultiPoint & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	std::auto_ptr< MultiLineString > result( new MultiLineString() );
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		result->addGeometry( extrude( g.pointN(i), dx, dy, dz ) );
	}
	return result.release() ;
}


///
///
///
PolyhedralSurface *   extrude( const MultiLineString & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	std::auto_ptr< PolyhedralSurface > result( new PolyhedralSurface() );
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		std::auto_ptr< PolyhedralSurface > extruded( extrude( g.lineStringN(i), dx, dy, dz ) );
		for ( size_t j = 0; j < extruded->numPolygons(); j++ ){
			result->addPolygon( extruded->polygonN(j) );
		}
	}
	return result.release() ;
}


///
///
///
MultiSolid *          extrude( const MultiPolygon & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	std::auto_ptr< MultiSolid > result( new MultiSolid() );
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		result->addGeometry( extrude( g.polygonN(i), dx, dy, dz ) );
	}
	return result.release() ;
}


///
///
///
Solid *   extrude( const TriangulatedSurface & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	std::auto_ptr< Solid > result( new Solid() );

	//bottom and top
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		Triangle bottomPart( g.geometryN(i) );
		bottomPart.reverse() ;
		result->exteriorShell().addPolygon( bottomPart );

		Triangle topPart( g.geometryN(i) );
		translate( topPart, dx, dy, dz );
		result->exteriorShell().addPolygon( topPart );
	}

	//boundary
	std::auto_ptr< Geometry > boundary( g.boundary() ) ;
	BOOST_ASSERT( boundary.get() != NULL );
	std::auto_ptr< Geometry > extrudedBoundary( extrude(*boundary, dx, dy, dz) ) ;
	BOOST_ASSERT( extrudedBoundary->is< PolyhedralSurface >() );
	result->exteriorShell().addPolygons( extrudedBoundary->as< PolyhedralSurface >() );

	return result.release() ;
}

///
///
///
Solid *   extrude( const PolyhedralSurface & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	TriangulatedSurface triangulatedSurface ;
	triangulate( g, triangulatedSurface );
	return extrude( g, dx, dy, dz ) ;
}

///
///
///
GeometryCollection*   extrude( const GeometryCollection & g, Kernel::FT dx, Kernel::FT dy, Kernel::FT dz )
{
	std::auto_ptr< GeometryCollection > result( new GeometryCollection() ) ;
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		result->addGeometry( extrude( g.geometryN(i), dx, dy, dz ).release() );
	}
	return result.release() ;
}


}//algorithm
}//SFCGAL
