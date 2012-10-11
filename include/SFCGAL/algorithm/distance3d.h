#ifndef _SFCGAL_ALGORITHM_DISTANCE3D_H_
#define _SFCGAL_ALGORITHM_DISTANCE3D_H_

#include <SFCGAL/Geometry.h>


namespace SFCGAL {
namespace algorithm {

	/**
	 * dispatch distance between two Geometries
	 */
	double distance3D( const Geometry & gA, const Geometry& gB ) ;

	/**
	 * dispatch distance from Point to Geometry
	 */
	double distancePointGeometry3D( const Point & gA, const Geometry& gB ) ;
	/**
	 * distance between two Points
	 */
	double distancePointPoint3D( const Point & gA, const Point& gB ) ;
	/**
	 * distance between a Point and a LineString
	 */
	double distancePointLineString3D( const Point & gA, const LineString& gB ) ;
	/**
	 * distance between a Point and a Triangle
	 */
	double distancePointTriangle3D( const Point & gA, const Triangle& gB ) ;
	/**
	 * distance between a Point and a Triangle
	 */
	double distancePointPolygon3D( const Point & gA, const Polygon& gB ) ;
	/**
	 * distance between a Point and a Solid
	 */
	double distancePointSolid3D( const Point & gA, const Solid& gB ) ;


	/**
	 * dispatch distance between a LineString and a Geometry
	 */
	double distanceLineStringGeometry3D( const LineString & gA, const Geometry& gB ) ;
	/**
	 * distance between two LineStrings
	 */
	double distanceLineStringLineString3D( const LineString & gA, const LineString& gB ) ;
	/**
	 * distance between a LineString and a Triangle
	 */
	double distanceLineStringTriangle3D( const LineString & gA, const Triangle& gB ) ;
	/**
	 * distance between a LineString and a Polygon
	 * @todo same method than distancePointPolygon3D (unify if triangulate is available)
	 */
	double distanceLineStringPolygon3D( const LineString & gA, const Polygon & gB ) ;
	/**
	 * distance between a LineString and a Solid
	 */
	double distanceLineStringSolid3D( const LineString & gA, const Solid & gB ) ;

	/**
	 * dispatch distance between a Triangle and a Geometry
	 */
	double distanceTriangleGeometry3D( const Triangle & gA, const Geometry& gB ) ;
	/**
	 * distance between a Triangle and a Solid
	 */
	double distanceTriangleSolid3D( const Triangle & gA, const Solid& gB ) ;



	/**
	 * dispatch distance between a Polygon and a Geometry
	 */
	double distancePolygonGeometry3D( const Polygon & gA, const Geometry& gB ) ;


	/**
	 * dispatch distance between a Solid and a Geometry
	 */
	double distanceSolidGeometry3D( const Solid & gA, const Geometry& gB ) ;
	/**
	 * distance between two Solids
	 */
	double distanceSolidSolid3D( const Solid & gA, const Solid& gB ) ;


	/**
	 * dispatch distance from a collection of geometry (gA) to a Geometry (gB)
	 */
	double distanceGeometryCollectionToGeometry3D( const Geometry & gA, const Geometry& gB ) ;


	double distancePointSegment3D( const Point & p, const Point & a, const Point & b ) ;
	double distancePointTriangle3D( const Point & p, const Point& a, const Point& b, const Point& c ) ;
	double distanceSegmentSegment3D( const Point & a, const Point & b, const Point & c, const Point & d );
	double distanceSegmentTriangle3D(
		const Point & sA, const Point & sB,
		const Point & tA, const Point & tB, const Point & tC
	);
	/**
	 * distance between two Triangles
	 */
	double distanceTriangleTriangle3D( const Triangle & gA, const Triangle& gB ) ;



}//namespace algorithm
}//namespace SFCGAL

#endif
