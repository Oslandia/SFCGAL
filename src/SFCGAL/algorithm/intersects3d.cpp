#include <map>

#include <SFCGAL/algorithm/intersects.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polyhedron_3.h>

#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>
#include <CGAL/Boolean_set_operations_2.h>

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_polyhedron_triangle_primitive.h>

#include <CGAL/box_intersection_d.h>

#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/all.h>
#include <SFCGAL/detail/GetPointsVisitor.h>
#include <SFCGAL/algorithm/detail/intersects.h>
#include <SFCGAL/algorithm/covers.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Point_3<Kernel> Point_3;
typedef CGAL::Segment_3<Kernel> Segment_3;
typedef CGAL::Triangle_3<Kernel> Triangle_3;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;

#define CACHE_TRIANGULATION

typedef std::vector<Segment_3> Segments;
typedef CGAL::Box_intersection_d::Box_with_handle_d<double, 3, Segments::const_iterator> SegmentBox;
	
namespace SFCGAL {
namespace algorithm
{
	static void to_segments( const LineString& ls, std::vector< Segment_3 >& segs )
	{
		segs.resize( ls.numPoints() - 1 );
		for ( size_t i = 1; i < ls.numPoints(); i++ ) {
			const Point& prev = ls.pointN( i - 1 );
			const Point& curr = ls.pointN( i );
			segs[i-1] = Segment_3( Point_3( prev.x(), prev.y(), prev.is3D() ? prev.z() : 0.0 ),
					       Point_3( curr.x(), curr.y(), curr.is3D() ? curr.z() : 0.0 ) );
		}
	}

	// ----------------------------------------------------------
	//
	// Intersections between primitives
	//
	// ----------------------------------------------------------
	
	static bool intersects3D_( const Point& pa, const Point& pb )
	{
		return pa == pb;
	}

	static bool intersects3D_( const Point& pta, const Triangle& tri )
	{
		return tri.toTriangle_3<Kernel>().has_on( pta.toPoint_3<Kernel>() );
	}

	static bool intersects3D_( const Triangle& tri1, const Triangle& tri2 )
	{
		return CGAL::do_intersect( tri1.toTriangle_3<Kernel>(), tri2.toTriangle_3<Kernel>() );
	}

	// ----------------------------------------------------------
	//
	// Intersections involving more complex objects
	// (linestrings, tin, etc.)
	//
	// ----------------------------------------------------------
	
	static bool intersects3D_( const Point& pta, const LineString& ls )
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

	///
	/// intersection test using CGAL::box_intersection_d
	///
	static bool intersects3D_bbox_d( const Geometry& ga, const Geometry& gb )
	{
		std::vector<detail::Object3Box> aboxes, bboxes;
		std::list<detail::ObjectHandle> ahandles, bhandles;

		detail::to_boxes<3>( ga, ahandles, aboxes );
		detail::to_boxes<3>( gb, bhandles, bboxes );
		
		try {
			CGAL::box_intersection_d( aboxes.begin(), aboxes.end(), 
						  bboxes.begin(), bboxes.end(),
						  detail::intersects_cb<Kernel,3> );
		}
		catch ( detail::found_intersection& e ) {
			return true;
		}
		return false;
	}

	// ----------------------------------------------------------
	//
	// Intersections involving solids
	//
	// ----------------------------------------------------------
	
	static bool intersects3D_solid_( const Geometry& ga, const Solid& solid )
	{
		// intersection between a geometry and a solid
		// 1. either one of the geometry' point lies inside the solid
		// 2. or the geometry intersects one of the solid's shells

		// 1.
		SFCGAL::detail::GetPointsVisitor get_point_visitor;
		ga.accept( get_point_visitor );
		for ( size_t i = 0; i < get_point_visitor.points.size(); ++i ) {
			const Point *pt = get_point_visitor.points[i];
			if ( covers3D( solid, *pt )) {
				return true;
			}
		}

		// 2.
		for ( size_t i = 0; i < solid.numShells(); ++i ) {
			if ( intersects3D( ga, solid.shellN( i ))) {
				return true;
			}
		}
		return false;
	}

#ifdef CACHE_TRIANGULATION
	typedef std::map<const Geometry*, boost::shared_ptr<TriangulatedSurface> > TriangulationCache;
	TriangulationCache triangulation_cache;
#endif

	bool intersects3D( const Geometry& ga, const Geometry& gb )
	{
		// deal with geometry collection
		// call intersects on each geometry of the collection
		const GeometryCollection* coll;
		if ( (coll = dynamic_cast<const GeometryCollection*>( &ga )) ) {
			for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
				if ( intersects3D( coll->geometryN( i ), gb ) ) {
					return true;
				}
			}
			return false;
		}
		if ( (coll = dynamic_cast<const GeometryCollection*>( &gb )) ) {
			for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
				if ( intersects3D( ga, coll->geometryN( i ) ) ) {
					return true;
				}
			}
			return false;
		}

		//
		// Double dispatch processing.

		switch ( ga.geometryTypeId() ) {
		case TYPE_POINT: {
			const Point& pta = static_cast<const Point&>( ga );
	    
			switch ( gb.geometryTypeId() ) {
			case TYPE_POINT:
				// two points intersects if they are the same
				return intersects3D_( pta, static_cast<const Point&>( gb ) );
			case TYPE_LINESTRING:
				return intersects3D_( pta, static_cast<const LineString&>( gb ) );
			case TYPE_TRIANGLE:
				return intersects3D_( pta, static_cast<const Triangle&>( gb ));
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
				break;
			case TYPE_TRIANGULATEDSURFACE:
				return intersects3D_bbox_d( static_cast<const Point&>(ga), static_cast<const TriangulatedSurface&>(gb));
			case TYPE_SOLID:
				break;
			default:
				// symmetric call
				return intersects3D( gb, ga );
			} // switch( gb.geometryTypeId() )
			break;
		} // TYPE_POINT
		case TYPE_LINESTRING: {
			switch ( gb.geometryTypeId() ) {
			case TYPE_LINESTRING:
				return intersects3D_bbox_d( ga, gb );
			case TYPE_TRIANGLE:
				return intersects3D_bbox_d( ga, gb );
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				break;
			case TYPE_TRIANGULATEDSURFACE:
				// call the proper accelerator
				return intersects3D_bbox_d( ga, gb );
			default:
				// symmetric call
				return intersects3D( gb, ga );
			} // switch( gb.geometryTypeId() )
			break;
		} // TYPE_LINESTRING
		case TYPE_TRIANGLE: {
			const Triangle& tri = static_cast<const Triangle&>( ga );
	    
			switch ( gb.geometryTypeId() ) {
			case TYPE_TRIANGLE:
				return intersects3D_( tri, static_cast<const Triangle&>( gb ));
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				break;
			case TYPE_TRIANGULATEDSURFACE:
				// call the proper accelerator
				return intersects3D_bbox_d( ga, gb );
			default:
				// symmetric call
				return intersects3D( gb, ga );
			} // switch( gb.geometryTypeId() )
			break;
		} // TYPE_TRIANGLE
		case TYPE_POLYGON: {
			switch ( gb.geometryTypeId() ) {
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_TRIANGULATEDSURFACE:
			case TYPE_SOLID:
				break;
			default:
				// symmetric call
				return intersects3D( gb, ga );
			} // switch( gb.geometryTypeId() )
			break;
		} // TYPE_POLYGON
		case TYPE_POLYHEDRALSURFACE: {
			switch ( gb.geometryTypeId() ) {
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_TRIANGULATEDSURFACE:
			case TYPE_SOLID:
				break;
			default:
				// symmetric call
				return intersects3D( gb, ga );
			} // switch( gb.geometryTypeId() )
			break;
		} // TYPE_POLYHEDRALSURFACE
		case TYPE_TRIANGULATEDSURFACE: {
			switch ( gb.geometryTypeId() ) {
			case TYPE_TRIANGULATEDSURFACE:
				return intersects3D_bbox_d( ga, gb );
			case TYPE_SOLID:
				break;
			default:
				// symmetric call
				return intersects3D( gb, ga );
			} // switch( gb.geometryTypeId() )
			break;
		} // TYPE_TIN
		case TYPE_SOLID: {
			switch ( gb.geometryTypeId() ) {
			case TYPE_SOLID:
				break;
			default:
				// symmetric call
				return intersects3D( gb, ga );
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
				return intersects3D( ga, *tri );
			}
			// else
			boost::shared_ptr<TriangulatedSurface> tri = it->second;
			return intersects3D( ga, *tri );
#else
			TriangulatedSurface tri;
			algorithm::triangulate( gb, tri );
			return intersects3D( ga, tri );
#endif
		}

		// Generic processing of a TIN : apply on each triangle
		else if ( gb.geometryTypeId() == TYPE_TRIANGULATEDSURFACE ) {
			const TriangulatedSurface& tri = static_cast< const TriangulatedSurface& >( gb );
			for ( size_t i = 0; i < tri.numGeometries(); i++ ) {
				if ( intersects3D(ga, tri.geometryN(i)) )
					return true;
			}
			return false;
		}

		else if ( gb.geometryTypeId() == TYPE_SOLID ) {
			return intersects3D_solid_( ga, static_cast<const Solid&>(gb) );
		}

		// generic processing of a Solid

		throw std::runtime_error( "intersects() not supported on " + ga.geometryType() + " x " + gb.geometryType() );
		return false;
	}
}
}
