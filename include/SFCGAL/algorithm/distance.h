#ifndef _SFCGAL_ALGORITHM_DISTANCE_H_
#define _SFCGAL_ALGORITHM_DISTANCE_H_

#include <SFCGAL/Geometry.h>


namespace SFCGAL {
namespace algorithm {

	/**
	 * dispatch distance
	 */
	double distance( const Geometry & gA, const Geometry& gB ) ;

	/**
	 * dispatch distance from Point to Geometry
	 */
	double distancePointGeometry( const Point & gA, const Geometry& gB ) ;
	/**
	 * distance between two Points
	 */
	double distancePointPoint( const Point & gA, const Point& gB ) ;
	/**
	 * distance between a Point an a LineString
	 */
	double distancePointLineString( const Point & gA, const LineString& gB ) ;
	/**
	 * distance between a Point an a Polygon
	 */
	double distancePointPolygon( const Point & gA, const Polygon& gB ) ;


	/**
	 * dispatch distance from LineString to Geometry
	 */
	double distanceLineStringGeometry( const LineString & gA, const Geometry& gB ) ;
	/**
	 * distance between two LineStrings
	 */
	double distanceLineStringLineString( const LineString & gA, const LineString& gB ) ;
	/**
	 * distance between a LineString and a Polygon
	 */
	double distanceLineStringPolygon( const LineString & gA, const Polygon& gB ) ;



	/**
	 * dispatch distance from Polygon to Geometry
	 */
	double distancePolygonGeometry( const Polygon & gA, const Geometry& gB ) ;
	/**
	 * distance between two Polygons
	 */
	double distancePolygonPolygon( const Polygon & gA, const Polygon& gB ) ;


	double distancePointSegment( const Point & p, const Point & a, const Point & b ) ;
	double distanceSegmentSegment( const Point & a, const Point & b, const Point & c, const Point & d );

}//namespace algorithm
}//namespace SFCGAL

#endif
