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
#include <SFCGAL/algorithm/length.h>
#include <SFCGAL/all.h>

namespace SFCGAL {
namespace algorithm {

///
///
///
double length( const LineString & g )
{
	double result = 0.0 ;
	for ( size_t i = 0; i < g.numSegments(); i++ ){
		Kernel::Segment_2 segment(
			g.pointN(i).toPoint_2(),
			g.pointN(i+1).toPoint_2()
		);
		result += CGAL::sqrt( CGAL::to_double( segment.squared_length() ) ) ;
	}
	return result ;
}

///
///
///
double length( const GeometryCollection & g )
{
	double result = 0.0 ;
	for ( GeometryCollection::const_iterator it = g.begin(); it != g.end(); ++it ){
		result += length( *it ) ;
	}
	return result ;
}


///
///
///
double length( const Geometry & g )
{
	switch ( g.geometryTypeId() ){
		case TYPE_POINT:
			return 0.0 ;
		case TYPE_LINESTRING:
			return length( g.as< LineString >() ) ;
		case TYPE_POLYGON:
			return 0.0 ;
		case TYPE_MULTIPOINT:
		case TYPE_MULTILINESTRING:
		case TYPE_MULTIPOLYGON:
		case TYPE_GEOMETRYCOLLECTION:
			return length( g.as< GeometryCollection >() );
		case TYPE_POLYHEDRALSURFACE:
		case TYPE_TRIANGULATEDSURFACE:
		case TYPE_TRIANGLE:
		case TYPE_SOLID:
		case TYPE_MULTISOLID:
			return 0.0 ;
	}
	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("undefined length for geometry type %s") % g.geometryType() ).str()
	));
	return 0.0 ;
}



//------ 3D


///
///
///
double length3D( const LineString & g )
{
	double result = 0.0 ;
	for ( size_t i = 0; i < g.numSegments(); i++ ){
		Kernel::Segment_3 segment(
			g.pointN(i).toPoint_3(),
			g.pointN(i+1).toPoint_3()
		);
		result += CGAL::sqrt( CGAL::to_double( segment.squared_length() ) ) ;
	}
	return result ;
}

///
///
///
double length3D( const GeometryCollection & g )
{
	double result = 0.0 ;
	for ( GeometryCollection::const_iterator it = g.begin(); it != g.end(); ++it ){
		result += length3D( *it ) ;
	}
	return result ;
}


///
///
///
double length3D( const Geometry & g )
{
	switch ( g.geometryTypeId() ){
		case TYPE_POINT:
			return 0.0 ;
		case TYPE_LINESTRING:
			return length3D( g.as< LineString >() ) ;
		case TYPE_POLYGON:
			return 0.0 ;
		case TYPE_MULTIPOINT:
		case TYPE_MULTILINESTRING:
		case TYPE_MULTIPOLYGON:
		case TYPE_GEOMETRYCOLLECTION:
			return length3D( g.as< GeometryCollection >() );
		case TYPE_POLYHEDRALSURFACE:
		case TYPE_TRIANGULATEDSURFACE:
		case TYPE_TRIANGLE:
		case TYPE_SOLID:
		case TYPE_MULTISOLID:
			return 0.0 ;
	}
	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("undefined length for geometry type %s") % g.geometryType() ).str()
	));
	return 0.0 ;
}


}//algorithm
}//SFCGAL

