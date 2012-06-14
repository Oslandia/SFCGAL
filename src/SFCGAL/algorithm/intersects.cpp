#include <SFCGAL/algorithm/intersects.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>
#include <CGAL/Boolean_set_operations_2.h>

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
#include <CGAL/AABB_segment_primitive.h>

#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/triangulate.h>

#include <SFCGAL/io/WktWriter.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
//typedef CGAL::Cartesian< double > Kernel;
typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Point_3<Kernel> Point_3;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Segment_3<Kernel> Segment_3;
typedef CGAL::Triangle_2<Kernel> Triangle_2;
typedef CGAL::Triangle_3<Kernel> Triangle_3;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;

namespace SFCGAL {
namespace algorithm
{
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
	segs.resize( ls.numPoints() - 1 );
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

    // TODO : replace by an iterator adaptor
    void to_triangles( const TriangulatedSurface& surf, std::list<Triangle_3>& rtri )
    {
	for ( size_t i = 0; i < surf.numTriangles(); i++ ) {
	    Triangle_3 t;
	    to_triangle( surf.triangleN( i ), t );
	    rtri.push_back( t );
	}
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
	Point_3 p = pta.toPoint_3<Kernel>();

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
	Triangle_3 t;
	to_triangle( tri, t );
	return t.has_on( pta.toPoint_3<Kernel>() );
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

	typedef std::vector<Segment_3>::const_iterator Iterator;
	typedef CGAL::AABB_segment_primitive<Kernel,Iterator> Primitive;
	typedef CGAL::AABB_traits<Kernel, Primitive> AABB_segment_traits;
	typedef CGAL::AABB_tree<AABB_segment_traits> Tree;

	std::vector<Segment_3> segsa, segsb;
	to_segments( la, segsa );
	to_segments( lb, segsb );

	Tree tree( segsa.begin(), segsa.end() );

	for ( size_t i = 0; i < segsb.size(); i++ ) {
	    if ( tree.do_intersect( segsb[i] ) ) {
		return true;
	    }
	}
	return false;
    }

    bool intersects_( const LineString& la, const Triangle& tri )
    {
	typedef std::vector<Segment_3>::const_iterator Iterator;
	typedef CGAL::AABB_segment_primitive<Kernel,Iterator> Primitive;
	typedef CGAL::AABB_traits<Kernel, Primitive> AABB_segment_traits;
	typedef CGAL::AABB_tree<AABB_segment_traits> Tree;

	std::vector<Segment_3> segsa;
	to_segments( la, segsa );

	Tree tree( segsa.begin(), segsa.end() );

	Triangle_3 request_tri;
	to_triangle( tri, request_tri );
	return tree.do_intersect( request_tri );
    }

    bool intersects_( const Triangle& tri1, const Triangle& tri2 )
    {
	Triangle_3 ctri1, ctri2;
	to_triangle( tri1, ctri1 );
	to_triangle( tri2, ctri2 );
	return CGAL::do_intersect( ctri1, ctri2 );
    }

    // accelerator for LineString x TriangulatedSurface
    bool intersects_( const LineString& la, const TriangulatedSurface& surf )
    {
	typedef std::list<Triangle_3>::const_iterator Iterator;
	typedef CGAL::AABB_triangle_primitive<Kernel,Iterator> Primitive;
	typedef CGAL::AABB_traits<Kernel, Primitive> AABB_triangle_traits;
	typedef CGAL::AABB_tree<AABB_triangle_traits> Tree;

	std::list<Triangle_3> tris;
	to_triangles( surf, tris );
	// Construct an AABB Tree
	Tree tree( tris.begin(), tris.end() );

	std::vector<Segment_3> segs;
	to_segments( la, segs );
	for ( size_t i = 0; i < segs.size(); i++ ) {
	    if ( tree.do_intersect( segs[i] ) ) {
		return true;
	    }
	}
	return false;
    }

    // accelerator for Triangle x TriangulatedSurface
    bool intersects_( const Triangle& la, const TriangulatedSurface& surf )
    {
	typedef std::list<Triangle_3>::const_iterator Iterator;
	typedef CGAL::AABB_triangle_primitive<Kernel,Iterator> Primitive;
	typedef CGAL::AABB_traits<Kernel, Primitive> AABB_triangle_traits;
	typedef CGAL::AABB_tree<AABB_triangle_traits> Tree;

	std::list<Triangle_3> tris;
	to_triangles( surf, tris );
	// Construct an AABB Tree
	Tree tree( tris.begin(), tris.end() );

	Triangle_3 request_tri;
	to_triangle( la, request_tri );
	return tree.do_intersect( request_tri );
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

	bool generic_processing = false;

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
	    case TYPE_POLYHEDRALSURFACE:
	    case TYPE_TIN:
	    case TYPE_SOLID:
		generic_processing = true;
		break;
	    default:
		break;
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_POINT
	case TYPE_LINESTRING: {
	    const LineString& ls = static_cast<const LineString&>( ga );
	    
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_LINESTRING:
	     	return intersects_( ls, static_cast<const LineString&>( gb ) );
	    case TYPE_TRIANGLE:
	     	return intersects_( ls, static_cast<const Triangle&>( gb ));
	    case TYPE_POLYGON:
	    case TYPE_POLYHEDRALSURFACE:
	    case TYPE_SOLID:
		generic_processing = true;
		break;
	    case TYPE_TIN:
		// call the proper accelerator
	     	return intersects_( ls, static_cast<const TriangulatedSurface&>( gb ));
	    default:
		break;
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_LINESTRING
	case TYPE_TRIANGLE: {
	    const Triangle& tri = static_cast<const Triangle&>( ga );
	    
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_TRIANGLE:
		return intersects_( tri, static_cast<const Triangle&>( gb ));
	    case TYPE_POLYGON:
	    case TYPE_POLYHEDRALSURFACE:
	    case TYPE_SOLID:
		generic_processing = true;
		break;
	    case TYPE_TIN:
		// call the proper accelerator
	     	return intersects_( tri, static_cast<const TriangulatedSurface&>( gb ));
	    default:
		break;
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_TRIANGLE
	case TYPE_POLYGON: {
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_POLYGON:
	    case TYPE_POLYHEDRALSURFACE:
	    case TYPE_TIN:
	    case TYPE_SOLID:
		generic_processing = true;
		break;
	    default:
		break;
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_POLYGON
	case TYPE_POLYHEDRALSURFACE: {
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_POLYHEDRALSURFACE:
	    case TYPE_TIN:
	    case TYPE_SOLID:
		generic_processing = true;
		break;
	    default:
		break;
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_POLYHEDRALSURFACE
	case TYPE_TIN: {
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_TIN:
	    case TYPE_SOLID:
		generic_processing = true;
		break;
	    default:
		break;
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_TIN
	case TYPE_SOLID: {
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_SOLID:
		generic_processing = true;
		break;
	    default:
		break;
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_SOLID

	default:
	    break;
	} // switch ( ga.geometryTypeId() )

	// symmetric arguments
	if ( !generic_processing ) {
	    return intersects( gb, ga );
	}

	// Generic processing of a polygon : triangulate it and apply on the triangulated surface
	if ( gb.geometryTypeId() == TYPE_POLYGON || gb.geometryTypeId() == TYPE_POLYHEDRALSURFACE ) {
	    TriangulatedSurface tri;
	    algorithm::triangulate( gb, tri );
	    return intersects( ga, tri );
	}
	if ( ga.geometryTypeId() == TYPE_POLYGON || gb.geometryTypeId() == TYPE_POLYHEDRALSURFACE ) {
	    TriangulatedSurface tri;
	    algorithm::triangulate( ga, tri );
	    return intersects( gb, tri );
	}

	// Generic processing of a TIN : apply on each triangle
	if ( gb.geometryTypeId() == TYPE_TIN ) {
	    const TriangulatedSurface& tri = static_cast< const TriangulatedSurface& >( gb );
	    for ( size_t i = 0; i < tri.numTriangles(); i++ ) {
		if ( intersects(ga, tri.triangleN(i)) )
		    return true;
	    }
	    return false;
	}
	if ( ga.geometryTypeId() == TYPE_TIN ) {
	    const TriangulatedSurface& tri = static_cast< const TriangulatedSurface& >( ga );
	    for ( size_t i = 0; i < tri.numTriangles(); i++ ) {
		if ( intersects(gb, tri.triangleN(i)) )
		    return true;
	    }
	    return false;
	}

	if ( (ga.geometryTypeId() == TYPE_SOLID) || (gb.geometryTypeId() == TYPE_SOLID) ) {
	    throw std::runtime_error( "intersects() not yet implemented on " + ga.geometryType() + " x " + gb.geometryType() );
	}
	throw std::runtime_error( "intersects() not supported on " + ga.geometryType() + " x " + gb.geometryType() );
	return false;
    }
}
}
