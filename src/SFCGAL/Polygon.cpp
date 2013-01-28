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
#include <SFCGAL/Polygon.h>
#include <SFCGAL/GeometryVisitor.h>

#include <SFCGAL/Triangle.h>
#include <SFCGAL/algorithm/orientation.h>

namespace SFCGAL {

///
///
///
Polygon::Polygon():
	Surface()
{
	_rings.push_back( new LineString()  );
}

///
///
///
Polygon::Polygon( const std::vector< LineString > & rings ):
	Surface()
{
	if ( rings.empty() ){
		_rings.resize(1, new LineString() );
	}else{
		for ( size_t i = 0; i < rings.size(); i++ ){
			_rings.push_back( rings[i].clone() ) ;
		}
	}
}

///
///
///
Polygon::Polygon( const LineString & exteriorRing ):
	Surface()
{
	_rings.push_back( exteriorRing.clone() );
}

///
///
///
Polygon::Polygon( LineString * exteriorRing ):
	Surface()
{
	_rings.push_back( exteriorRing );
}

///
///
///
Polygon::Polygon( const Triangle & triangle ):
	Surface()
{
	_rings.push_back( new LineString() );
	if ( ! triangle.isEmpty() ){
		for ( size_t i = 0; i < 4; i++ ){
			exteriorRing().addPoint( triangle.vertex(i) );
		}
	}
}

///
///
///
Polygon::Polygon( Polygon const& other ):
	Surface(other)
{
	(*this) = other ;
}

///
///
///
Polygon::Polygon( const CGAL::Polygon_with_holes_2< Kernel >& poly )
{
	_rings.push_back( new LineString() );
	CGAL::Polygon_2<Kernel> outer = poly.outer_boundary();
	CGAL::Polygon_2<Kernel>::Edge_const_iterator ei;
	for ( ei = outer.edges_begin(); ei != outer.edges_end(); ++ei ) {
		_rings.back().addPoint( ei->source() );
	}
	_rings.back().addPoint( _rings.back().startPoint() );
	for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator hit = poly.holes_begin(); hit != poly.holes_end(); ++hit) {
		_rings.push_back( new LineString() );
		CGAL::Polygon_2<Kernel>::Edge_const_iterator ei;
		for ( ei = hit->edges_begin(); ei != hit->edges_end(); ++ei ) {
			_rings.back().addPoint( ei->source() );
		}
		_rings.back().addPoint( _rings.back().startPoint() );
	}
}

///
///
///
Polygon& Polygon::operator = ( const Polygon & other )
{
	_rings.clear() ;
	for ( size_t i = 0; i < other.numRings(); i++ ){
		_rings.push_back( other.ringN(i).clone() );
	}
	return *this ;
}

///
///
///
Polygon::~Polygon()
{

}

///
///
///
int Polygon::coordinateDimension() const
{
	return _rings[0].coordinateDimension() ;
}


///
///
///
std::string Polygon::geometryType() const
{
	return "Polygon" ;
}

///
///
///
GeometryType Polygon::geometryTypeId() const
{
	return TYPE_POLYGON ;
}

///
///
///
Polygon * Polygon::clone() const
{
	return new Polygon(*this);
}

///
///
///
bool   Polygon::isEmpty() const
{
	return exteriorRing().isEmpty() ;
}

///
///
///
bool   Polygon::is3D() const
{
	return exteriorRing().is3D() ;
}

///
///
///
void Polygon::reverse()
{
	for ( size_t i = 0; i < numRings(); i++ ){
		ringN(i).reverse();
	}
}


///
///
///
void Polygon::accept( GeometryVisitor & visitor )
{
	return visitor.visit(*this);
}

///
///
///
void Polygon::accept( ConstGeometryVisitor & visitor ) const
{
	return visitor.visit(*this);
}

///
///
///
bool Polygon::isCounterClockWiseOriented() const
{
	return algorithm::isCounterClockWiseOriented( *this );
}

///
///
///
CGAL::Polygon_2<Kernel> Polygon::toPolygon_2() const
{
	return exteriorRing().toPolygon_2();
}

///
///
///
CGAL::Polygon_with_holes_2<Kernel> Polygon::toPolygon_with_holes_2() const
{
	std::list<CGAL::Polygon_2<Kernel> > holes;
	for ( size_t i = 0; i < numInteriorRings(); ++i ) {
		holes.push_back( interiorRingN(i).toPolygon_2() );
	}
	return CGAL::Polygon_with_holes_2<Kernel>( exteriorRing().toPolygon_2(),
						   holes.begin(),
						   holes.end());
}

}//SFCGAL

