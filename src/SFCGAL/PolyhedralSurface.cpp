#include <SFCGAL/PolyhedralSurface.h>


namespace SFCGAL {

///
///
///
PolyhedralSurface::PolyhedralSurface():
	_polygons()
{

}

///
///
///
PolyhedralSurface::PolyhedralSurface( const std::vector< Polygon > & polygons ) :
	_polygons(polygons)
{

}

///
///
///
PolyhedralSurface::PolyhedralSurface( PolyhedralSurface const& other ) :
	_polygons(other._polygons)
{

}

///
///
///
PolyhedralSurface& PolyhedralSurface::operator = ( const PolyhedralSurface & other )
{
	_polygons = other._polygons ;
	return *this ;
}


///
///
///
PolyhedralSurface::~PolyhedralSurface()
{

}

///
///
///
PolyhedralSurface * PolyhedralSurface::clone() const
{
	return new PolyhedralSurface(*this);
}

///
///
///
std::string  PolyhedralSurface::geometryType() const
{
	return "PolyhedralSurface" ;
}

///
///
///
GeometryType  PolyhedralSurface::geometryTypeId() const
{
	return TYPE_POLYHEDRALSURFACE ;
}

///
///
///
int  PolyhedralSurface::dimension() const
{
	return 2 ;
}

///
///
///
int  PolyhedralSurface::coordinateDimension() const
{
	if ( isEmpty() ){
		return 0 ;
	}else{
		return _polygons.front().coordinateDimension() ;
	}
}

///
///
///
bool  PolyhedralSurface::isEmpty() const
{
	return _polygons.empty();
}

///
///
///
bool  PolyhedralSurface::is3D() const
{
	if ( isEmpty() ){
		return false ;
	}else{
		return _polygons.front().is3D() ;
	}
}


}//SFCGAL



