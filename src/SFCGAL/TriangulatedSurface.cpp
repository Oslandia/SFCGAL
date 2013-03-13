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
	for ( size_t i = 0; i < triangles.size(); i++ ){
		_triangles.push_back( triangles[i].clone() ) ;
	}
}

///
///
///
TriangulatedSurface::TriangulatedSurface( TriangulatedSurface const& other ):
	Surface(),
	_triangles(other._triangles)
{

}

///
///
///
TriangulatedSurface& TriangulatedSurface::operator = ( const TriangulatedSurface & other )
{
	_triangles = other._triangles ;
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
	return TYPE_TRIANGULATEDSURFACE ;
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
		return _triangles[0].coordinateDimension() ;
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
	return ! _triangles.empty() && _triangles.front().is3D() ;
}

///
///
///
bool TriangulatedSurface::isMeasured() const
{
	return ! _triangles.empty() && _triangles.front().isMeasured() ;
}


///
///
///
void  TriangulatedSurface::addTriangles( const TriangulatedSurface & other )
{
	for ( TriangulatedSurface::const_iterator it = other.begin(); it != other.end(); ++it ){
		addTriangle( *it ) ;
	}
}


///
///
///
size_t  TriangulatedSurface::numGeometries() const
{
	return _triangles.size();
}

///
///
///
const Triangle  &  TriangulatedSurface::geometryN( size_t const& n ) const
{
	BOOST_ASSERT( n < numGeometries() );
	return _triangles[n];
}

///
///
///
Triangle &   TriangulatedSurface::geometryN( size_t const& n )
{
	BOOST_ASSERT( n < numGeometries() );
	return _triangles[n];
}

///
///
///
void TriangulatedSurface::reserve( const size_t & n )
{
	_triangles.reserve(n);
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


