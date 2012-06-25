#include <map>

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

#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/all.h>
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

#define CACHE_TRIANGULATION
#define CACHE_AABBTREE

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

    // TODO : replace by an iterator adaptor
    void to_triangles( const TriangulatedSurface& surf, std::list<Triangle_3>& rtri )
    {
	for ( size_t i = 0; i < surf.numTriangles(); i++ ) {
	    rtri.push_back( surf.triangleN(i).toTriangle_3<Kernel>() );
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
	return tri.toTriangle_3<Kernel>().has_on( pta.toPoint_3<Kernel>() );
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

	Triangle_3 request_tri = tri.toTriangle_3<Kernel>();
	return tree.do_intersect( request_tri );
    }

    bool intersects_( const Triangle& tri1, const Triangle& tri2 )
    {
	return CGAL::do_intersect( tri1.toTriangle_3<Kernel>(), tri2.toTriangle_3<Kernel>() );
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

	Triangle_3 request_tri = la.toTriangle_3<Kernel>();
	bool r;
	try {
	    r = tree.do_intersect( request_tri );
	}
	catch ( std::exception& e ) {
	    std::cout << surf.asText() << " does not have a good AABB tree" << std::endl;
	    return false;
	}
	return r;
    }

    typedef std::map<const Geometry*, boost::shared_ptr<CGAL::Polygon_2<Kernel> > > PolygonCache;
    PolygonCache polygon_cache;

    bool intersects_( const Polygon& pa, const Polygon& pb )
    {
	// no polygon with holes for now ...
	BOOST_ASSERT( pa.numInteriorRings() == 0 );
	BOOST_ASSERT( pb.numInteriorRings() == 0 );

	return CGAL::do_intersect( pa.exteriorRing().toPolygon_2<Kernel>(), pb.exteriorRing().toPolygon_2<Kernel>() );
    }

    typedef std::list<Triangle_3>::const_iterator TIterator;
    typedef CGAL::AABB_triangle_primitive<Kernel,TIterator> TPrimitive;
    typedef CGAL::AABB_traits<Kernel, TPrimitive> AABB_triangle_traits;
    typedef CGAL::AABB_tree<AABB_triangle_traits> TriangleTree;

#ifdef CACHE_AABBTREE
    typedef std::map<const Geometry*, boost::shared_ptr<TriangleTree> > TriangleTreeCache;
    TriangleTreeCache triangle_tree_cache;
#endif

    // accelerator for TriangulatedSurface x TriangulatedSurface
    bool intersects_( const TriangulatedSurface& surfa, const TriangulatedSurface& surfb )
    {
	TriangleTree* ptree;
#ifdef CACHE_AABBTREE
	TriangleTreeCache::const_iterator it = triangle_tree_cache.find( &surfa );
	if ( it == triangle_tree_cache.end() ) {
#endif
	    std::list<Triangle_3> tris;
	    to_triangles( surfa, tris );
	    // Construct an AABB Tree
#ifdef CACHE_AABBTREE
	    ptree = new TriangleTree( tris.begin(), tris.end() );
	    triangle_tree_cache[&surfa].reset( ptree );
	}
	else {
	    ptree = it->second.get();
	}
#else
	TriangleTree tree( tris.begin(), tris.end() );
	ptree = &tree;
#endif

	bool r = false;
	for ( size_t i = 0; i < surfb.numTriangles(); i++ ) {
	    Triangle_3 request_tri = surfb.triangleN(i).toTriangle_3<Kernel>();
	    try {
		r = ptree->do_intersect( request_tri );
	    }
	    catch ( std::exception& e ) {
		std::cout << surfa.asText() << " has not a good AABB tree" << std::endl;
		return false;
	    }
	    if ( r )
		return r;
	}
	return r;
    }

#ifdef CACHE_TRIANGULATION
    typedef std::map<const Geometry*, boost::shared_ptr<TriangulatedSurface> > TriangulationCache;
    TriangulationCache triangulation_cache;
#endif

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
	    case TYPE_POLYHEDRALSURFACE:
	    case TYPE_TIN:
	    case TYPE_SOLID:
		break;
	    default:
		// symmetric call
		return intersects( gb, ga );
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
		break;
	    case TYPE_TIN:
		// call the proper accelerator
	     	return intersects_( ls, static_cast<const TriangulatedSurface&>( gb ));
	    default:
		// symmetric call
		return intersects( gb, ga );
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
		break;
	    case TYPE_TIN:
		// call the proper accelerator
	     	return intersects_( tri, static_cast<const TriangulatedSurface&>( gb ));
	    default:
		// symmetric call
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_TRIANGLE
	case TYPE_POLYGON: {
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_POLYGON:
		return intersects_( static_cast<const Polygon&>(ga), static_cast<const Polygon&>(gb) );
	    case TYPE_POLYHEDRALSURFACE:
	    case TYPE_TIN:
	    case TYPE_SOLID:
		break;
	    default:
		// symmetric call
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_POLYGON
	case TYPE_POLYHEDRALSURFACE: {
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_POLYHEDRALSURFACE:
	    case TYPE_TIN:
	    case TYPE_SOLID:
		break;
	    default:
		// symmetric call
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_POLYHEDRALSURFACE
	case TYPE_TIN: {
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_TIN:
		return intersects_( static_cast<const TriangulatedSurface&>(ga),
				    static_cast<const TriangulatedSurface&>(gb) );
	    case TYPE_SOLID:
		break;
	    default:
		// symmetric call
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_TIN
	case TYPE_SOLID: {
	    switch ( gb.geometryTypeId() ) {
	    case TYPE_SOLID:
		break;
	    default:
		// symmetric call
		return intersects( gb, ga );
	    } // switch( gb.geometryTypeId() )
	    break;
	} // TYPE_SOLID

	default:
	    break;
	} // switch ( ga.geometryTypeId() )

	// Generic processing of a polygon : triangulate it and apply on the triangulated surface
	if ( gb.geometryTypeId() == TYPE_POLYGON || gb.geometryTypeId() == TYPE_POLYHEDRALSURFACE ) {
#ifdef CACHE_TRIANGULATION
	    TriangulationCache::const_iterator it = triangulation_cache.find( &gb );
	    if ( it == triangulation_cache.end() ) {
		boost::shared_ptr<TriangulatedSurface> tri ( new TriangulatedSurface );
		algorithm::triangulate( gb, *tri );
		triangulation_cache[ &gb ] = tri;
		return intersects( ga, *tri );
	    }
	    // else
	    boost::shared_ptr<TriangulatedSurface> tri = it->second;
	    return intersects( ga, *tri );
#else
	    TriangulatedSurface tri;
	    algorithm::triangulate( gb, tri );
	    return intersects( ga, tri );
#endif
	}
	if ( ga.geometryTypeId() == TYPE_POLYGON || ga.geometryTypeId() == TYPE_POLYHEDRALSURFACE ) {
#ifdef CACHE_TRIANGULATION
	    TriangulationCache::const_iterator it = triangulation_cache.find( &ga );
	    if ( it == triangulation_cache.end() ) {
		boost::shared_ptr<TriangulatedSurface> tri ( new TriangulatedSurface );
		algorithm::triangulate( ga, *tri );
		triangulation_cache[ &ga ] = tri;
		return intersects( gb, *tri );
	    }
	    // else
	    boost::shared_ptr<TriangulatedSurface> tri = it->second;
	    return intersects( gb, *tri );
#else
	    TriangulatedSurface tri;
	    algorithm::triangulate( ga, tri );
	    return intersects( gb, tri );
#endif
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
