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

	bool intersects_( const Point& pa, const Point& pb )
	{
		return pa == pb;
	}

	bool intersects_( const Point& pta, const LineString& ls )
	{
		// TODO : use point location with arrangements ?

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
		// compute an arrangement of the triangle and the isolated point
		// they intersect if the face where the point lies in is the triangle's face
		typedef CGAL::Arr_segment_traits_2<ExactKernel> Traits_2;
		typedef Traits_2::Curve_2 Segment_2;
		typedef CGAL::Arrangement_2<Traits_2> Arrangement_2;
		typedef CGAL::Arr_naive_point_location<Arrangement_2> PointLocation;

		Arrangement_2 arr;
		for ( size_t i = 0; i < 3 ; i++ ) {
			CGAL::Point_2<ExactKernel> p1( tri.vertex(i).toPoint_2<ExactKernel>() );
			CGAL::Point_2<ExactKernel> p2( tri.vertex( (i+1)%3 ).toPoint_2<ExactKernel>() );
			Segment_2 seg( p1, p2 );
			std::string msg = (boost::format( "Insert (%1%,%2%)-(%3%,%4%)") % p1.x() % p1.y() % p2.x() % p2.y()).str();
			Logger::get()->log( Logger::Debug, msg );
			CGAL::insert( arr, seg );
		}

		Arrangement_2::Face_const_handle f;
		Arrangement_2::Vertex_const_handle v;
		Arrangement_2::Halfedge_const_handle e;

		PointLocation pl(arr);
		CGAL::Object obj = pl.locate( pta.toPoint_2<ExactKernel>() );
		if ( CGAL::assign( f, obj ) ) {
			// it intersects if it is the bounded face
			return !f->is_unbounded();
		}
		else if ( CGAL::assign( e, obj )) {
			// point on an edge
			for ( size_t i = 0; i < 3; ++i ) {
				// point on an edge ?
				CGAL::Point_2<ExactKernel> p = tri.vertex(i).toPoint_2<ExactKernel>();
				if ( p == e->source()->point() || p == e->target()->point() ) {
					return true;
				}
			}
		}
		else if ( CGAL::assign( v, obj )) {
			for ( size_t i = 0; i < 3; ++i ) {
				// point on vertex ?
				if ( pta == tri.vertex(i) ) {
					return true;
				}
			}
		}
		return false;
	}
    
	typedef std::vector<Segment_2> Segments;
	typedef Segments::const_iterator EdgeIterator;
	typedef CGAL::Box_intersection_d::Box_with_handle_d<double, 2, EdgeIterator> SegmentEdgeBox;
	
	struct intersects_segment_segment_tag {};
	void intersects_segment_segment_cb( const SegmentEdgeBox& a, const SegmentEdgeBox& b )
	{
		if ( CGAL::do_intersect( *(a.handle()), *(b.handle()) )) {
			throw intersects_segment_segment_tag();
		}
	}
	bool intersects_( const LineString& la, const LineString& lb )
	{
		std::vector<SegmentEdgeBox> aboxes, bboxes;
		Segments segsa, segsb;
		to_segments( la, segsa );
		to_segments( lb, segsb );
		
		for ( Segments::const_iterator it = segsa.begin(); it != segsa.end(); ++it ) {
			aboxes.push_back( SegmentEdgeBox( it->bbox(), it ));
		}
		for ( Segments::const_iterator it = segsb.begin(); it != segsb.end(); ++it ) {
			bboxes.push_back( SegmentEdgeBox( it->bbox(), it ));
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
		typedef CGAL::Arr_segment_traits_2<Kernel> Traits_2;
		typedef Traits_2::Curve_2 Segment_2;
		typedef CGAL::Arrangement_2<Traits_2> Arrangement_2;

		Arrangement_2 arr;
		for ( size_t i = 0; i < la.points().size() ; i++ ) {
			Segment_2 seg( la.pointN(i).toPoint_2<Kernel>(), la.pointN( (i+1)%la.points().size() ).toPoint_2<Kernel>() );
			CGAL::insert( arr, seg );
		}

		// count number of vertices, faces and holes before inserting the triangle
		size_t n_vertices = arr.number_of_vertices();
		size_t n_faces = arr.number_of_vertices();
		size_t n_holes = 0;
		Arrangement_2::Face_iterator it;
		for ( it = arr.faces_begin(); it != arr.faces_end(); it++ ) {
			n_holes += std::distance( it->holes_begin(), it->holes_end() );
		}

		CGAL::Triangle_2<Kernel> ctri = tri.toTriangle_2<Kernel>();
		for ( size_t i = 0; i < 3; i++ ) {
			Segment_2 seg ( ctri[i], ctri[(i+1)%3] );
			CGAL::insert( arr, seg );
		}
		size_t new_n_holes = 0;
		for ( it = arr.faces_begin(); it != arr.faces_end(); it++ ) {
			new_n_holes += std::distance( it->holes_begin(), it->holes_end() );
		}

		// they intersect if there are new vertices
		// or new faces
		// or new holes
		// in the arrangement
		return arr.number_of_vertices() > (n_vertices + 3) ||
			arr.number_of_faces() > (n_faces + 1) ||
			new_n_holes > n_holes;
	}

	bool intersects_( const Triangle& tri1, const Triangle& tri2 )
	{
		return CGAL::do_intersect( tri1.toTriangle_2<Kernel>(), tri2.toTriangle_2<Kernel>() );
	}

	// accelerator for LineString x TriangulatedSurface
	bool intersects_( const LineString& la, const TriangulatedSurface& surf )
	{
		typedef CGAL::Arr_segment_traits_2<Kernel> Traits_2;
		typedef Traits_2::Curve_2 Segment_2;
		typedef CGAL::Arrangement_2<Traits_2> Arrangement_2;

		Arrangement_2 arr;
		for ( size_t i = 0; i < la.points().size() ; i++ ) {
			Segment_2 seg( la.pointN(i).toPoint_2<Kernel>(), la.pointN( (i+1)%la.points().size() ).toPoint_2<Kernel>() );
			CGAL::insert( arr, seg );
		}

		// count number of vertices, faces and holes before inserting the triangle
		size_t n_vertices = arr.number_of_vertices();
		size_t n_faces = arr.number_of_vertices();
		size_t n_holes = 0;
		Arrangement_2::Face_iterator it;
		for ( it = arr.faces_begin(); it != arr.faces_end(); it++ ) {
			n_holes += std::distance( it->holes_begin(), it->holes_end() );
		}

		for ( size_t j = 0; j < surf.numTriangles(); j++ ) {
			CGAL::Triangle_2<Kernel> ctri = surf.triangleN(j).toTriangle_2<Kernel>();
			for ( size_t i = 0; i < 3; i++ ) {
				Segment_2 seg ( ctri[i], ctri[(i+1)%3] );
				CGAL::insert( arr, seg );
			}
		}
		size_t new_n_holes = 0;
		for ( it = arr.faces_begin(); it != arr.faces_end(); it++ ) {
			new_n_holes += std::distance( it->holes_begin(), it->holes_end() );
		}

		// they intersect if there are new vertices
		// or new faces
		// or new holes
		// in the arrangement
		return arr.number_of_vertices() > (n_vertices + 3) ||
			arr.number_of_faces() > (n_faces + 1) ||
			new_n_holes > n_holes;
	}

	CGAL::Bbox_2 get_bbox( const Polygon& poly )
	{
		double minf = std::numeric_limits<double>::infinity();
		double xmin = +minf;
		double ymin = +minf;
		double xmax = -minf;
		double ymax = -minf;
		for ( size_t i = 0; i < poly.exteriorRing().numPoints(); ++i ) {
			const Point& pt = poly.exteriorRing().pointN(i);
			if ( pt.x() > xmax )
				xmax = pt.x();
			if ( pt.x() < xmin )
				xmin = pt.x();
			if ( pt.y() > ymax )
				ymax = pt.y();
			if ( pt.y() < ymin )
				ymin = pt.y();
		}
		return CGAL::Bbox_2( xmin, ymin, xmax, ymax );
	}

	bool intersects_( const Polygon& pa, const Polygon& pb )
	{
		// first compute the bbox of the two polygons
		CGAL::Bbox_2 gbboxa = get_bbox( pa );
		CGAL::Bbox_2 gbboxb = get_bbox( pb );
		if ( CGAL::do_overlap( gbboxa, gbboxb ) ) {
			// test intersections between each rings
			std::vector<SegmentEdgeBox> aboxes, bboxes;
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
				aboxes.push_back( SegmentEdgeBox( it->bbox(), it ));
			}
			for ( Segments::const_iterator it = segsb.begin(); it != segsb.end(); ++it ) {
				bboxes.push_back( SegmentEdgeBox( it->bbox(), it ));
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
		if ( CGAL::bounded_side_2( pb.exteriorRing().points_2_begin<Kernel>(),
					   pb.exteriorRing().points_2_end<Kernel>(),
					   pa.exteriorRing().startPoint().toPoint_2<Kernel>() )
		     == CGAL::ON_BOUNDED_SIDE ) {
		    return true;
		}
		// is pb inside pa ?
		if ( CGAL::bounded_side_2( pa.exteriorRing().points_2_begin<Kernel>(),
					   pa.exteriorRing().points_2_end<Kernel>(),
					   pb.exteriorRing().startPoint().toPoint_2<Kernel>() )
		     == CGAL::ON_BOUNDED_SIDE ) {
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
		Logger::get()->log( Logger::Debug, "ga: " + ga.geometryType() + " gb: " + gb.geometryType() );
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
