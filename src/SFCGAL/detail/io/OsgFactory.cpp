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
#include <SFCGAL/config.h>
#ifdef SFCGAL_WITH_OSG

#include <SFCGAL/detail/io/OsgFactory.h>

#include <SFCGAL/all.h>
#include <SFCGAL/triangulate/triangulatePolygon.h>
#include <SFCGAL/Exception.h>


namespace SFCGAL {
namespace io {

	void OsgFactory::addToGeometry( osg::Geometry* geometry, const Geometry& g )
	{
		if ( g.is<GeometryCollection>() ) {
			addToGeometry( geometry, g.as<GeometryCollection>() );
			return;
		}

		switch ( g.geometryTypeId() ){
		case TYPE_POINT:
			addToGeometry( geometry, g.as<Point>() );
			break;
		case TYPE_LINESTRING:
			addToGeometry( geometry, g.as<LineString>() );
			break;
		case TYPE_POLYGON:
			addToGeometry( geometry, g.as<Polygon>() );
			break;
		case TYPE_TRIANGLE:
			addToGeometry( geometry, g.as<Triangle>() );
			break;
		case TYPE_TRIANGULATEDSURFACE:
			addToGeometry( geometry, g.as<TriangulatedSurface>() );
			break;
		case TYPE_POLYHEDRALSURFACE:
			addToGeometry( geometry, g.as<PolyhedralSurface>() );
			break;
		case TYPE_SOLID:
			addToGeometry( geometry, g.as<Solid>() );
			break;
		default:
			BOOST_THROW_EXCEPTION(Exception(
							( boost::format("can't convert %1% to osg::Geometry") % g.geometryType() ).str()
							));
			break;
		}
	}

	void OsgFactory::addToGeometry( osg::Geometry* geometry, const Point& p )
	{
		osg::Vec3Array * vertices = static_cast<osg::Vec3Array*>(geometry->getVertexArray());

		size_t start = vertices->size();
		osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::POINTS, start );
		primitiveSet->push_back( createVertex( vertices, p ) );
		geometry->addPrimitiveSet( primitiveSet );
	}

	void OsgFactory::addToGeometry( osg::Geometry* geometry, const LineString& g )
	{
		osg::Vec3Array * vertices = static_cast<osg::Vec3Array*>(geometry->getVertexArray());

		size_t start = vertices->size() ;
		for ( size_t i = 0; i < g.numPoints(); i++ ){
			createVertex( vertices, g.pointN( i ) ) ;
		}
		geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, start, g.numPoints() ) );
	}

	void OsgFactory::addToGeometry( osg::Geometry* geometry, const Triangle& g )
	{
		osg::Vec3Array * vertices = static_cast<osg::Vec3Array*>(geometry->getVertexArray());

		size_t start = vertices->size();
		osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, start );
		for ( size_t i = 0; i < 3; i++ ){
			primitiveSet->push_back( createVertex( vertices, g.vertex( i ) ) );
		}
		geometry->addPrimitiveSet( primitiveSet );
	}

	void OsgFactory::addToGeometry( osg::Geometry* geometry, const TriangulatedSurface& g )
	{
		osg::Vec3Array * vertices = static_cast<osg::Vec3Array*>(geometry->getVertexArray());
		osg::Vec3Array * normals = static_cast<osg::Vec3Array*>(geometry->getNormalArray());

		size_t start = vertices->size() ;
		for ( size_t i = 0; i < g.numGeometries(); i++ ){
			const Triangle & triangle = g.geometryN( i );
			
			osg::Vec3 a = createVec3( triangle.vertex( 0 ) );
			osg::Vec3 b = createVec3( triangle.vertex( 1 ) );
			osg::Vec3 c = createVec3( triangle.vertex( 2 ) );
			
			//vertices
			createVertex( vertices, a ) ;
			createVertex( vertices, b ) ;
			createVertex( vertices, c ) ;
			
			//normal
			osg::Vec3 normal = ( c - b ) ^ ( a - b ) ;
			normal.normalize();
			
			normals->push_back( normal ) ;
			normals->push_back( normal ) ;
			normals->push_back( normal ) ;
		}
		
		geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
		geometry->addPrimitiveSet(  new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, start, g.numGeometries() * 3 ) );
	}

	void OsgFactory::addToGeometry( osg::Geometry* geometry, const Polygon& g )
	{
		TriangulatedSurface surf;
		triangulate::triangulatePolygon3D( g, surf );
		addToGeometry( geometry, surf );
	}

	void OsgFactory::addToGeometry( osg::Geometry* geometry, const PolyhedralSurface& g )
	{
		TriangulatedSurface surf;
		triangulate::triangulatePolygon3D( g, surf );
		addToGeometry( geometry, surf );
	}

	void OsgFactory::addToGeometry( osg::Geometry* geometry, const Solid& g )
	{
		TriangulatedSurface triangulatedSurface ;
		for ( size_t i = 0; i < g.numShells(); i++ ){
			triangulate::triangulatePolygon3D( g.shellN(i), triangulatedSurface );
		}
		addToGeometry( geometry, triangulatedSurface );
	}

	void OsgFactory::addToGeometry( osg::Geometry* geometry, const GeometryCollection& g )
	{
		for ( size_t i = 0; i < g.numGeometries(); ++i ) {
			// pseudo-recurse call
			addToGeometry( geometry, g.geometryN( i ) );
		}
	}
///
///
///
osg::Geometry* OsgFactory::createGeometry( const Geometry & g )
{
	if ( g.isEmpty() ) {
		return NULL;
	}

	osg::ref_ptr<osg::Geometry> geometry( new osg::Geometry );
	geometry->setVertexArray( new osg::Vec3Array() );
	geometry->setNormalArray( new osg::Vec3Array() );

	addToGeometry( geometry.get(), g );
	return geometry.release();
}

///
///
///
osg::Vec3 OsgFactory::createVec3( const Point & g ) const
{
	return osg::Vec3(
		CGAL::to_double( g.x() ),
		CGAL::to_double( g.y() ),
		CGAL::to_double( g.z() )
	) ;
}

///
///
///
size_t OsgFactory::createVertex( osg::Vec3Array * vertices, const Point & g )
{
	return createVertex( vertices, createVec3(g) );
}

///
///
///
size_t OsgFactory::createVertex( osg::Vec3Array * vertices, const osg::Vec3 & g )
{
	size_t id = vertices->size();
	vertices->push_back( g );
	return id ;
}

}//io
}//SFCGAL


#endif // SFCGAL_WITH_OSG

