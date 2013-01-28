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
#include <SFCGAL/Geometry.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/GeometryVisitor.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/detail/GetPointsVisitor.h>

#include <SFCGAL/algorithm/BoundaryVisitor.h>
#include <SFCGAL/algorithm/distance.h>
#include <SFCGAL/algorithm/distance3d.h>

#include <SFCGAL/detail/EnvelopeVisitor.h>


#include <SFCGAL/Kernel.h>

namespace SFCGAL {

///
///
///
Geometry::~Geometry()
{

}

Geometry* Geometry::fromCGAL( const CGAL::Object& obj )
{
	typedef CGAL::Point_2<Kernel> Point_2;
	typedef CGAL::Segment_2<Kernel> Segment_2;
	typedef CGAL::Triangle_2<Kernel> Triangle_2;
	typedef CGAL::Point_3<Kernel> Point_3;
	typedef CGAL::Segment_3<Kernel> Segment_3;
	typedef CGAL::Triangle_3<Kernel> Triangle_3;
	
	if ( obj.empty() ) {
	    return 0;
	}

	if ( const Point_2* p = CGAL::object_cast<Point_2>(&obj) ) {
		return new Point(*p);
	}
	if ( const Point_3* p = CGAL::object_cast<Point_3>(&obj) ) {
		return new Point(*p);
	}
	else if ( const Segment_2* s = CGAL::object_cast<Segment_2>(&obj) ) {
		return new LineString( s->source(), s->target() );
	}
	else if ( const Segment_3* s = CGAL::object_cast<Segment_3>(&obj) ) {
		return new LineString( s->source(), s->target() );
	}
	else if ( const Triangle_2* t = CGAL::object_cast<Triangle_2>(&obj) ) {
		return new Triangle( t->vertex(0), t->vertex(1), t->vertex(2) );
	}
	else if ( const Triangle_3* t = CGAL::object_cast<Triangle_3>(&obj) ) {
		return new Triangle( t->vertex(0), t->vertex(1), t->vertex(2) );
	}
	else if ( const std::vector<Point_2>* v = CGAL::object_cast<std::vector<Point_2> >(&obj)) {
		MultiPoint* mp = new MultiPoint();
		for ( size_t i = 0; i < v->size(); ++i ) {
			mp->addGeometry( Point((*v)[i]) );
		}
		return mp;
	}
	else if ( const std::vector<Point_3>* v = CGAL::object_cast<std::vector<Point_3> >(&obj)) {
		MultiPoint* mp = new MultiPoint();
		for ( size_t i = 0; i < v->size(); ++i ) {
			mp->addGeometry( Point((*v)[i]) );
		}
		return mp;
	}
	// else
	std::cout << "No conversion found! " << obj.type().name() << std::endl;
	return 0;
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

