#include <SFCGAL/io/OsgFactory.h>

#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/triangulate.h>

#include <SFCGAL/Exception.h>


namespace SFCGAL {
namespace io {

///
///
///
osg::Geometry* OsgFactory::createGeometry( const Geometry & g )
{
	switch ( g.geometryTypeId() ){
	case TYPE_POINT:
		return createGeometry(g.as< Point >() );
	case TYPE_LINESTRING:
		return createGeometry(g.as< LineString >() );
	case TYPE_POLYGON:
		return createGeometry(g.as< Polygon >() );
	case TYPE_TRIANGLE:
		return createGeometry(g.as< Triangle >() );
	//case TYPE_GEOMETRYCOLLECTION:
	//	return createGeometry(g.as< GeometryCollection >() );
	case TYPE_MULTIPOINT:
		return createGeometry(g.as< MultiPoint >() );
	case TYPE_MULTILINESTRING:
		return createGeometry(g.as< MultiLineString >() );
	case TYPE_MULTIPOLYGON:
		return createGeometry(g.as< MultiPolygon >() );
	case TYPE_TIN:
		return createGeometry(g.as< TriangulatedSurface >() );
	case TYPE_POLYHEDRALSURFACE:
		return createGeometry(g.as< PolyhedralSurface >() );
	case TYPE_SOLID:
		return createGeometry(g.as< Solid >() );
	case TYPE_MULTISOLID:
		return createGeometry(g.as< MultiSolid >() );
	}
	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("can't convert %1% to osg::Geometry") % g.geometryType() ).str()
	));
}

///
///
///
osg::Geometry* OsgFactory::createGeometry( const Point & g )
{
	if ( g.isEmpty() )
		return NULL ;

	osg::ref_ptr< osg::Geometry > geometry( new osg::Geometry );

	osg::Vec3Array * vertices = new osg::Vec3Array() ;
	geometry->setVertexArray( vertices );

	osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::POINTS, 0 );
	primitiveSet->push_back( createVertex( vertices, g ) );
	geometry->addPrimitiveSet( primitiveSet );

	return geometry.release() ;
}


///
///
///
osg::Geometry* OsgFactory::createGeometry( const LineString & g )
{
	if ( g.isEmpty() )
		return NULL ;

	osg::ref_ptr< osg::Geometry > geometry( new osg::Geometry );
	osg::Vec3Array * vertices = new osg::Vec3Array() ;
	geometry->setVertexArray( vertices );

	size_t start = vertices->size() ;
	for ( size_t i = 0; i < g.numPoints(); i++ ){
		createVertex( vertices, g.pointN( i ) ) ;
	}
	geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, start, g.numPoints() ) );

	return geometry.release();
}

///
///
///
osg::Geometry* OsgFactory::createGeometry( const Polygon & g )
{
	if ( g.isEmpty() )
		return NULL ;

	TriangulatedSurface triangulatedSurface ;
	algorithm::triangulate( g, triangulatedSurface );
	return createGeometry( triangulatedSurface );
}

///
///
///
osg::Geometry* OsgFactory::createGeometry( const Triangle & g )
{
	if ( g.isEmpty() )
		return NULL ;

	osg::ref_ptr< osg::Geometry > geometry( new osg::Geometry );
	osg::Vec3Array * vertices = new osg::Vec3Array() ;
	geometry->setVertexArray( vertices );

	osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
	for ( size_t i = 0; i < 3; i++ ){
		primitiveSet->push_back( createVertex( vertices, g.vertex( i ) ) );
	}
	geometry->addPrimitiveSet( primitiveSet );

	return geometry.release();
}

///
///
///
osg::Geometry* OsgFactory::createGeometry( const Solid & g )
{
	if ( g.isEmpty() )
		return NULL ;

	TriangulatedSurface triangulatedSurface ;
	for ( size_t i = 0; i < g.numShells(); i++ ){
		algorithm::triangulate( g.shellN(i), triangulatedSurface );
	}
	return createGeometry( triangulatedSurface );
}


///
///
///
osg::Geometry* OsgFactory::createGeometry( const TriangulatedSurface & g )
{
	if ( g.isEmpty() )
		return NULL ;

	osg::ref_ptr< osg::Geometry > geometry( new osg::Geometry );
	osg::Vec3Array * vertices = new osg::Vec3Array() ;
	osg::Vec3Array * normals  = new osg::Vec3Array() ;
	geometry->setVertexArray( vertices );


	osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
	for ( size_t i = 0; i < g.numTriangles(); i++ ){
		const Triangle & triangle = g.triangleN( i );

		osg::Vec3 a = createVec3( triangle.vertex( 0 ) );
		osg::Vec3 b = createVec3( triangle.vertex( 1 ) );
		osg::Vec3 c = createVec3( triangle.vertex( 2 ) );

		//vertices
		primitiveSet->push_back( createVertex( vertices, a ) );
		primitiveSet->push_back( createVertex( vertices, b ) );
		primitiveSet->push_back( createVertex( vertices, c ) );

		//normal
		osg::Vec3 normal = ( c - b ) ^ ( a - b ) ;
		normal.normalize();

		normals->push_back( normal ) ;
		normals->push_back( normal ) ;
		normals->push_back( normal ) ;
	}

	geometry->setNormalArray( normals );
	geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

	geometry->addPrimitiveSet( primitiveSet );
	return geometry.release();
}

///
///
///
osg::Geometry* OsgFactory::createGeometry( const PolyhedralSurface & g )
{
	if ( g.isEmpty() )
		return NULL ;

	TriangulatedSurface triangulatedSurface ;
	algorithm::triangulate( g, triangulatedSurface );
	return createGeometry( triangulatedSurface );
}


///
///
///
osg::Geometry* OsgFactory::createGeometry( const MultiPoint & g )
{
	if ( g.isEmpty() )
		return NULL ;

	osg::ref_ptr< osg::Geometry > geometry( new osg::Geometry );

	osg::Vec3Array * vertices = new osg::Vec3Array() ;
	geometry->setVertexArray( vertices );

	osg::DrawElementsUInt* primitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::POINTS, 0 );
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		primitiveSet->push_back( createVertex( vertices, g.pointN(i) ) );
	}
	geometry->addPrimitiveSet( primitiveSet );

	return geometry.release() ;
}

///
///
///
osg::Geometry* OsgFactory::createGeometry( const MultiLineString & g )
{
	if ( g.isEmpty() )
		return NULL ;

	osg::ref_ptr< osg::Geometry > geometry( new osg::Geometry );
	osg::Vec3Array * vertices = new osg::Vec3Array() ;
	geometry->setVertexArray( vertices );

	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		const LineString & lineString = g.lineStringN(i) ;

		size_t start = vertices->size() ;
		for ( size_t j = 0; j < lineString.numPoints(); j++ ){
			createVertex( vertices, lineString.pointN( j ) ) ;
		}
		geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, start, lineString.numPoints() ) );
	}

	return geometry.release();
}

///
///
///
osg::Geometry* OsgFactory::createGeometry( const MultiPolygon & g )
{
	if ( g.isEmpty() )
		return NULL ;

	TriangulatedSurface triangulatedSurface ;
	algorithm::triangulate( g, triangulatedSurface );
	return createGeometry( triangulatedSurface );
}

///
///
///
osg::Geometry* OsgFactory::createGeometry( const MultiSolid & g )
{
	if ( g.isEmpty() )
		return NULL ;

	TriangulatedSurface triangulatedSurface ;
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		const Solid & solid = g.solidN(i) ;
		for ( size_t j = 0; j < solid.numShells(); j++ ){
			algorithm::triangulate( solid.shellN(j), triangulatedSurface );
		}
	}
	return createGeometry( triangulatedSurface );
}

///
///
///
osg::Vec3 OsgFactory::createVec3( const Point & g ) const
{
	return osg::Vec3(
		isNaN( g.x() ) ? 0.0 : g.x() ,
		isNaN( g.y() ) ? 0.0 : g.y() ,
		isNaN( g.z() ) ? 0.0 : g.z()
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

