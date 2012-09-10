#include <SFCGAL/LineString.h>
#include <SFCGAL/GeometryVisitor.h>

namespace SFCGAL {

///
///
///
LineString::LineString():
	Geometry(),
	_points()
{

}

///
///
///
LineString::LineString( const std::vector< Point > & points ):
	Geometry(),
	_points(points.size())
{
	for ( size_t i = 0; i < points.size(); i++ ){
		_points[i] = points[i].clone() ;
	}
}

///
///
///
LineString::LineString( const Point & startPoint, const Point & endPoint ):
	Geometry(),
	_points(2)
{
	_points[0] = startPoint.clone() ;
	_points[1] = endPoint.clone() ;
}

///
///
///
LineString::LineString( LineString const& other ):
	Geometry()
{
	(*this) = other ;
}

///
///
///
LineString& LineString::operator = ( const LineString & other )
{
	clear();
	_points.resize( other.numPoints() );
	for ( size_t i = 0; i < other.numPoints(); i++ ){
		_points[i] = other.pointN(i).clone() ;
	}
	return *this ;
}

///
///
///
LineString::~LineString()
{

}

///
///
///
LineString * LineString::clone() const
{
	return new LineString(*this);
}


///
///
///
GeometryType LineString::geometryTypeId() const
{
	return TYPE_LINESTRING ;
}

///
///
///
std::string LineString::geometryType() const
{
	return "LineString" ;
}

///
///
///
int LineString::dimension() const
{
	return 1 ;
}

///
///
int   LineString::coordinateDimension() const
{
	return isEmpty() ? 0 : _points[0]->coordinateDimension() ;
}

///
///
///
bool   LineString::isEmpty() const
{
	return _points.empty() ;
}

///
///
///
bool  LineString::is3D() const
{
	return ! isEmpty() && startPoint().is3D() ;
}

///
///
///
void LineString::clear()
{
	for ( std::vector< Point* >::iterator it = _points.begin(); it != _points.end(); ++it ){
		delete *it ;
		*it = NULL ;
	}
	_points.clear();
}

///
///
///
void LineString::reverse()
{
	std::reverse( _points.begin(), _points.end() );
}




///
///
///
void LineString::accept( GeometryVisitor & visitor )
{
	return visitor.visit(*this);
}

///
///
///
void LineString::accept( ConstGeometryVisitor & visitor ) const
{
	return visitor.visit(*this);
}


}//SFCGAL

