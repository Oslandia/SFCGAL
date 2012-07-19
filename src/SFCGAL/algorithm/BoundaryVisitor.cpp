#include <SFCGAL/algorithm/BoundaryVisitor.h>
#include <SFCGAL/all.h>

#include <set>
#include <complex>
#include <SFCGAL/detail/ComplexComparator.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
void BoundaryVisitor::visit( const Point & g )
{
	_boundary.reset();
}

///
///
///
void BoundaryVisitor::visit( const LineString & g )
{
	if ( g.isEmpty() ){
		_boundary.reset();
		return ;
	}

	if ( g.startPoint().coordinate() == g.endPoint().coordinate() ){
		_boundary.reset() ;
	}else{
		std::auto_ptr< MultiPoint > boundary( new MultiPoint );
		boundary->addGeometry( g.startPoint() );
		boundary->addGeometry( g.endPoint() );
		_boundary.reset( boundary.release() );
	}
}

///
///
///
void BoundaryVisitor::visit( const Polygon & g )
{
	if ( g.isEmpty() ){
		_boundary.reset();
		return ;
	}

	if ( ! g.hasInteriorRings() ){
		_boundary.reset( g.exteriorRing().clone() );
	}else{
		std::auto_ptr< MultiLineString > boundary( new MultiLineString );
		for ( size_t i = 0; i < g.numRings(); i++ ){
			boundary->addGeometry( g.ringN(i) );
		}
		_boundary.reset( boundary.release() );
	}
}

///
///
///
void BoundaryVisitor::visit( const Triangle & g )
{
	if ( g.isEmpty() ){
		_boundary.reset();
		return ;
	}

	std::auto_ptr< LineString > boundary( new LineString );
	for ( size_t i = 0; i < 4; i++ ){
		boundary->addPoint( g.vertex(i) );
	}
	_boundary.reset( boundary.release() );
}

///
///
///
void BoundaryVisitor::visit( const Solid & g )
{
	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("unsupported type %1% in boundary operation") % g.geometryType() ).str()
	));
}

///
///
///
void BoundaryVisitor::visit( const MultiPoint & g )
{
	_boundary.reset();
}

///
///
///
void BoundaryVisitor::visit( const MultiLineString & g )
{
	if ( g.isEmpty() ){
		_boundary.reset();
		return ;
	}

	/*
	 * create a GeometryGraph and rely on vertex degree (1 means boundary)
	 */
	graph::GeometryGraph        graph ;
	graph::GeometryGraphBuilder graphBuilder( graph ) ;
	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		graphBuilder.addLineString( g.lineStringN(i) );
	}
	getBoundaryFromLineStrings( graph ) ;
}

///
///
///
void BoundaryVisitor::visit( const MultiPolygon & g )
{
	graph::GeometryGraph        graph ;
	graph::GeometryGraphBuilder graphBuilder( graph ) ;

	for ( size_t i = 0; i < g.numGeometries(); i++ ){
		graphBuilder.addPolygon(g.polygonN(i));
	}
	getBoundaryFromPolygons( graph ) ;
}

///
///
///
void BoundaryVisitor::visit( const MultiSolid & g )
{
	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("unsupported type %1% in boundary operation") % g.geometryType() ).str()
	));
}

///
///
///
void BoundaryVisitor::visit( const GeometryCollection & g )
{
	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("unsupported type %1% in boundary operation") % g.geometryType() ).str()
	));
}

///
///
///
void BoundaryVisitor::visit( const PolyhedralSurface & g )
{
	graph::GeometryGraph        graph ;
	graph::GeometryGraphBuilder graphBuilder( graph ) ;

	graphBuilder.addPolyhedralSurface( g );
	getBoundaryFromPolygons( graph ) ;
}

///
///
///
void BoundaryVisitor::visit( const TriangulatedSurface & g )
{
	graph::GeometryGraph        graph ;
	graph::GeometryGraphBuilder graphBuilder( graph ) ;

	graphBuilder.addTriangulatedSurface( g );
	getBoundaryFromPolygons( graph ) ;
}


///
///
///
Geometry* BoundaryVisitor::releaseBoundary()
{
	if ( _boundary.get() ){
		return _boundary.release() ;
	}else{
		return new GeometryCollection() ;
	}
}


///
///
///
void BoundaryVisitor::getBoundaryFromLineStrings( const graph::GeometryGraph & graph )
{
	typedef graph::GeometryGraph::vertex_descriptor vertex_descriptor ;
	typedef graph::GeometryGraph::vertex_iterator   vertex_iterator ;


	std::vector< vertex_descriptor > vertices ;

	vertex_iterator it,end ;
	for ( boost::tie(it,end) = graph.vertices(); it != end; ++it ){
		vertex_descriptor vertex = *it ;
		if ( graph.degree(vertex) == 1 ){
			vertices.push_back( vertex );
		}
	}

	if ( vertices.empty() ){
		_boundary.reset() ;
	}else if ( vertices.size() == 1 ){
		_boundary.reset( new Point( graph[ vertices[0] ].coordinate ) );
	}else{
		std::auto_ptr< MultiPoint > boundary( new MultiPoint );
		for ( size_t i = 0; i < vertices.size(); i++ ){
			boundary->addGeometry( new Point( graph[ vertices[i] ].coordinate ) ) ;
		}
		_boundary.reset( boundary.release() );
	}
}


///
///
///
void BoundaryVisitor::getBoundaryFromPolygons( const graph::GeometryGraph & g )
{
	BOOST_THROW_EXCEPTION(Exception(
		"!!!!!!! BoundaryVisitor::getBoundaryFromPolygons not implemented"
	));
}





}//algorithm
}//SFCGAL

