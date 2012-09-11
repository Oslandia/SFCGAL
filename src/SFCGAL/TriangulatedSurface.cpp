#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
TriangulatedSurface::TriangulatedSurface():
	Surface(),
	_triangles()
{

}

///
///
///
TriangulatedSurface::TriangulatedSurface( const std::vector< Triangle > & triangles ):
	Surface()
{
	_triangles.resize( triangles.size() ) ;
	for ( size_t i = 0; i < triangles.size(); i++ ){
		_triangles[i] = TrianglePtr( triangles[i].clone() ) ;
	}
}

///
///
///
TriangulatedSurface::TriangulatedSurface( TriangulatedSurface const& other ):
	Surface()
{
	(*this) = other ;
}

///
///
///
TriangulatedSurface& TriangulatedSurface::operator = ( const TriangulatedSurface & other )
{
	_triangles.resize( other.numTriangles() ) ;
	for ( size_t i = 0; i < other.numTriangles(); i++ ){
		_triangles[i] = TrianglePtr( other.triangleN(i).clone() ) ;
	}
	return *this ;
}

///
///
///
TriangulatedSurface::~TriangulatedSurface()
{

}

///
///
///
TriangulatedSurface * TriangulatedSurface::clone() const
{
	return new TriangulatedSurface( *this );
}

///
///
///
std::string  TriangulatedSurface::geometryType() const
{
	return "TriangulatedSurface" ;
}

///
///
///
GeometryType TriangulatedSurface::geometryTypeId() const
{
	return TYPE_TIN ;
}

///
///
///
int TriangulatedSurface::dimension() const
{
	//surface
	return 2 ;
}

///
///
///
int TriangulatedSurface::coordinateDimension() const
{
	if ( _triangles.empty() ){
		return 0 ;
	}else{
		return _triangles[0]->coordinateDimension() ;
	}
}

///
///
///
bool TriangulatedSurface::isEmpty() const
{
	return _triangles.empty() ;
}

///
///
///
bool TriangulatedSurface::is3D() const
{
	return ! _triangles.empty() && _triangles.front()->is3D() ;
}

///
///
///
void TriangulatedSurface::accept( GeometryVisitor & visitor )
{
	return visitor.visit(*this);
}

///
///
///
void TriangulatedSurface::accept( ConstGeometryVisitor & visitor ) const
{
	return visitor.visit(*this);
}


}//SFCGAL


