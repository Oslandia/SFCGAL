#include <SFCGAL/algorithm/intersects.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>

#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/triangulate.h>
//#include <SFCGAL/Kernel.h>

#include <SFCGAL/io/WktWriter.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Point_3<Kernel> Point_3;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Segment_3<Kernel> Segment_3;
typedef CGAL::Triangle_2<Kernel> Triangle_2;
typedef CGAL::Triangle_3<Kernel> Triangle_3;

namespace SFCGAL {
namespace algorithm
{
    void to_point( const Point& p, Point_2& pt )
    {
	pt = Point_2( p.x(), p.y() );
    }

    void to_point( const Point& p, Point_3& pt )
    {
	pt = Point_3( p.x(), p.y(), p.is3D() ? p.z() : 0.0 );
    }

    void to_segments( const LineString& ls, std::vector< Segment_3 >& segs )
    {
	segs.resize( ls.numPoints() - 1 );
	for ( size_t i = 1; i < ls.numPoints(); i++ ) {
	    const Point& prev = ls.pointN( i - 1 );
	    const Point& curr = ls.pointN( i );
	    segs[i-1] = Segment_3( Point_3( prev.x(), prev.y(), prev.is3D() ? prev.z() : 0.0 ),
				       Point_3( curr.x(), curr.y(), curr.is3D() ? curr.z() : 0.0 ) );
	}
    }

    void to_segments( const LineString& ls, std::vector< Segment_2 >& segs )
    {
	//	segs.resize( ls.numPoints() - 1 );
	for ( size_t i = 1; i < ls.numPoints(); i++ ) {
	    const Point& prev = ls.pointN( i - 1 );
	    const Point& curr = ls.pointN( i );
	    segs[i-1] = Segment_2( Point_2( prev.x(), prev.y() ),
				       Point_2( curr.x(), curr.y() ) );
	}
    }

    void to_triangle( const Triangle& tri, Triangle_2& rtri )
    {
	rtri = Triangle_2( Point_2( tri.vertex(0).x(), tri.vertex(0).y() ),
			   Point_2( tri.vertex(1).x(), tri.vertex(1).y() ),
			   Point_2( tri.vertex(2).x(), tri.vertex(2).y() ) );
    }

    void to_triangle( const Triangle& tri, Triangle_3& rtri )
    {
	rtri = Triangle_3( Point_3( tri.vertex(0).x(), tri.vertex(0).y(), tri.vertex(0).is3D() ? tri.vertex(0).z() : 0.0 ),
			   Point_3( tri.vertex(1).x(), tri.vertex(1).y(), tri.vertex(1).is3D() ? tri.vertex(1).z() : 0.0 ),
			   Point_3( tri.vertex(2).x(), tri.vertex(2).y(), tri.vertex(2).is3D() ? tri.vertex(2).z() : 0.0 ) );
    }

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
	Point_3 p;
	to_point( pta, p );
	std::vector< Segment_3 > segs;
	to_segments( ls, segs );
	for ( size_t i = 0; i < segs.size(); i++ ) {
	    if ( segs[i].has_on( p ) )
		return true;
	}
	return false;
    }

    bool intersects_( const Point& pta, const Triangle& tri )
    {
	Point_3 p;
	to_point( pta, p );
	Triangle_3 t;
	to_triangle( tri, t );
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
	// TODO : add shortcuts ( bbox not overlapping )
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

    bool intersects_( const Point& pta, const Solid& solid )
    {
	// TODO : cast a ray from the point to a random direction and count the number of intersections. Must be > 0
	throw std::runtime_error( "intersects_ Point, Solid not implemented" );
	return false;
    }

    bool intersects_( const LineString& la, const LineString& lb )
    {
	// Use 2D arrangments and compute_intersection_points for 2D lines ?
	// TODO Equivalent in 3D ?? => 3D plane sweep ??
	
	if ( !la.is3D() && !lb.is3D() ) {
	    typedef CGAL::Arr_segment_traits_2<Kernel> Traits_2;
	    typedef Traits_2::Curve_2 Segment_2;
	    
	    std::vector<Segment_2> segs;
	    const std::vector<Point>& pts_a = la.points();
	    const std::vector<Point>& pts_b = lb.points();
	    for ( size_t i = 1; i < pts_a.size(); i++ ) {
		Segment_2 seg( Point_2( pts_a[i-1].x(), pts_a[i-1].y() ),
			       Point_2( pts_a[i].x(), pts_a[i].y() ) );
		segs.push_back( seg );
	    }
	    for ( size_t i = 1; i < pts_b.size(); i++ ) {
		Segment_2 seg( Point_2( pts_b[i-1].x(), pts_b[i-1].y() ),
			       Point_2( pts_b[i].x(), pts_b[i].y() ) );
		segs.push_back( seg );
	    }
	    
	    std::vector<Point_2> pts;
	    CGAL::compute_intersection_points( segs.begin(), segs.end(), std::back_inserter(pts) );
	    return pts.size() > 0;
	}

	// in 3D, call do_intersects on each pair of lines
	std::vector<Segment_3> segsa, segsb;
	to_segments( la, segsa );
	to_segments( lb, segsb );

	for ( size_t i = 0; i < segsa.size(); i++ ) {
	    bool found = false;
	    for ( size_t j = 0; j < segsb.size(); j++ ) {
		if ( CGAL::do_intersect( segsa[i], segsb[j] ) ) {
		    found = true;
		    break;
		}
	    }
	    if ( found )
		return true;
	}
	return false;
    }

    bool intersects_( const LineString& la, const Triangle& tri )
    {
	std::vector<Segment_3> segs;
	Triangle_3 tri3;
	to_segments( la, segs );
	to_triangle( tri, tri3 );
	for ( size_t i = 0; i < segs.size(); i++ ) {
	    if ( CGAL::do_intersect( segs[i], tri3 ) )
		return true;
	}
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

	//
	// Double dispatch processing.
	// Not very clean.
	//
	// Use a 2-dimension arrays of function pointers ?
	// cf http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/DDJ/2004/0401/0401e/0401e.htm

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
	    case TYPE_SOLID:
		return intersects_( pta, static_cast<const Solid&>( gb ));
	    default:
		// recurse call on the symmetric
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	} // TYPE_POINT
	case TYPE_LINESTRING: {
	    const LineString& ls = static_cast<const LineString&>( ga );
	    
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_LINESTRING:
	     	return intersects_( ls, static_cast<const LineString&>( gb ) );
	    case TYPE_TRIANGLE:
	     	return intersects_( ls, static_cast<const Triangle&>( gb ));
	    // case TYPE_POLYGON:
	    // 	return intersects_( ls, static_cast<const Polygon&>( gb ));
	    // case TYPE_POLYHEDRALSURFACE:
	    // 	return intersects_( ls, static_cast<const PolyhedralSurface&>( gb ));
	    // case TYPE_TIN:
	    // 	return intersects_( ls, static_cast<const TriangulatedSurface&>( gb ));
	    // case TYPE_SOLID:
	    // 	return intersects_( ls, static_cast<const Solid&>( gb ));
	    default:
		// recurse call on the symmetric
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	} // TYPE_LINESTRING
#ifdef NOT_YET_IMPLEMENT
	case TYPE_TRIANGLE: {
	    const Triangle& tri = static_cast<const Triangle&>( ga );
	    
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_TRIANGLE:
		return intersects_( tri, static_cast<const Triangle&>( gb ));
	    case TYPE_POLYGON:
		return intersects_( tri, static_cast<const Polygon&>( gb ));
	    case TYPE_POLYHEDRALSURFACE:
		return intersects_( tri, static_cast<const PolyhedralSurface&>( gb ));
	    case TYPE_TIN:
		return intersects_( tri, static_cast<const TriangulatedSurface&>( gb ));
	    case TYPE_SOLID:
		return intersects_( tri, static_cast<const Solid&>( gb ));
	    default:
		// recurse call on the symmetric
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	} // TYPE_TRIANGLE
	case TYPE_POLYGON: {
	    const Polygon& poly = static_cast<const Polygon&>( ga );
	    
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_POLYGON:
		return intersects_( poly, static_cast<const Polygon&>( gb ));
	    case TYPE_POLYHEDRALSURFACE:
		return intersects_( poly, static_cast<const PolyhedralSurface&>( gb ));
	    case TYPE_TIN:
		return intersects_( poly, static_cast<const TriangulatedSurface&>( gb ));
	    case TYPE_SOLID:
		return intersects_( poly, static_cast<const Solid&>( gb ));
	    default:
		// recurse call on the symmetric
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	} // TYPE_POLYGON
	case TYPE_POLYHEDRALSURFACE: {
	    const PolyhedralSurface& poly = static_cast<const PolyhedralSurface&>( ga );
	    
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_POLYHEDRALSURFACE:
		return intersects_( poly, static_cast<const PolyhedralSurface&>( gb ));
	    case TYPE_TIN:
		return intersects_( poly, static_cast<const TriangulatedSurface&>( gb ));
	    case TYPE_SOLID:
		return intersects_( poly, static_cast<const Solid&>( gb ));
	    default:
		// recurse call on the symmetric
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	} // TYPE_POLYHEDRAL_SURFACE
	case TYPE_TIN: {
	    const TriangulatedSurface& poly = static_cast<const TriangulatedSurface&>( ga );
	    
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_TIN:
		return intersects_( poly, static_cast<const TriangulatedSurface&>( gb ));
	    case TYPE_SOLID:
		return intersects_( poly, static_cast<const Solid&>( gb ));
	    default:
		// recurse call on the symmetric
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	} // TYPE_TIN
	case TYPE_SOLID: {
	    const Solid& poly = static_cast<const Solid&>( ga );
	    
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_SOLID:
		return intersects_( poly, static_cast<const Solid&>( gb ));
	    default:
		// recurse call on the symmetric
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	} // TYPE_SOLID
#endif
	default:
	    break;
	} // switch ( ga.geometryTypeId() )
	return false;
    }
}
}
