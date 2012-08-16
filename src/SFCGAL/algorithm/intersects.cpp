#include <map>
#include <sstream>

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

#include <CGAL/box_intersection_d.h>
#include <CGAL/Bbox_2.h>

#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/tools/Log.h>
#include <SFCGAL/algorithm/detail/intersects.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

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
	bool intersects_( const Point& pa, const Point& pb )
	{
		return pa == pb;
	}

	bool intersects_( const Point& pta, const LineString& ls )
	{
		// build a CGAL::Segment for each line string element and call CGAL::has_on
		Point_2 p = pta.toPoint_2<Kernel>();

		for ( size_t i = 0; i < ls.numPoints() - 1; ++i ) {
			Segment_2 seg( ls.pointN(i).toPoint_2<Kernel>(), ls.pointN(i+1).toPoint_2<Kernel>() );

			if ( seg.has_on( p ) )
				return true;
		}
		return false;
	}

	bool intersects_( const Point& pta, const Triangle& tri )
	{
		CGAL::Bounded_side b = tri.toTriangle_2<Kernel>().bounded_side( pta.toPoint_2<Kernel>() );
		return b == CGAL::ON_BOUNDED_SIDE || b == CGAL::ON_BOUNDARY;
	}
    
	bool intersects_( const Triangle& tri1, const Triangle& tri2 )
	{
		return CGAL::do_intersect( tri1.toTriangle_2<Kernel>(), tri2.toTriangle_2<Kernel>() );
	}

	///
	/// Auxiliary function used to fill up vectors of handle and boxes for segments, triangle and triangulated surfaces
	///
	void to_boxes( const LineString& ls, std::list<detail::ObjectHandle>& handles, std::vector<detail::Object2Box>& boxes )
	{
		for ( size_t i = 0; i < ls.numPoints() - 1; ++i ) {
			handles.push_back( detail::ObjectHandle( &ls.pointN(i), &ls.pointN(i+1) ));
			boxes.push_back( detail::Object2Box( handles.back().bbox_2(), &handles.back() ));
		}
	}
	void to_boxes( const Triangle& tri, std::list<detail::ObjectHandle>& handles, std::vector<detail::Object2Box>& boxes )
	{
		handles.push_back( detail::ObjectHandle( &tri ));
		boxes.push_back( detail::Object2Box( handles.back().bbox_2(), &handles.back() ));
	}
	void to_boxes( const TriangulatedSurface& surf, std::list<detail::ObjectHandle>& handles, std::vector<detail::Object2Box>& boxes )
	{
		for ( size_t i = 0; i < surf.numTriangles(); ++i ) {
			handles.push_back( &surf.triangleN(i));
			boxes.push_back( detail::Object2Box( handles.back().bbox_2(), &handles.back() ));
		}
	}

	///
	/// Generic function
	void to_boxes_g( const Geometry& g, std::list<detail::ObjectHandle>& handles, std::vector<detail::Object2Box>& boxes )
	{
		switch ( g.geometryTypeId() ){
		case TYPE_LINESTRING:
			to_boxes( static_cast<const LineString&>(g), handles, boxes );
			break;
		case TYPE_TRIANGLE:
			to_boxes( static_cast<const Triangle&>(g), handles, boxes );
			break;
		case TYPE_TIN:
			to_boxes( static_cast<const TriangulatedSurface&>(g), handles, boxes );
			break;
		default:
			BOOST_THROW_EXCEPTION( Exception( "Trying to call to_boxes() with an incompatible type" ));
		}
	}

	///
	/// intersection test using CGAL::box_intersection_d
	///
	bool intersects_bbox_d( const Geometry& ga, const Geometry& gb )
	{
		std::vector<detail::Object2Box> aboxes, bboxes;
		std::list<detail::ObjectHandle> ahandles, bhandles;

		to_boxes_g( ga, ahandles, aboxes );
		to_boxes_g( gb, bhandles, bboxes );
		
		try {
			CGAL::box_intersection_d( aboxes.begin(), aboxes.end(), 
						  bboxes.begin(), bboxes.end(),
						  detail::intersects2_cb<Kernel> );
		}
		catch ( detail::found_intersection& e ) {
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
			std::vector<detail::Object2Box> aboxes, bboxes;
			std::list<detail::ObjectHandle> ahandles, bhandles;

			to_boxes( pa.exteriorRing(), ahandles, aboxes );
			for ( size_t i = 0; i < pa.numInteriorRings(); ++i ) {
				to_boxes( pa.interiorRingN( i ), ahandles, aboxes );
			}
			to_boxes( pb.exteriorRing(), bhandles, bboxes );
			for ( size_t i = 0; i < pb.numInteriorRings(); ++i ) {
				to_boxes( pb.interiorRingN( i ), bhandles, bboxes );
			}

			try {
				CGAL::box_intersection_d( aboxes.begin(), aboxes.end(), 
							  bboxes.begin(), bboxes.end(),
							  detail::intersects2_cb<Kernel> );
			}
			catch ( detail::found_segment_segment_intersection& e ) {
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
		SFCGAL_DEBUG( "ga: " + ga.geometryType() + " gb: " + gb.geometryType() );
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
			switch ( gb.geometryTypeId() ) {
			case TYPE_LINESTRING:
				return intersects_bbox_d( ga, gb );
			case TYPE_TRIANGLE:
				return intersects_bbox_d( ga, gb );
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				break;
			case TYPE_TIN:
				// call the proper accelerator
				return intersects_bbox_d( ga, gb );
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
				return intersects_bbox_d( ga, gb );
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
				return intersects_bbox_d( ga, gb );
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
