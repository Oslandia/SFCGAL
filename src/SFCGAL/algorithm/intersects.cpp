#include <map>
#include <sstream>

#include <SFCGAL/algorithm/intersects.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
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
#include <CGAL/Arr_naive_point_location.h>

#include <CGAL/box_intersection_d.h>
#include <CGAL/Bbox_2.h>

#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/tools/Log.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Exact_predicates_exact_constructions_kernel ExactKernel;

typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Point_3<Kernel> Point_3;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Segment_3<Kernel> Segment_3;
typedef CGAL::Triangle_2<Kernel> Triangle_2;
typedef CGAL::Triangle_3<Kernel> Triangle_3;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;

//#define CACHE_TRIANGULATION

namespace SFCGAL {
namespace algorithm
{
	void to_segments( const LineString& ls, std::vector< Segment_2 >& segs )
	{
		for ( size_t i = 1; i < ls.numPoints(); i++ ) {
			const Point& prev = ls.pointN( i - 1 );
			const Point& curr = ls.pointN( i );
			segs.push_back(Segment_2( Point_2( prev.x(), prev.y() ),
						  Point_2( curr.x(), curr.y() ) ));
		}
	}

	typedef std::vector<Segment_2> Segments;
	typedef const Segment_2* SegmentIterator;
	typedef CGAL::Box_intersection_d::Box_with_handle_d<double, 2, SegmentIterator> SegmentBox;
	
	///
	/// Box_d_intersection callback for linestring / linestring intersection test
	///
	struct intersects_segment_segment_tag {};
	void intersects_segment_segment_cb( const SegmentBox& a, const SegmentBox& b )
	{
		if ( CGAL::do_intersect( *(a.handle()), *(b.handle()) )) {
			throw intersects_segment_segment_tag();
		}
	}

	///
	/// Auxiliary function used to test intersection between a Segment_2 and a Triangle_2
	///
	bool intersects_( const Segment_2& segment, const Triangle_2& tri )
	{
		// A segment intersects a triangle if
		// - its start point is inside the triangle
		// - or its end point is inside the triangle
		// - or it intersects one of the triangle's edge
		
		CGAL::Bounded_side b1 = tri.bounded_side( segment.source() );
		CGAL::Bounded_side b2 = tri.bounded_side( segment.target() );
		if ( b1 == CGAL::ON_BOUNDED_SIDE || b1 == CGAL::ON_BOUNDARY ) {
			return true;
		}
		if ( b2 == CGAL::ON_BOUNDED_SIDE || b2 == CGAL::ON_BOUNDARY ) {
			return true;
		}
		
		for ( size_t i = 0; i < 3; ++i ) {
			Segment_2 seg( tri.vertex(i), tri.vertex((i+1)%3) );
			if ( CGAL::do_intersect( seg, segment )) {
				return true;
			}
		}
		return false;
	}

	///
	/// Box_d_intersection callback for linestring / triangle intersection test
	///
	struct intersects_segment_triangle_tag {};
	struct intersects_segment_triangle
	{
		Triangle_2 tri;
		intersects_segment_triangle( const Triangle_2 tri ) : tri(tri) {}

		void operator() ( const SegmentBox& a, SegmentBox& b )
		{
			const Segment_2* segment = a.handle();
			if ( intersects_( *segment, tri )) {
				throw intersects_segment_triangle_tag();
			}
		}
	};

	///
	/// Box_d_intersection callback for linestring / triangulatedsurface intersection test
	///
	struct intersects_segment_tin_tag {};
	struct intersects_segment_tin
	{
		const std::vector<Triangle>& tris;
		const std::map<const Segment_2*, size_t> segment_map;
		intersects_segment_tin( const std::vector<Triangle>& tris, const std::map<const Segment_2*, size_t>& segment_map ) :
			tris(tris), segment_map(segment_map) {}

		void operator() ( const SegmentBox& a, SegmentBox& b )
		{
			const Segment_2* segment = a.handle();
			std::map<const Segment_2*, size_t>::const_iterator it = segment_map.find( b.handle() );
			const Triangle& tri = tris[ it->second ];
			Triangle_2 tri2( tri.toTriangle_2<Kernel>() );
			if ( intersects_( *segment, tri2 )) {
				throw intersects_segment_tin_tag();
			}
		}
	};

	bool intersects_( const Point& pa, const Point& pb )
	{
		return pa == pb;
	}

	bool intersects_( const Point& pta, const LineString& ls )
	{
		// build a CGAL::Segment for each line string element and call CGAL::has_on
		Point_2 p = pta.toPoint_2<Kernel>();

		std::vector< Segment_2 > segs;
		to_segments( ls, segs );
		for ( size_t i = 0; i < segs.size(); i++ ) {
			if ( segs[i].has_on( p ) )
				return true;
		}
		return false;
	}

	bool intersects_( const Point& pta, const Triangle& tri )
	{
		CGAL::Bounded_side b = tri.toTriangle_2<Kernel>().bounded_side( pta.toPoint_2<Kernel>() );
		return b == CGAL::ON_BOUNDED_SIDE || b == CGAL::ON_BOUNDARY;
	}
    
	bool intersects_( const LineString& la, const LineString& lb )
	{
		std::vector<SegmentBox> aboxes, bboxes;
		Segments segsa, segsb;
		to_segments( la, segsa );
		to_segments( lb, segsb );
		
		for ( Segments::const_iterator it = segsa.begin(); it != segsa.end(); ++it ) {
			aboxes.push_back( SegmentBox( it->bbox(), &*(it) ));
		}
		for ( Segments::const_iterator it = segsb.begin(); it != segsb.end(); ++it ) {
			bboxes.push_back( SegmentBox( it->bbox(), &*(it) ));
		}
		
		try {
			CGAL::box_intersection_d( aboxes.begin(), aboxes.end(), 
						  bboxes.begin(), bboxes.end(),
						  intersects_segment_segment_cb );
		}
		catch ( intersects_segment_segment_tag& e ) {
			return true;
		}
		return false;
	}

	bool intersects_( const LineString& la, const Triangle& tri )
	{
		std::vector<SegmentBox> boxes;
		Segments segs;
		to_segments( la, segs );

		for ( Segments::const_iterator it = segs.begin(); it != segs.end(); ++it ) {
			boxes.push_back( SegmentBox( it->bbox(), &*(it) ));
		}

		SegmentBox tboxes[3];
		Triangle_2 tri2 ( tri.toTriangle_2<Kernel>() );
		Segment_2 trisegs[3];
		for ( size_t i = 0; i < 3; ++i ) {
			trisegs[i] = Segment_2( tri2.vertex(i), tri2.vertex((i+1)%3) );
			tboxes[i] = SegmentBox( trisegs[i].bbox(), &trisegs[i] );
		}

		try {
			CGAL::box_intersection_d( boxes.begin(), boxes.end(),
						  tboxes, tboxes + 3,
						  intersects_segment_triangle( tri2 ) );
		}
		catch ( intersects_segment_triangle_tag& e ) {
			return true;
		}
		return false;
	}

	bool intersects_( const Triangle& tri1, const Triangle& tri2 )
	{
		return CGAL::do_intersect( tri1.toTriangle_2<Kernel>(), tri2.toTriangle_2<Kernel>() );
	}

	// accelerator for LineString x TriangulatedSurface
	bool intersects_( const LineString& la, const TriangulatedSurface& surf )
	{
		std::vector<SegmentBox> boxes;
		Segments segs;
		to_segments( la, segs );

		for ( Segments::const_iterator it = segs.begin(); it != segs.end(); ++it ) {
			boxes.push_back( SegmentBox( it->bbox(), &*(it) ));
		}

		// maps segment to a triangle index
		std::map< const Segment_2*, size_t > segmentMap;
		std::vector<Segment_2> tinsegs;
		std::vector<SegmentBox> tboxes;

		for ( size_t i = 0; i < surf.numTriangles(); ++i ) {
			Triangle_2 tri2 ( surf.triangleN(i).toTriangle_2<Kernel>() );
			for ( size_t j = 0; j < 3; ++j ) {
				tinsegs.push_back( Segment_2( tri2.vertex(j), tri2.vertex((j+1)%3) ) );
				segmentMap[&tinsegs.back()] = i;
				tboxes.push_back( SegmentBox( tinsegs.back().bbox(), &tinsegs.back() ));
			}
		}

		try {
			CGAL::box_intersection_d( boxes.begin(), boxes.end(),
						  tboxes.begin(), tboxes.end(),
						  intersects_segment_tin( surf.triangles(), segmentMap ) );
		}
		catch ( intersects_segment_tin_tag& e ) {
			return true;
		}
		return false;
	}

	bool intersects_( const Point& pt, const Polygon& poly )
	{
		CGAL::Bounded_side b1 = CGAL::bounded_side_2( poly.exteriorRing().points_2_begin<Kernel>(),
							      poly.exteriorRing().points_2_end<Kernel>(),
							      pt.toPoint_2<Kernel>() );
		if ( b1 == CGAL::ON_BOUNDARY ) {
			return true;
		}
		if ( b1 == CGAL::ON_BOUNDED_SIDE ) {
			// might be in a hole
			for ( size_t i = 0; i < poly.numInteriorRings(); ++i ) {
				const LineString& ring = poly.interiorRingN(i);
				CGAL::Bounded_side b = CGAL::bounded_side_2( ring.points_2_begin<Kernel>(),
									     ring.points_2_end<Kernel>(),
									     pt.toPoint_2<Kernel>() );
				if ( b == CGAL::ON_BOUNDED_SIDE ) {
					return false;
				}
			}
		}
		else {
			return false;
		}
		return true;
	}

	bool intersects_( const Polygon& pa, const Polygon& pb )
	{
		// first compute the bbox of the two polygons
		CGAL::Bbox_2 gbboxa = pa.envelope().toBbox_2();
		CGAL::Bbox_2 gbboxb = pb.envelope().toBbox_2();
		if ( CGAL::do_overlap( gbboxa, gbboxb ) ) {
			// test intersections between each rings
			std::vector<SegmentBox> aboxes, bboxes;
			Segments segsa, segsb;
			to_segments( pa.exteriorRing(), segsa );
			for ( size_t i = 0; i < pa.numInteriorRings(); ++i ) {
				to_segments( pa.interiorRingN( i ), segsa );
			}
			to_segments( pb.exteriorRing(), segsb );
			for ( size_t i = 0; i < pb.numInteriorRings(); ++i ) {
				to_segments( pb.interiorRingN( i ), segsb );
			}
			
			for ( Segments::const_iterator it = segsa.begin(); it != segsa.end(); ++it ) {
				aboxes.push_back( SegmentBox( it->bbox(), &*(it) ));
			}
			for ( Segments::const_iterator it = segsb.begin(); it != segsb.end(); ++it ) {
				bboxes.push_back( SegmentBox( it->bbox(), &*(it) ));
			}
			
			try {
				CGAL::box_intersection_d( aboxes.begin(), aboxes.end(), 
							  bboxes.begin(), bboxes.end(),
							  intersects_segment_segment_cb );
			}
			catch ( intersects_segment_segment_tag& e ) {
				return true;
			}
		}
		// rings do not intersect, check if one polygon is inside another

		// is pa inside pb ?
		CGAL::Bounded_side b1 = CGAL::bounded_side_2( pb.exteriorRing().points_2_begin<Kernel>(),
							      pb.exteriorRing().points_2_end<Kernel>(),
							      pa.exteriorRing().startPoint().toPoint_2<Kernel>() );
		if ( b1 == CGAL::ON_BOUNDED_SIDE ) {
		    return true;
		}
		// is pb inside pa ?
		CGAL::Bounded_side b2 = CGAL::bounded_side_2( pa.exteriorRing().points_2_begin<Kernel>(),
							      pa.exteriorRing().points_2_end<Kernel>(),
							      pb.exteriorRing().startPoint().toPoint_2<Kernel>() );
		if ( b2 == CGAL::ON_BOUNDED_SIDE ) {
		    return true;
		}
		return false;
	}

#ifdef CACHE_TRIANGULATION
	typedef std::map<const Geometry*, boost::shared_ptr<TriangulatedSurface> > TriangulationCache;
	TriangulationCache triangulation_cache;
#endif

	bool intersects( const Geometry& ga, const Geometry& gb )
	{
		LOG_DEBUG( "ga: " << ga.geometryType() << " gb: " << gb.geometryType() );

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
			case TYPE_TIN:
				break;
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

		throw std::runtime_error( "intersects() not supported on " + ga.geometryType() + " x " + gb.geometryType() );
		return false;
	}
}
}
