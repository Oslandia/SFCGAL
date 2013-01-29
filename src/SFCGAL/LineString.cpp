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
	_points()
{
	for ( size_t i = 0; i < points.size(); i++ ){
		_points.push_back( points[i].clone() ) ;
	}
}

///
///
///
LineString::LineString( const Point & startPoint, const Point & endPoint ):
	Geometry(),
	_points()
{
	_points.push_back( startPoint.clone() );
	_points.push_back( endPoint.clone() );
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
	_points.resize( other.numPoints() );
	for ( size_t i = 0; i < other.numPoints(); i++ ){
		_points[i] = other.pointN(i) ;
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
	return new LineString( *this );
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
	return isEmpty() ? 0 : _points[0].coordinateDimension() ;
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
size_t LineString::numSegments() const
{
	if ( _points.empty() ){
		return 0 ;
	}
	BOOST_ASSERT( numPoints() >= 2U );
	return numPoints() - 1 ;
}

///
///
///
bool LineString::isClosed() const
{
	return ( ! isEmpty() ) && ( startPoint() == endPoint() ) ;
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

///
///
///
CGAL::Polygon_2< Kernel > LineString::toPolygon_2() const
{
	if ( isEmpty() ){
		return CGAL::Polygon_2< Kernel >();
	}

	Point_2_const_iterator pend = points_2_end();
	// skip the last point
	pend--;
	return CGAL::Polygon_2< Kernel >( points_2_begin(), pend );
}


}//SFCGAL

