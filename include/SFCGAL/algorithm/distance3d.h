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
#ifndef _SFCGAL_ALGORITHM_DISTANCE3D_H_
#define _SFCGAL_ALGORITHM_DISTANCE3D_H_

#include <SFCGAL/config.h>

#include <SFCGAL/Geometry.h>

namespace SFCGAL {
namespace algorithm {

	/**
	 * dispatch distance between two Geometries
	 */
	SFCGAL_API double distance3D( const Geometry & gA, const Geometry& gB ) ;

	/**
	 * dispatch distance from Point to Geometry
	 */
	SFCGAL_API double distancePointGeometry3D( const Point & gA, const Geometry& gB ) ;
	/**
	 * distance between two Points
	 */
	SFCGAL_API double distancePointPoint3D( const Point & gA, const Point& gB ) ;
	/**
	 * distance between a Point and a LineString
	 */
	SFCGAL_API double distancePointLineString3D( const Point & gA, const LineString& gB ) ;
	/**
	 * distance between a Point and a Triangle
	 */
	SFCGAL_API double distancePointTriangle3D( const Point & gA, const Triangle& gB ) ;
	/**
	 * distance between a Point and a Triangle
	 */
	SFCGAL_API double distancePointPolygon3D( const Point & gA, const Polygon& gB ) ;
	/**
	 * distance between a Point and a Solid
	 */
	SFCGAL_API double distancePointSolid3D( const Point & gA, const Solid& gB ) ;


	/**
	 * dispatch distance between a LineString and a Geometry
	 */
	SFCGAL_API double distanceLineStringGeometry3D( const LineString & gA, const Geometry& gB ) ;
	/**
	 * distance between two LineStrings
	 */
	SFCGAL_API double distanceLineStringLineString3D( const LineString & gA, const LineString& gB ) ;
	/**
	 * distance between a LineString and a Triangle
	 */
	SFCGAL_API double distanceLineStringTriangle3D( const LineString & gA, const Triangle& gB ) ;
	/**
	 * distance between a LineString and a Polygon
	 * @todo same method than distancePointPolygon3D (unify if triangulate is available)
	 */
	SFCGAL_API double distanceLineStringPolygon3D( const LineString & gA, const Polygon & gB ) ;
	/**
	 * distance between a LineString and a Solid
	 */
	SFCGAL_API double distanceLineStringSolid3D( const LineString & gA, const Solid & gB ) ;

	/**
	 * dispatch distance between a Triangle and a Geometry
	 */
	SFCGAL_API double distanceTriangleGeometry3D( const Triangle & gA, const Geometry& gB ) ;
	/**
	 * distance between a Triangle and a Solid
	 */
	SFCGAL_API double distanceTriangleSolid3D( const Triangle & gA, const Solid& gB ) ;



	/**
	 * dispatch distance between a Polygon and a Geometry
	 */
	SFCGAL_API double distancePolygonGeometry3D( const Polygon & gA, const Geometry& gB ) ;


	/**
	 * dispatch distance between a Solid and a Geometry
	 */
	SFCGAL_API double distanceSolidGeometry3D( const Solid & gA, const Geometry& gB ) ;
	/**
	 * distance between two Solids
	 */
	SFCGAL_API double distanceSolidSolid3D( const Solid & gA, const Solid& gB ) ;


	/**
	 * dispatch distance from a collection of geometry (gA) to a Geometry (gB)
	 */
	SFCGAL_API double distanceGeometryCollectionToGeometry3D( const Geometry & gA, const Geometry& gB ) ;


	SFCGAL_API double distancePointSegment3D( const Point & p, const Point & a, const Point & b ) ;
	SFCGAL_API double distancePointTriangle3D( const Point & p, const Point& a, const Point& b, const Point& c ) ;
	SFCGAL_API double distanceSegmentSegment3D( const Point & a, const Point & b, const Point & c, const Point & d );
	SFCGAL_API double distanceSegmentTriangle3D(
		const Point & sA, const Point & sB,
		const Point & tA, const Point & tB, const Point & tC
	);
	/**
	 * distance between two Triangles
	 */
	SFCGAL_API double distanceTriangleTriangle3D( const Triangle & gA, const Triangle& gB ) ;



}//namespace algorithm
}//namespace SFCGAL

#endif
