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
#include <SFCGAL/Geometry.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/GeometryVisitor.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/detail/GetPointsVisitor.h>

#include <SFCGAL/algorithm/BoundaryVisitor.h>
#include <SFCGAL/algorithm/distance.h>
#include <SFCGAL/algorithm/distance3d.h>

#include <SFCGAL/detail/EnvelopeVisitor.h>

#include <SFCGAL/transform/RoundTransform.h>

#include <SFCGAL/Kernel.h>

namespace SFCGAL {

///
///
///
Geometry::~Geometry()
{

}


///
///
///
std::string Geometry::asText( const int & numDecimals ) const
{
	std::ostringstream oss;
	if ( numDecimals >= 0 ){
		oss << std::fixed ;
		oss.precision( numDecimals );
	}
	io::WktWriter writer( oss );
	bool exact = false;
	if ( numDecimals == -1 ) {
		exact = true;
	}
	writer.write( *this, exact );
	return oss.str();
}

///
///
///
Envelope   Geometry::envelope() const
{
	Envelope box ;
	detail::EnvelopeVisitor envelopeVisitor( box );
	accept(envelopeVisitor);
	return box ;
}

///
///
///
Geometry* Geometry::boundary() const
{
	algorithm::BoundaryVisitor visitor ;
	accept(visitor);
	return visitor.releaseBoundary() ;
}


///
///
///
double Geometry::distance( const Geometry & other ) const
{
	return algorithm::distance( *this, other ) ;
}

///
///
///
double Geometry::distance3D( const Geometry & other ) const
{
	return algorithm::distance3D( *this, other ) ;
}

///
///
///
void Geometry::round( const long & scale )
{
	transform::RoundTransform roundTransform( scale );
	accept( roundTransform ) ;
}

///
///
///
size_t Geometry::numGeometries() const
{
	return 1 ;
}

///
///
///
const Geometry  &  Geometry::geometryN( size_t const& n ) const
{
	BOOST_ASSERT( n == 0 );
	return *this ;
}

///
///
///
Geometry &  Geometry::geometryN( size_t const& n )
{
	BOOST_ASSERT( n == 0 );
	return *this ;
}


///
///
///
Geometry::Geometry()
{

}

///
///
///
Geometry::Geometry( Geometry const& other )
{

}

///
/// Function used to compare geometries
/// FIXME
/// Since we do not have (yet) a real "equals" operator, we only compare points coordinates
bool operator == ( const Geometry& ga, const Geometry& gb )
{
    detail::GetPointsVisitor get_points_a, get_points_b;
    ga.accept( get_points_a );
    gb.accept( get_points_b );

    if ( get_points_a.points.size() != get_points_b.points.size() )
	return false;

    for ( size_t i = 0; i < get_points_a.points.size(); ++i ) {
	bool found = false;
	for ( size_t j = 0; j < get_points_b.points.size(); ++j ) {
	    const Point& pta = *(get_points_a.points[i]);
	    const Point& ptb = *(get_points_b.points[j]);
	    if ( pta == ptb ) {
		found = true;
		break;
	    }
	}
	if (! found) {
	    return false;
	}
    }
    return true;
}

}//SFCGAL

