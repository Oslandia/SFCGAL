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
#include <SFCGAL/algorithm/distance3d.h>

#include <SFCGAL/all.h>
#include <SFCGAL/Exception.h>
#include <SFCGAL/detail/tools/Log.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <SFCGAL/detail/transform/AffineTransform3.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/triangulate/triangulatePolygon.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel ;
typedef Kernel::FT                                        squared_distance_t ;

typedef Kernel::Point_3                                   Point_3 ;
typedef Kernel::Vector_3                                  Vector_3 ;
typedef Kernel::Segment_3                                 Segment_3 ;
typedef Kernel::Triangle_3                                Triangle_3 ;
typedef Kernel::Plane_3                                   Plane_3 ;

namespace SFCGAL {
namespace algorithm {

///
///
///
double distance3D( const Geometry & gA, const Geometry& gB )
{
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D(gA);
    SFCGAL_ASSERT_GEOMETRY_VALIDITY_3D(gB);
	//SFCGAL_DEBUG( boost::format("dispatch distance3D(%s,%s)") % gA.asText() % gB.asText() );

	switch ( gA.geometryTypeId() ){
	case TYPE_POINT:
		return distancePointGeometry3D( gA.as< Point >(), gB );
	case TYPE_LINESTRING:
		return distanceLineStringGeometry3D( gA.as< LineString >(), gB );
	case TYPE_POLYGON:
		//SFCGAL_DEBUG( boost::format("gA is a Polygon (%s)") % gA.geometryTypeId() );
		return distancePolygonGeometry3D( gA.as< Polygon >(), gB );
	case TYPE_TRIANGLE:
		//SFCGAL_DEBUG( boost::format("gA is a Triangle (%s)") % gA.geometryTypeId() );
		return distanceTriangleGeometry3D( gA.as< Triangle >(), gB );
	case TYPE_SOLID:
		return distanceSolidGeometry3D( gA.as< Solid >(), gB );

	//collection dispatch
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_MULTIPOLYGON:
	case TYPE_MULTISOLID:
	case TYPE_GEOMETRYCOLLECTION:
	case TYPE_TRIANGULATEDSURFACE:
	case TYPE_POLYHEDRALSURFACE:
		return distanceGeometryCollectionToGeometry3D( gB, gA );
	}

	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("distance3D(%s,%s) is not implemented") % gA.geometryType() % gB.geometryType() ).str()
	));
}

///
///
///
double distancePointGeometry3D( const Point & gA, const Geometry& gB )
{
	//SFCGAL_DEBUG( boost::format("dispatch distancePointGeometry3D(%s,%s)") % gA.asText() % gB.asText() );

	switch ( gB.geometryTypeId() ){
	case TYPE_POINT:
		return distancePointPoint3D( gA, gB.as< Point >() );
	case TYPE_LINESTRING:
		return distancePointLineString3D( gA, gB.as< LineString >() );
	case TYPE_TRIANGLE:
		return distancePointTriangle3D( gA, gB.as< Triangle >() );
	case TYPE_POLYGON:
		return distancePointPolygon3D( gA, gB.as< Polygon >() );
	case TYPE_SOLID:
		return distancePointSolid3D( gA, gB.as< Solid >() );

	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_MULTIPOLYGON:
	case TYPE_MULTISOLID:
	case TYPE_GEOMETRYCOLLECTION:
	case TYPE_TRIANGULATEDSURFACE:
	case TYPE_POLYHEDRALSURFACE:
		return distanceGeometryCollectionToGeometry3D( gB, gA );
	}

	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("distance3D(%s,%s) is not implemented") % gA.geometryType() % gB.geometryType() ).str()
	));
}

///
///
///
double distancePointPoint3D( const Point & gA, const Point& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	return CGAL::sqrt(
		CGAL::to_double(
			CGAL::squared_distance( gA.toPoint_3(), gB.toPoint_3() )
		)
	);
}

///
///
///
double distancePointLineString3D( const Point & gA, const LineString& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gB.numSegments(); i++ ){
		dMin = std::min( dMin, distancePointSegment3D( gA, gB.pointN(i), gB.pointN(i+1) ) );
	}
	return dMin ;
}


///
///
///
double distancePointTriangle3D( const Point & gA, const Triangle& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}
	return distancePointTriangle3D( gA, gB.vertex(0), gB.vertex(1), gB.vertex(2) );
}


///
///
///
double distancePointPolygon3D( const Point & gA, const Polygon & gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}
	TriangulatedSurface triangulateSurfaceB ;
	triangulate::triangulatePolygon3D( gB, triangulateSurfaceB ) ;
	return distanceGeometryCollectionToGeometry3D( triangulateSurfaceB, gA );
}



///
///
///
double distancePointSolid3D( const Point & gA, const Solid& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	if ( intersects3D(gA,gB) ){
		return 0.0 ;
	}

	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gB.numShells(); i++ ){
		dMin = std::min( dMin, distanceGeometryCollectionToGeometry3D( gB.shellN(i), gA ) );
	}
	return dMin ;
}





///
///
///
double distanceLineStringGeometry3D( const LineString & gA, const Geometry& gB )
{
	//SFCGAL_DEBUG( boost::format("dispatch distanceLineStringGeometry3D(%s,%s)") % gA.asText() % gB.asText() );

	switch ( gB.geometryTypeId() ){
		case TYPE_POINT:
			return distancePointLineString3D( gB.as< Point >(), gA ); //symetric
		case TYPE_LINESTRING:
			return distanceLineStringLineString3D( gA, gB.as< LineString >() );
		case TYPE_TRIANGLE:
			return distanceLineStringTriangle3D( gA, gB.as< Triangle >() );
		case TYPE_POLYGON:
			return distanceLineStringPolygon3D( gA, gB.as< Polygon >() );
		case TYPE_SOLID:
			return distanceLineStringSolid3D( gA, gB.as< Solid >() );

		case TYPE_MULTIPOINT:
		case TYPE_MULTILINESTRING:
		case TYPE_MULTIPOLYGON:
		case TYPE_MULTISOLID:
		case TYPE_GEOMETRYCOLLECTION:
		case TYPE_TRIANGULATEDSURFACE:
		case TYPE_POLYHEDRALSURFACE:
			return distanceGeometryCollectionToGeometry3D( gB, gA );
	}

	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("distance3D(%s,%s) is not implemented") % gA.geometryType() % gB.geometryType() ).str()
	));
}

///
///
///
double distanceLineStringLineString3D( const LineString & gA, const LineString& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	size_t nsA = gA.numSegments() ;
	size_t nsB = gB.numSegments() ;

	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < nsA; i++ ){
		for ( size_t j = 0; j < nsB; j++ ){
			dMin = std::min(
				dMin,
				distanceSegmentSegment3D(
					gA.pointN(i), gA.pointN(i+1),
					gB.pointN(j), gB.pointN(j+1)
				)
			) ;
		}
	}
	return dMin ;
}

///
///
///
double distanceLineStringTriangle3D( const LineString & gA, const Triangle& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	double dMin = std::numeric_limits< double >::infinity() ;

	const Point & tA = gB.vertex(0) ;
	const Point & tB = gB.vertex(1) ;
	const Point & tC = gB.vertex(2) ;

	for ( size_t i = 0; i < gA.numSegments(); i++ ){
		dMin = std::min( dMin, distanceSegmentTriangle3D( gA.pointN(i), gA.pointN(i+1), tA, tB, tC ) );
	}
	return dMin ;
}

///
///
///
double distanceLineStringPolygon3D( const LineString & gA, const Polygon & gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}
	TriangulatedSurface triangulateSurfaceB ;
	triangulate::triangulatePolygon3D( gB, triangulateSurfaceB ) ;
	return distanceGeometryCollectionToGeometry3D( triangulateSurfaceB, gA );
}

///
///
///
double distanceLineStringSolid3D( const LineString & gA, const Solid & gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}
	if ( intersects( gA, gB ) ){
		return 0.0 ;
	}
	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gB.numShells(); i++ ){
		dMin = std::min( dMin, gB.shellN(i).distance3D(gA) );
	}
	return dMin ;
}


///
///
///
double distanceTriangleGeometry3D( const Triangle & gA, const Geometry& gB )
{
	//SFCGAL_DEBUG( boost::format("dispatch distanceTriangleGeometry3D(%s,%s)") % gA.asText() % gB.asText() );

	switch ( gB.geometryTypeId() ){
		case TYPE_POINT:
			return distancePointTriangle3D( gB.as< Point >(), gA ); //symetric
		case TYPE_LINESTRING:
			return distanceLineStringTriangle3D( gB.as< LineString >(), gA ); //symetric
		case TYPE_TRIANGLE:
			return distanceTriangleTriangle3D( gA, gB.as< Triangle >() );
		case TYPE_POLYGON:
			return distancePolygonGeometry3D( gB.as< Polygon >(), gA );
		case TYPE_SOLID:
			return distanceTriangleSolid3D( gA, gB.as< Solid >() );

		case TYPE_MULTIPOINT:
		case TYPE_MULTILINESTRING:
		case TYPE_MULTIPOLYGON:
		case TYPE_MULTISOLID:
		case TYPE_GEOMETRYCOLLECTION:
		case TYPE_TRIANGULATEDSURFACE:
		case TYPE_POLYHEDRALSURFACE:
			return distanceGeometryCollectionToGeometry3D( gB, gA );
	}

	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("distance3D(%s,%s) is not implemented") % gA.geometryType() % gB.geometryType() ).str()
	));
}

///
///
///
double distanceTriangleSolid3D( const Triangle & gA, const Solid& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}
	if ( intersects( gA, gB ) ){
		return 0.0 ;
	}
	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gB.numShells(); i++ ){
		dMin = std::min( dMin, gB.shellN(i).distance3D(gA) );
	}
	return dMin ;
}



///
///
///
double distancePolygonGeometry3D( const Polygon & gA, const Geometry& gB )
{
	//SFCGAL_DEBUG( boost::format("dispatch distancePolygonGeometry3D(%s,%s)") % gA.asText() % gB.asText() );

	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	TriangulatedSurface triangulateSurfaceA ;
	triangulate::triangulatePolygon3D( gA, triangulateSurfaceA ) ;
	return distanceGeometryCollectionToGeometry3D( triangulateSurfaceA, gB );
}


///
///
///
double distanceSolidGeometry3D( const Solid & gA, const Geometry& gB )
{
	//SFCGAL_DEBUG( boost::format("dispatch distanceSolidGeometry3D(%s,%s)") % gA.asText() % gB.asText() );

	switch ( gB.geometryTypeId() ){
		case TYPE_POINT:
			return distancePointSolid3D( gB.as< Point >(), gA ); //symetric
		case TYPE_LINESTRING:
			return distanceLineStringSolid3D( gB.as< LineString >(), gA ); //symetric
		case TYPE_TRIANGLE:
			return distanceTriangleSolid3D( gB.as< Triangle >(), gA ); //symetric
		case TYPE_POLYGON:
			return distancePolygonGeometry3D( gB.as< Polygon >(), gA ); //symetric
		case TYPE_SOLID:
			return distanceSolidSolid3D( gA, gB.as< Solid >() );

		case TYPE_MULTIPOINT:
		case TYPE_MULTILINESTRING:
		case TYPE_MULTIPOLYGON:
		case TYPE_MULTISOLID:
		case TYPE_GEOMETRYCOLLECTION:
		case TYPE_TRIANGULATEDSURFACE:
		case TYPE_POLYHEDRALSURFACE:
			return distanceGeometryCollectionToGeometry3D( gB, gA );
	}

	BOOST_THROW_EXCEPTION(Exception(
		( boost::format("distance3D(%s,%s) is not implemented") % gA.geometryType() % gB.geometryType() ).str()
	));
}

///
///
///
double distanceSolidSolid3D( const Solid & gA, const Solid& gB )
{
	//SFCGAL_DEBUG( boost::format("dispatch distancePolygonGeometry3D(%s,%s)") % gA.asText() % gB.asText() );

	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	if ( intersects( gA, gB ) ){
		return 0.0 ;
	}

	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gA.numShells(); i++ ){
		for ( size_t j = 0; j < gB.numShells(); j++ ){
			dMin = std::min( dMin, gA.shellN(i).distance3D( gB.shellN(j) ) );
		}
	}
	return dMin ;
}


///
///
///
double distanceGeometryCollectionToGeometry3D( const Geometry & gA, const Geometry& gB )
{
	//SFCGAL_DEBUG( boost::format("dispatch distanceGeometryCollectionToGeometry3D(%s,%s)") % gA.asText() % gB.asText() );

	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gA.numGeometries(); i++ ){
		dMin = std::min( dMin, distance3D( gA.geometryN(i), gB ) );
	}
	return dMin ;
}


///
///
///
double distancePointSegment3D( const Point & p, const Point & a, const Point & b )
{
	// empty already checked
	BOOST_ASSERT( ! p.isEmpty() );
	BOOST_ASSERT( ! a.isEmpty() );
	BOOST_ASSERT( ! b.isEmpty() );

	return CGAL::sqrt(
		CGAL::to_double(
			CGAL::squared_distance(
				p.toPoint_3(),
				Segment_3(
					a.toPoint_3(),
					b.toPoint_3()
				)
			)
		)
	);
}

/*
 * missing in CGAL?
 */
squared_distance_t squaredDistancePointTriangle3D(
	const Point_3 & p,
	const Triangle_3& abc
)
{
	Point_3 a = abc.vertex(0);
	Point_3 b = abc.vertex(1);
	Point_3 c = abc.vertex(2);

	/*
	 * project P on ABC plane as projP.
	 */
	Point_3 projP = Plane_3( a, b, c ).projection( p );

	squared_distance_t dMin ;
	if ( abc.has_on( projP ) ){
		// Is projP is in the triangle, return distance from P to its projection
		// on the plane
		dMin = CGAL::squared_distance( p, projP ) ;
	}else{
		// Else, the distance is the minimum from P to triangle sides
		dMin = CGAL::squared_distance( p, Segment_3(a,b) ) ;
		dMin = std::min( dMin, CGAL::squared_distance( p, Segment_3(b,c) ) );
		dMin = std::min( dMin, CGAL::squared_distance( p, Segment_3(c,a) ) );
	}

	return dMin ;
}

///
///
///
double distancePointTriangle3D( const Point & p_, const Point& a_, const Point& b_, const Point& c_ )
{
	// empty already checked
	BOOST_ASSERT( ! p_.isEmpty() );
	BOOST_ASSERT( ! a_.isEmpty() );
	BOOST_ASSERT( ! b_.isEmpty() );
	BOOST_ASSERT( ! c_.isEmpty() );

	Point_3 p = p_.toPoint_3();
	Triangle_3 abc( a_.toPoint_3(), b_.toPoint_3(), c_.toPoint_3() ) ;

	squared_distance_t dMin = squaredDistancePointTriangle3D( p, abc );
	return CGAL::sqrt( CGAL::to_double( dMin ) ) ;
}



///
///
///
double distanceSegmentSegment3D( const Point & a, const Point & b, const Point & c, const Point & d )
{
	// empty already checked
	BOOST_ASSERT( ! a.isEmpty() );
	BOOST_ASSERT( ! b.isEmpty() );
	BOOST_ASSERT( ! c.isEmpty() );
	BOOST_ASSERT( ! d.isEmpty() );

	return CGAL::sqrt( CGAL::to_double(
		CGAL::squared_distance(
			CGAL::Segment_3< Kernel >( a.toPoint_3(), b.toPoint_3() ),
			CGAL::Segment_3< Kernel >( c.toPoint_3(), d.toPoint_3() )
		)
	) );
}



squared_distance_t squaredDistanceSegmentTriangle3D(
	const Segment_3& sAB,
	const Triangle_3 & tABC
)
{
	typedef Kernel::FT squared_distance_t ;

	/*
	 * If [sAsB] intersects the triangle (tA,tB,tC), distance is 0.0
	 */
	if ( ! CGAL::intersection( sAB, tABC ).empty() ){
		return 0.0 ;
	}

	/*
	 * else, distance is the min of the following values :
	 * - distance from sA to the Triangle
	 * - distance from sB to the Triangle
	 * - distance from sAB to the side of the Triangles
	 */
	squared_distance_t dMin = squaredDistancePointTriangle3D( sAB.vertex(0), tABC );
	dMin = std::min( dMin, squaredDistancePointTriangle3D( sAB.vertex(1), tABC ) );

	for ( int i = 0; i < 3; i++ ){
		dMin = std::min( dMin, CGAL::squared_distance( sAB,
			Segment_3( tABC.vertex(i), tABC.vertex(i+1) ) )
		) ;
	}

	return dMin ;
}


///
///
///
double distanceSegmentTriangle3D( const Point & sA_, const Point & sB_,
	const Point & tA_, const Point & tB_, const Point & tC_
)
{
	// empty already checked
	BOOST_ASSERT( ! sA_.isEmpty() );
	BOOST_ASSERT( ! sB_.isEmpty() );
	BOOST_ASSERT( ! tA_.isEmpty() );
	BOOST_ASSERT( ! tB_.isEmpty() );
	BOOST_ASSERT( ! tC_.isEmpty() );

	Point_3 sA = sA_.toPoint_3();
	Point_3 sB = sB_.toPoint_3();
	Segment_3 sAB( sA, sB );

	Point_3 tA = tA_.toPoint_3();
	Point_3 tB = tB_.toPoint_3();
	Point_3 tC = tC_.toPoint_3();
	Triangle_3 tABC( tA, tB, tC );

	squared_distance_t dMin = squaredDistanceSegmentTriangle3D( sAB, tABC );
	return CGAL::sqrt( CGAL::to_double( dMin ) ) ;
}

/*
 * missing in CGAL?
 */
squared_distance_t squaredDistanceTriangleTriangle3D(
	const Triangle_3 & triangleA,
	const Triangle_3 & triangleB
)
{
	if ( ! CGAL::intersection( triangleA, triangleB ).empty() ){
		return squared_distance_t(0);
	}

	/*
	 * min of distance from A segments to B triangle and B segments to A triangle
	 */

	squared_distance_t dMin = squaredDistanceSegmentTriangle3D(
		Segment_3( triangleA.vertex(0), triangleA.vertex(1) ),
		triangleB
	);
	dMin = std::min( dMin, squaredDistanceSegmentTriangle3D(
		Segment_3( triangleA.vertex(1), triangleA.vertex(2) ),
		triangleB
	) );
	dMin = std::min( dMin, squaredDistanceSegmentTriangle3D(
		Segment_3( triangleA.vertex(2), triangleA.vertex(0) ),
		triangleB
	) );

	dMin = std::min( dMin, squaredDistanceSegmentTriangle3D(
		Segment_3( triangleB.vertex(0), triangleB.vertex(1) ),
		triangleA
	) );
	dMin = std::min( dMin, squaredDistanceSegmentTriangle3D(
		Segment_3( triangleB.vertex(1), triangleB.vertex(2) ),
		triangleA
	) );
	dMin = std::min( dMin, squaredDistanceSegmentTriangle3D(
		Segment_3( triangleB.vertex(2), triangleB.vertex(0) ),
		triangleA
	) );

	return dMin ;
}


///
///
///
double distanceTriangleTriangle3D( const Triangle & gA, const Triangle& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	Triangle_3 triangleA = gA.toTriangle_3() ;
	Triangle_3 triangleB = gB.toTriangle_3() ;

	squared_distance_t dMin = squaredDistanceTriangleTriangle3D( triangleA, triangleB );
	return CGAL::sqrt( CGAL::to_double( dMin ) ) ;
}



}//namespace algorithm
}//namespace SFCGAL
