#include <SFCGAL/algorithm/intersects.h>

#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/Kernel.h>

#include <SFCGAL/io/WktWriter.h>

namespace SFCGAL {
namespace algorithm
{
    bool intersects_( const Point& pa, const Point& pb );
    bool intersects_( const Point& pta, const LineString& ls );
    bool intersects_( const Point& pta, const Triangle& tri );
    bool intersects_( const Point& pta, const Polygon& poly );
    bool intersects_( const Point& pta, const TriangulatedSurface& tri );
    bool intersects_( const Point& pta, const PolyhedralSurface& tri );

    bool intersects_( const Point& pa, const Point& pb )
    {
	return pa == pb;
    }

    bool intersects_( const Point& pta, const LineString& ls )
    {
	// build a CGAL::Segment for each line string element and call CGAL::has_on
	for ( size_t i = 1; i < ls.numPoints(); i++ ) {
	    const Point& prev = ls.pointN( i - 1 );
	    const Point& cur = ls.pointN( i );
	    Point_3 p( pta.x(), pta.y(), pta.z() );
	    Segment_3 seg( Point_3( prev.x(), prev.y(), prev.z() ),
			   Point_3( cur.x(), cur.y(), cur.z() ) );
	    if ( seg.has_on( p ) )
		return true;
	}
	return false;
    }

    bool intersects_( const Point& pta, const Triangle& tri )
    {
	Point_3 p( pta.x(), pta.y(), pta.z() );
	Point_3 pa( tri.vertex(0).x(), tri.vertex(0).y(), tri.vertex(0).z() );
	Point_3 pb( tri.vertex(1).x(), tri.vertex(1).y(), tri.vertex(1).z() );
	Point_3 pc( tri.vertex(2).x(), tri.vertex(2).y(), tri.vertex(2).z() );
	Triangle_3 t( pa, pb, pc );
	return t.has_on( p );
    }

    bool intersects_( const Point& pta, const Polygon& poly )
    {
	// Compute a 2D triangulation of the polygon
	TriangulatedSurface tri;
	algorithm::triangulate( poly, tri );
	return intersects_( pta, tri );
    }

    bool intersects_( const Point& pta, const TriangulatedSurface& tri )
    {
	for ( size_t i = 0; i < tri.numTriangles(); i++ ) {
	    const Triangle& t = tri.triangleN( i );
	    // call the intersection test between a point and a triangle
	    if ( intersects_( pta, t )) {
		return true;
	    }
	}
	return false;
    }

    bool intersects_( const Point& pta, const PolyhedralSurface& polyh )
    {
	// TODO : triangulate
	throw std::runtime_error( "intersects_ Point, PolyhedralSurface not implemented" );
	return false;
    }

    bool intersects( const Geometry& ga, const Geometry& gb )
    {
	// deal with geometry collection
	// call intersects on each geometry of the collection
	const GeometryCollection* coll;
	if ( (coll = dynamic_cast<const GeometryCollection*>( &ga )) ) {
	    for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
		if ( intersects( coll->geometryN( i ), gb ) ) {
		    return true;
		}
	    }
	    return false;
	}
	if ( (coll = dynamic_cast<const GeometryCollection*>( &gb )) ) {
	    for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
		if ( intersects( ga, coll->geometryN( i ) ) ) {
		    return true;
		}
	    }
	    return false;
	}

	switch ( ga.geometryTypeId() ) {
	case TYPE_POINT: {
	    const Point& pta = static_cast<const Point&>( ga );
	    
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_POINT:
		// two points intersects if they are the same
		return intersects_( pta, static_cast<const Point&>( gb ) );
	    case TYPE_LINESTRING:
		return intersects_( pta, static_cast<const LineString&>( gb ) );
	    case TYPE_TRIANGLE:
		return intersects_( pta, static_cast<const Triangle&>( gb ));
	    case TYPE_POLYGON:
		return intersects_( pta, static_cast<const Polygon&>( gb ));
	    case TYPE_POLYHEDRALSURFACE:
		return intersects_( pta, static_cast<const PolyhedralSurface&>( gb ));
	    case TYPE_TIN:
		return intersects_( pta, static_cast<const TriangulatedSurface&>( gb ));
	    default:
		// recurse call on the symmetric
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	} // TYPE_POINT
	default:
	    break;
	} // switch ( ga.geometryTypeId() )
	return false;
    }
}
}
