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
#include <SFCGAL/algorithm/distance.h>
#include <SFCGAL/algorithm/isValid.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>
#include <SFCGAL/Exception.h>

#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_2_algorithms.h>

#include <SFCGAL/detail/transform/AffineTransform3.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/detail/GetPointsVisitor.h>


typedef SFCGAL::Kernel::Point_2                                   Point_2 ;
typedef SFCGAL::Kernel::Vector_2                                  Vector_2 ;
typedef SFCGAL::Kernel::Segment_2                                 Segment_2 ;

typedef CGAL::Polygon_2< SFCGAL::Kernel >                         Polygon_2 ;
typedef CGAL::Polygon_with_holes_2< SFCGAL::Kernel >              Polygon_with_holes_2 ;



namespace SFCGAL {
namespace algorithm {

///
///
///
double distance( const Geometry & gA, const Geometry& gB, NoValidityCheck )
{
	switch ( gA.geometryTypeId() ){
	case TYPE_POINT:
		return distancePointGeometry( gA.as< Point >(), gB ) ;
	case TYPE_LINESTRING:
		return distanceLineStringGeometry( gA.as< LineString >(), gB ) ;
	case TYPE_POLYGON:
		return distancePolygonGeometry( gA.as< Polygon >(), gB ) ;
	case TYPE_TRIANGLE:
		return distanceTriangleGeometry( gA.as< Triangle >(), gB ) ;
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_MULTIPOLYGON:
	case TYPE_MULTISOLID:
	case TYPE_GEOMETRYCOLLECTION:
	case TYPE_TRIANGULATEDSURFACE:
	case TYPE_POLYHEDRALSURFACE:
		return distanceGeometryCollectionToGeometry( gA, gB );
	case TYPE_SOLID:
        BOOST_THROW_EXCEPTION(NotImplementedException(
            ( boost::format("distance(%s,%s) is not implemented") % gA.geometryType() % gB.geometryType() ).str()
        ));
	}
    BOOST_ASSERT(false);
    return 0;
}

double distance( const Geometry & gA, const Geometry& gB )
{
	SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D(gA);
	SFCGAL_ASSERT_GEOMETRY_VALIDITY_2D(gB);
	return distance( gA, gB, NoValidityCheck() );
}

///
///
///
double distancePointGeometry( const Point & gA, const Geometry& gB )
{
	switch ( gB.geometryTypeId() ){
	case TYPE_POINT:
		return distancePointPoint( gA, gB.as< Point >() );
	case TYPE_LINESTRING:
		return distancePointLineString( gA, gB.as< LineString >() );
	case TYPE_POLYGON:
		return distancePointPolygon( gA, gB.as< Polygon >() );
	case TYPE_TRIANGLE:
		return distancePointTriangle( gA, gB.as< Triangle >() );

	//collection dispatch
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_MULTIPOLYGON:
	case TYPE_MULTISOLID:
	case TYPE_GEOMETRYCOLLECTION:
	case TYPE_TRIANGULATEDSURFACE:
	case TYPE_POLYHEDRALSURFACE:
		return distanceGeometryCollectionToGeometry( gB, gA );
	case TYPE_SOLID:
        BOOST_THROW_EXCEPTION(NotImplementedException(
            ( boost::format("distance(%s,%s) is not implemented") % gA.geometryType() % gB.geometryType() ).str()
        ));
	}
    BOOST_ASSERT(false);
    return 0;
}

///
///
///
double distancePointPoint( const Point & gA, const Point& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	return CGAL::sqrt(
		CGAL::to_double(
			CGAL::squared_distance( gA.toPoint_2(), gB.toPoint_2() )
		)
	);
}

///
///
///
double distancePointLineString( const Point & gA, const LineString& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	size_t numSegments = gB.numSegments() ;

	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < numSegments; i++ ){
		double d = distancePointSegment( gA, gB.pointN(i), gB.pointN(i+1) );
		if ( i == 0 || d < dMin ){
			dMin = d ;
		}
	}
	return dMin ;
}

///
///
///
double distancePointPolygon( const Point & gA, const Polygon& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	if ( intersects( gA , gB, NoValidityCheck() ) ){
		return 0.0 ;
	}

	double dMin = 0.0 ;
	//check if the point is in the polygon
	for ( size_t i = 0; i < gB.numRings(); i++ ){
		double d = distancePointLineString( gA, gB.ringN(i) );
		if ( i == 0 || d < dMin ){
			dMin = d ;
		}
	}
	return dMin ;
}

///
///
///
double distancePointTriangle( const Point & gA, const Triangle& gB )
{
	return distancePointPolygon( gA, gB.toPolygon() );
}



///
///
///
double distanceLineStringGeometry( const LineString & gA, const Geometry& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	switch ( gB.geometryTypeId() ){
	case TYPE_POINT:
		return distancePointLineString( gB.as< Point >(), gA ); //symetric
	case TYPE_LINESTRING:
		return distanceLineStringLineString( gA, gB.as< LineString >() );
	case TYPE_POLYGON:
		return distanceLineStringPolygon( gA, gB.as< Polygon >() );
	case TYPE_TRIANGLE:
		return distanceLineStringTriangle( gA, gB.as< Triangle >() );

	//collection dispatch
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_MULTIPOLYGON:
	case TYPE_MULTISOLID:
	case TYPE_GEOMETRYCOLLECTION:
	case TYPE_TRIANGULATEDSURFACE:
	case TYPE_POLYHEDRALSURFACE:
		return distanceGeometryCollectionToGeometry( gB, gA );
	case TYPE_SOLID:
        BOOST_THROW_EXCEPTION(NotImplementedException(
            ( boost::format("distance(%s,%s) is not implemented") % gA.geometryType() % gB.geometryType() ).str()
        ));
	}
    BOOST_ASSERT(false);
    return 0;
}

///
///
///
double distanceLineStringLineString( const LineString & gA, const LineString& gB )
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
				distanceSegmentSegment(
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
double distanceLineStringPolygon( const LineString & gA, const Polygon& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	if ( intersects( gA, gB, NoValidityCheck() ) ){
		return 0.0 ;
	}
	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gB.numRings(); i++ ){
		double d = distanceLineStringLineString( gA, gB.ringN(i) );
		if ( d < dMin )
			dMin = d ;
	}
	return dMin ;
}


///
///
///
double distanceLineStringTriangle( const LineString & gA, const Triangle& gB )
{
	return distanceLineStringPolygon( gA, gB.toPolygon() );
}





///
///
///
double distancePolygonGeometry( const Polygon & gA, const Geometry& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	switch ( gB.geometryTypeId() ){
	case TYPE_POINT:
		return distancePointPolygon( gB.as< Point >(), gA );           //symetric
	case TYPE_LINESTRING:
		return distanceLineStringPolygon( gB.as< LineString >(), gA ); //symetric
	case TYPE_POLYGON:
		return distancePolygonPolygon( gA, gB.as< Polygon >() );
	case TYPE_TRIANGLE:
		return distancePolygonTriangle( gA, gB.as< Triangle >() );

	//collection dispatch
	case TYPE_MULTIPOINT:
	case TYPE_MULTILINESTRING:
	case TYPE_MULTIPOLYGON:
	case TYPE_MULTISOLID:
	case TYPE_GEOMETRYCOLLECTION:
	case TYPE_TRIANGULATEDSURFACE:
	case TYPE_POLYHEDRALSURFACE:
		return distanceGeometryCollectionToGeometry( gB, gA );
	case TYPE_SOLID:
        BOOST_THROW_EXCEPTION(NotImplementedException(
            ( boost::format("distance(%s,%s) is not implemented") % gA.geometryType() % gB.geometryType() ).str()
        ));
	}
    BOOST_ASSERT(false);
    return 0;
}

///
///
///
double distancePolygonPolygon( const Polygon & gA, const Polygon& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

	if ( intersects( gA, gB, NoValidityCheck() ) ){
		return 0.0 ;
	}
	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gA.numRings(); i++ ){
		for ( size_t j = 0; j < gB.numRings(); j++ ){
			double d = distanceLineStringLineString( gA.ringN(i), gB.ringN(j) );
			if ( d < dMin )
				dMin = d ;
		}
	}
	return dMin ;
}

///
///
///
double distancePolygonTriangle( const Polygon & gA, const Triangle& gB )
{
	return distancePolygonPolygon( gA, gB.toPolygon() );
}

///
///
///
double distanceTriangleGeometry( const Triangle & gA, const Geometry& gB )
{
	return distancePolygonGeometry( gA.toPolygon(), gB );
}

struct Circle {
    double radius;
    CGAL::Vector_2<Kernel> center;
};

const Circle boundingCircle( const Geometry & geom )
{
    BOOST_ASSERT( ! geom.isEmpty() );
    using namespace SFCGAL::detail;
    GetPointsVisitor v;
    const_cast< Geometry & >(geom).accept( v );

    BOOST_ASSERT( v.points.size() );

    typedef CGAL::Vector_2< Kernel > Vector_2 ;

    const GetPointsVisitor::const_iterator end = v.points.end();

    // centroid
    Vector_2 c(0,0);
    int numPoint = 0;
    for ( GetPointsVisitor::const_iterator x = v.points.begin(); x != end; ++x ) {
        c = c + (*x)->toVector_2() ;
        ++numPoint;
    }
    BOOST_ASSERT( numPoint );
    c = c / numPoint;

    // farest point from centroid
    Vector_2 f = c ;
    Kernel::FT maxDistanceSq = 0;
    for ( GetPointsVisitor::const_iterator x = v.points.begin(); x != end; ++x ) {
        const Vector_2 cx = (*x)->toVector_2() - c ;
        const Kernel::FT dSq = cx * cx ;
        if ( dSq > maxDistanceSq ) {
            f = (*x)->toVector_2() ;
            maxDistanceSq = dSq ;
        }
    }

    const Circle circle = { std::sqrt( CGAL::to_double( maxDistanceSq ) ), c };
    return circle;
}

///
///
///
double distanceGeometryCollectionToGeometry( const Geometry & gA, const Geometry& gB )
{
	if ( gA.isEmpty() || gB.isEmpty() ){
		return std::numeric_limits< double >::infinity() ;
	}

    // if bounding spheres (BS) of gB and gAi don't intersect and
    // if the closest point of BS(gAj) is further than the farest 
    // point of BS(gAi) there is no need to compute the distance(gAj, gB)
    // since it will be greater than distance(gAi, gB)
    //
    // The aim is not to find the minimal bounding sphere, but a good enought sphere than
    // encloses all points
    std::set<size_t> noTest;
    if (1)
    {
        std::vector<Circle> bcA;
        for ( size_t i = 0; i < gA.numGeometries(); i++ ){
            bcA.push_back( boundingCircle(gA.geometryN(i)) );
        }
        Circle bcB( boundingCircle( gB ) );
        std::vector<size_t> noIntersect;
        for ( size_t i = 0; i < gA.numGeometries(); i++ ){
            const double l2 = CGAL::to_double( (bcB.center - bcA[i].center).squared_length() );
            if (std::pow(bcB.radius + bcA[i].radius, 2) < l2 ) {
                noIntersect.push_back( i );
            }
        }

        for ( size_t i = 0; i < noIntersect.size(); i++ ) {
            const double li = std::sqrt( CGAL::to_double( (bcA[i].center - bcB.center).squared_length() ) );
            for ( size_t j = i; j < noIntersect.size(); j++ ) {
                const double lj = std::sqrt( CGAL::to_double( (bcA[j].center - bcB.center).squared_length() ) );
                if ( li + bcA[i].radius < lj - bcA[j].radius  ) noTest.insert( j );
                else if ( lj + bcA[j].radius < li - bcA[i].radius  ) noTest.insert( i );
            }
        }
        //if (!noTest.empty()) std::cout << "pruning " << noTest.size() << "/" << gA.numGeometries() << "\n";
    }




	double dMin = std::numeric_limits< double >::infinity() ;
	for ( size_t i = 0; i < gA.numGeometries(); i++ ){
        if ( noTest.end() != noTest.find(i) ) continue;
		dMin = std::min( dMin, distance( gA.geometryN(i), gB ) );
	}
	return dMin ;
}




//--- private



///
///
///
double distancePointSegment( const Point & p, const Point & a, const Point & b )
{
	// empty already checked
	BOOST_ASSERT( ! p.isEmpty() );
	BOOST_ASSERT( ! a.isEmpty() );
	BOOST_ASSERT( ! b.isEmpty() );

	return CGAL::sqrt(
		CGAL::to_double(
			CGAL::squared_distance(
				p.toPoint_2(),
				Segment_2(
					a.toPoint_2(),
					b.toPoint_2()
				)
			)
		)
	);
}

///
///
///
double distanceSegmentSegment( const Point & a, const Point & b, const Point & c, const Point & d )
{
	// empty already checked
	BOOST_ASSERT( ! a.isEmpty() );
	BOOST_ASSERT( ! b.isEmpty() );
	BOOST_ASSERT( ! c.isEmpty() );
	BOOST_ASSERT( ! d.isEmpty() );

	return CGAL::sqrt( CGAL::to_double(
		CGAL::squared_distance(
			CGAL::Segment_2< Kernel >( a.toPoint_2(), b.toPoint_2() ),
			CGAL::Segment_2< Kernel >( c.toPoint_2(), d.toPoint_2() )
		)
	) );
}


}//namespace algorithm
}//namespace SFCGAL
