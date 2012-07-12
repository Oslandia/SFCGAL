#include <map>

#include <SFCGAL/algorithm/intersects.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>

#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>
#include <CGAL/Boolean_set_operations_2.h>

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
#include <CGAL/AABB_segment_primitive.h>

#include <CGAL/box_intersection_d.h>

#include <SFCGAL/algorithm/triangulate.h>
#include <SFCGAL/all.h>
#include <SFCGAL/io/WktWriter.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Exact_predicates_exact_constructions_kernel ExactKernel;
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

	// TODO : replace by an iterator adaptor
	void to_triangles( const TriangulatedSurface& surf, std::list<Triangle_3>& rtri )
	{
		for ( size_t i = 0; i < surf.numTriangles(); i++ ) {
			rtri.push_back( surf.triangleN(i).toTriangle_3<Kernel>() );
		}
	}

	bool intersects3D_( const Point& pa, const Point& pb )
	{
		return pa == pb;
	}

	bool intersects3D_( const Point& pta, const LineString& ls )
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

	bool intersects3D_( const Point& pta, const Triangle& tri )
	{
		// FIXME : project on the triangle plane and call intersects2D()
		return tri.toTriangle_3<Kernel>().has_on( pta.toPoint_3<Kernel>() );
	}

	struct found_intersection_segments {};
	void segment_intersect_callback( const SegmentBox& a, const SegmentBox& b )
	{
		if ( CGAL::do_intersect( *(a.handle()), *(b.handle()))) {
			throw found_intersection_segments();
		}
	}

	bool intersects3D_( const LineString& la, const LineString& lb )
	{
	    
		// typedef std::vector<Segment_3>::const_iterator Iterator;
		// typedef CGAL::AABB_segment_primitive<Kernel,Iterator> Primitive;
		// typedef CGAL::AABB_traits<Kernel, Primitive> AABB_segment_traits;
		// typedef CGAL::AABB_tree<AABB_segment_traits> Tree;

		// std::vector<Segment_3> segsa, segsb;
		// to_segments( la, segsa );
		// to_segments( lb, segsb );

		// Tree tree( segsa.begin(), segsa.end() );

		// for ( size_t i = 0; i < segsb.size(); i++ ) {
		// 	if ( tree.do_intersect( segsb[i] ) ) {
		// 		return true;
		// 	}
		// }
		// return false;
		Segments segsa, segsb;
		to_segments( la, segsa );
		to_segments( lb, segsb );

		std::vector<SegmentBox> boxa, boxb;
		for ( Segments::const_iterator it = segsa.begin(); it != segsa.end(); ++it ) {
			boxa.push_back( SegmentBox( it->bbox(), it ));
		}
		for ( Segments::const_iterator it = segsb.begin(); it != segsb.end(); ++it ) {
			boxb.push_back( SegmentBox( it->bbox(), it ));
		}
		
		try {
			CGAL::box_intersection_d( boxa.begin(), boxa.end(),
						  boxb.begin(), boxb.end(),
						  segment_intersect_callback );
		}
		catch ( found_intersection_segments& e ) {
			return true;
		}
		return false;
	}

	bool intersects3D_( const LineString& la, const Triangle& tri )
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

	bool intersects3D_( const Triangle& tri1, const Triangle& tri2 )
	{
		return CGAL::do_intersect( tri1.toTriangle_3<Kernel>(), tri2.toTriangle_3<Kernel>() );
	}

	// accelerator for LineString x TriangulatedSurface
	bool intersects3D_( const LineString& la, const TriangulatedSurface& surf )
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
	bool intersects3D_( const Triangle& la, const TriangulatedSurface& surf )
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

	typedef std::list<Triangle_3>::const_iterator TIterator;
	typedef CGAL::AABB_triangle_primitive<Kernel,TIterator> TPrimitive;
	typedef CGAL::AABB_traits<Kernel, TPrimitive> AABB_triangle_traits;
	typedef CGAL::AABB_tree<AABB_triangle_traits> TriangleTree;

	// accelerator for TriangulatedSurface x TriangulatedSurface
	bool intersects3D_( const TriangulatedSurface& surfa, const TriangulatedSurface& surfb )
	{
		std::list<Triangle_3> tris;
		to_triangles( surfa, tris );
		// Construct an AABB Tree
		TriangleTree tree( tris.begin(), tris.end() );

		bool r = false;
		for ( size_t i = 0; i < surfb.numTriangles(); i++ ) {
			Triangle_3 request_tri = surfb.triangleN(i).toTriangle_3<Kernel>();
			try {
				r = tree.do_intersect( request_tri );
			}
			catch ( std::exception& e ) {
				std::cout << surfa.asText() << " does not have a good AABB tree" << std::endl;
				return false;
			}
			if ( r )
				return r;
		}
		return r;
	}

	typedef CGAL::Nef_polyhedron_3<ExactKernel> Nef;

	//
	// Helper function that test intersection between a point and a Nef_polyhedron
	bool intersects3D_( const Point& pt, const Nef& nef )
	{
		CGAL::Object obj = nef.locate( pt.toPoint_3<ExactKernel>() );
		typename Nef::Volume_const_handle vol;
		typename Nef::Halffacet_const_handle facet;
		typename Nef::Halfedge_const_handle edge;
		typename Nef::Vertex_const_handle vertex;
		if ( CGAL::assign(vol, obj) ) {
			typename Nef::Volume_const_iterator it;
			// If mark() is true, the volume is the one we requested
			for ( it = nef.volumes_begin(); it != nef.volumes_end(); ++it ) {
				if ( it == vol ) {
					if ( it->mark() )
						return true;
					else
						return false;
				}
			}
		}
		else if ( CGAL::assign(facet, obj) ) {
			typename Nef::Halffacet_const_iterator it;
			// If mark() is true, the volume is the one we requested
			for ( it = nef.halffacets_begin(); it != nef.halffacets_end(); ++it ) {
				if ( it == facet ) {
					if ( it->mark() )
						return true;
					else
						return false;
				}
			}
		}
		else if ( CGAL::assign(edge, obj) ) {
			typename Nef::Halfedge_const_iterator it;
			// If mark() is true, the volume is the one we requested
			for ( it = nef.halfedges_begin(); it != nef.halfedges_end(); ++it ) {
				if ( it == edge ) {
					if ( it->mark() )
						return true;
					else
						return false;
				}
			}
		}
		else if ( CGAL::assign(vertex, obj) ) {
			typename Nef::Vertex_const_iterator it;
			// If mark() is true, the volume is the one we requested
			for ( it = nef.vertices_begin(); it != nef.vertices_end(); ++it ) {
				if ( it == vertex ) {
					if ( it->mark() )
						return true;
					else
						return false;
				}
			}
		}
		return false;
	}

	bool intersects3D_( const Point& pta, const Solid& solid )
	{
	    //		Nef nef( solid.toNef_polyhedron_3<ExactKernel>());
	    //		return intersects3D_( pta, nef );
	}

	// helper function that fills a TriangleTree with triangles from a Nef_polyhedron
	void build_triangle_aabb_tree( const Nef& nef, TriangleTree& tree )
	{
		typedef CGAL::Polyhedron_3<ExactKernel> Polyhedron;
		CGAL::Polyhedron_3<ExactKernel> poly;
		// convert back to a polyhedron
		nef.convert_to_polyhedron( poly );

		BOOST_ASSERT( poly.is_pure_triangle() );

		std::list<Triangle_3> triangles(1);
		typename Polyhedron::Facet_iterator it;
		for ( it = poly.facets_begin(); it != poly.facets_end(); it++ )
		{
			Point_3 tri[3];
			typename Polyhedron::Halfedge_around_facet_circulator eit;
			size_t s = 0;
			for ( eit = it->facet_begin(); s < 3; s++, eit++ )
			{
				// build a triangle and convert from the exact kernel
				tri[s] = Point_3( CGAL::to_double(eit->vertex()->point().x()),
						  CGAL::to_double(eit->vertex()->point().y()),
						  CGAL::to_double(eit->vertex()->point().z()));
			}
			*triangles.begin() = Triangle_3( tri[0], tri[1], tri[2] );
			tree.insert( triangles.begin(), triangles.end() );
		}
	}

	bool intersects3D_( const LineString& ls, const Solid& solid )
	{
		// intersection between a linestring and a solid
		// 1. either one of the segments' point lies inside the solid
		// 2. or any of the segments intersects the solid's surface

		// 1. test segment's points

		Nef nef( solid.toNef_polyhedron_3<ExactKernel>());
		for ( size_t i = 0; i < ls.numPoints(); ++i ) {
			if ( intersects3D_( ls.pointN(i), nef )) {
				return true;
			}
		}
		
		// 2. test segments' intersection

		// AABB tree
		TriangleTree tree;
		build_triangle_aabb_tree( nef, tree );

		std::vector<Segment_3> segs;
		to_segments( ls, segs );
		for ( size_t i = 0; i < segs.size(); i++ ) {
			if ( tree.do_intersect( segs[i]) ) {
				return true;
			}
		}
		return false;
	}

	bool intersects3D_( const Triangle& tri, const Solid& solid )
	{
		// intersection between a triangle and a solid
		// 1. either one of the triangles' point lies inside the solid
		// 2. or any of the triangles intersects the solid's surface

		// 1. test triangles' points

		Nef nef( solid.toNef_polyhedron_3<ExactKernel>());
		for ( size_t i = 0; i < 3; ++i ) {
			if ( intersects3D_( tri.vertex(i), nef )) {
				return true;
			}
		}
		
		// 2. test triangles' intersection

		// AABB tree
		TriangleTree tree;
		build_triangle_aabb_tree( nef, tree );

		return tree.do_intersect( tri.toTriangle_3<Kernel>() );
	}

	// TODO: use "Box_intersection_d()" here ?
	bool intersects3D_( const TriangulatedSurface& surf, const Solid& solid )
	{
		CGAL::Polyhedron_3<ExactKernel> poly(surf.toPolyhedron_3<ExactKernel>());
		Nef nef(poly);
		Nef solid_nef( solid.toNef_polyhedron_3<ExactKernel>());
		Nef N = nef * solid_nef;
		return ! N.is_empty();
	}

	// TODO: use "Box_intersection_d()" here ?
	bool intersects3D_( const Solid& solid1, const Solid& solid2 )
	{
		Nef nef1( solid1.toNef_polyhedron_3<ExactKernel>());
		Nef nef2( solid2.toNef_polyhedron_3<ExactKernel>());
		Nef N = nef1 * nef2;
		return ! N.is_empty();
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
				return intersects3D_( pta, static_cast<const Point&>( gb ) );
			case TYPE_LINESTRING:
				return intersects3D_( pta, static_cast<const LineString&>( gb ) );
			case TYPE_TRIANGLE:
				return intersects3D_( pta, static_cast<const Triangle&>( gb ));
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_TIN:
				break;
			case TYPE_SOLID:
				return intersects3D_( pta, static_cast<const Solid&>(gb) );
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
				return intersects3D_( ls, static_cast<const LineString&>( gb ) );
			case TYPE_TRIANGLE:
				return intersects3D_( ls, static_cast<const Triangle&>( gb ));
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				return intersects3D_( ls, static_cast<const Solid&>( gb ));
			case TYPE_TIN:
				// call the proper accelerator
				return intersects3D_( ls, static_cast<const TriangulatedSurface&>( gb ));
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
				return intersects3D_( tri, static_cast<const Triangle&>( gb ));
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				return intersects3D_( tri, static_cast<const Solid&>( gb ));
			case TYPE_TIN:
				// call the proper accelerator
				return intersects3D_( tri, static_cast<const TriangulatedSurface&>( gb ));
			default:
				// symmetric call
				return intersects( gb, ga );
			} // switch( gb.geometryTypeId() )
			break;
		} // TYPE_TRIANGLE
		case TYPE_POLYGON: {
			switch ( gb.geometryTypeId() ) {
			case TYPE_POLYGON:
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
				return intersects3D_( static_cast<const TriangulatedSurface&>(ga),
						      static_cast<const TriangulatedSurface&>(gb) );
			case TYPE_SOLID:
				return intersects3D_( static_cast<const TriangulatedSurface&>(ga),
						      static_cast<const Solid&>(gb) );
			default:
				// symmetric call
				return intersects( gb, ga );
			} // switch( gb.geometryTypeId() )
			break;
		} // TYPE_TIN
		case TYPE_SOLID: {
			switch ( gb.geometryTypeId() ) {
			case TYPE_SOLID:
				return intersects3D_( static_cast<const Solid&>(ga),
						      static_cast<const Solid&>(gb) );
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
