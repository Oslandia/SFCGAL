#include <map>
#include <sstream>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/algorithm/intersects.h>


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

typedef CGAL::Point_2< SFCGAL::Kernel >              Point_2;
typedef CGAL::Point_3< SFCGAL::Kernel >              Point_3;
typedef CGAL::Segment_2< SFCGAL::Kernel >            Segment_2;
typedef CGAL::Segment_3< SFCGAL::Kernel >            Segment_3;
typedef CGAL::Triangle_2< SFCGAL::Kernel >           Triangle_2;
typedef CGAL::Triangle_3< SFCGAL::Kernel >           Triangle_3;
typedef CGAL::Polygon_2< SFCGAL::Kernel >            Polygon_2;
typedef CGAL::Polygon_with_holes_2< SFCGAL::Kernel > Polygon_with_holes_2;

//#define CACHE_TRIANGULATION

namespace SFCGAL {
namespace algorithm
{
	bool intersects_( const Point& pa, const Point& pb )
	{
		return pa.x() == pb.x() && pa.y() == pb.y();
	}

	bool intersects_( const Point& pta, const LineString& ls )
	{
		// build a CGAL::Segment for each line string element and call CGAL::has_on
		Point_2 p = pta.toPoint_2();

		for ( size_t i = 0; i < ls.numPoints() - 1; ++i ) {
			Segment_2 seg( ls.pointN(i).toPoint_2(), ls.pointN(i+1).toPoint_2() );

			if ( seg.has_on( p ) )
				return true;
		}
		return false;
	}

	bool intersects_( const Point& pta, const Triangle& tri )
	{
		CGAL::Bounded_side b = tri.toTriangle_2().bounded_side( pta.toPoint_2() );
		return b == CGAL::ON_BOUNDED_SIDE || b == CGAL::ON_BOUNDARY;
	}
    
	bool intersects_( const Triangle& tri1, const Triangle& tri2 )
	{
		return CGAL::do_intersect( tri1.toTriangle_2(), tri2.toTriangle_2() );
	}

	///
	/// intersection test using CGAL::box_intersection_d
	///
	bool intersects_bbox_d( const Geometry& ga, const Geometry& gb )
	{
		std::vector<detail::Object2Box> aboxes, bboxes;
		std::list<detail::ObjectHandle> ahandles, bhandles;

		detail::to_boxes<2>( ga, ahandles, aboxes );
		detail::to_boxes<2>( gb, bhandles, bboxes );
		
		try {
			CGAL::box_intersection_d( aboxes.begin(), aboxes.end(), 
						  bboxes.begin(), bboxes.end(),
						  detail::intersects_cb<2> );
		}
		catch ( detail::found_intersection& e ) {
			return true;
		}
		return false;
	}

	bool intersects_( const Point& pt, const Polygon& poly )
	{
		CGAL::Bounded_side b1 = CGAL::bounded_side_2( poly.exteriorRing().points_2_begin(),
							      poly.exteriorRing().points_2_end(),
							      pt.toPoint_2() );
		if ( b1 == CGAL::ON_BOUNDARY ) {
			return true;
		}
		if ( b1 == CGAL::ON_BOUNDED_SIDE ) {
			// might be in a hole
			for ( size_t i = 0; i < poly.numInteriorRings(); ++i ) {
				const LineString& ring = poly.interiorRingN(i);
				CGAL::Bounded_side b = CGAL::bounded_side_2( ring.points_2_begin(),
									     ring.points_2_end(),
									     pt.toPoint_2() );
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
		Envelope bboxa = pa.envelope();
		Envelope bboxb = pb.envelope();

		if ( Envelope::overlaps( bboxa, bboxb ) ) {
			// test intersections between each rings
			std::vector<detail::Object2Box> aboxes, bboxes;
			std::list<detail::ObjectHandle> ahandles, bhandles;

			detail::to_boxes<2>( pa.exteriorRing(), ahandles, aboxes );
			for ( size_t i = 0; i < pa.numInteriorRings(); ++i ) {
				detail::to_boxes<2>( pa.interiorRingN( i ), ahandles, aboxes );
			}
			detail::to_boxes<2>( pb.exteriorRing(), bhandles, bboxes );
			for ( size_t i = 0; i < pb.numInteriorRings(); ++i ) {
				detail::to_boxes<2>( pb.interiorRingN( i ), bhandles, bboxes );
			}
			try {
				CGAL::box_intersection_d( aboxes.begin(), aboxes.end(), 
							  bboxes.begin(), bboxes.end(),
							  detail::intersects_cb<2> );
			}
			catch ( detail::found_segment_segment_intersection& e ) {
				return true;
			}
		}

		// rings do not intersect, check if one polygon is inside another

		// if pa is inside pb
		if ( Envelope::contains( bboxb, bboxa ) )
		{
			// is pa inside one of pb's holes ?
			for ( size_t i = 0; i < pb.numInteriorRings(); ++i ) {
				const LineString& interiorRing = pb.interiorRingN( i );
				CGAL::Bounded_side b2 = CGAL::bounded_side_2( interiorRing.points_2_begin(),
									      interiorRing.points_2_end(),
									      pa.exteriorRing().startPoint().toPoint_2() );
				if ( b2 == CGAL::ON_BOUNDED_SIDE ) {
					return false;
				}
			}
			return true;
		}
		// if pb is inside pa
		if ( Envelope::contains( bboxa, bboxb ) )
		{
			// is pb inside one of pa's holes ?
			for ( size_t i = 0; i < pa.numInteriorRings(); ++i ) {
				const LineString& interiorRing = pa.interiorRingN( i );
				CGAL::Bounded_side b2 = CGAL::bounded_side_2( interiorRing.points_2_begin(),
									      interiorRing.points_2_end(),
									      pb.exteriorRing().startPoint().toPoint_2() );
				if ( b2 == CGAL::ON_BOUNDED_SIDE ) {
					return false;
				}
			}
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
			case TYPE_TRIANGULATEDSURFACE:
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
			case TYPE_TRIANGULATEDSURFACE:
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
			case TYPE_TRIANGULATEDSURFACE:
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
			case TYPE_TRIANGULATEDSURFACE:
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
			case TYPE_TRIANGULATEDSURFACE:
			case TYPE_SOLID:
				break;
			default:
				// symmetric call
				return intersects( gb, ga );
			} // switch( gb.geometryTypeId() )
			break;
		} // TYPE_POLYHEDRALSURFACE
		case TYPE_TRIANGULATEDSURFACE: {
			switch ( gb.geometryTypeId() ) {
			case TYPE_TRIANGULATEDSURFACE:
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
		if ( gb.geometryTypeId() == TYPE_TRIANGULATEDSURFACE ) {
			const TriangulatedSurface& tri = static_cast< const TriangulatedSurface& >( gb );
			for ( size_t i = 0; i < tri.numGeometries(); i++ ) {
				if ( intersects(ga, tri.geometryN(i)) )
					return true;
			}
			return false;
		}
		if ( ga.geometryTypeId() == TYPE_TRIANGULATEDSURFACE ) {
			const TriangulatedSurface& tri = static_cast< const TriangulatedSurface& >( ga );
			for ( size_t i = 0; i < tri.numGeometries(); i++ ) {
				if ( intersects(gb, tri.geometryN(i)) )
					return true;
			}
			return false;
		}

		throw std::runtime_error( "intersects() not supported on " + ga.geometryType() + " x " + gb.geometryType() );
		return false;
	}
}
}
