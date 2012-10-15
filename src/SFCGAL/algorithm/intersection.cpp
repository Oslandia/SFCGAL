#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/detail/intersects.h>
#include <SFCGAL/algorithm/collect.h>
#include <SFCGAL/algorithm/collectionHomogenize.h>

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

//
// Intersection kernel

namespace SFCGAL {

typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Triangle_2<Kernel> Triangle_2;

namespace algorithm
{
	static std::auto_ptr<Geometry> intersection_point_x_( const Point& pt, const Geometry& gb )
	{
		if ( intersects( pt, gb )) {
			return std::auto_ptr<Geometry>(new Point(pt));
		}
		return std::auto_ptr<Geometry>(new GeometryCollection());
	}

	static std::auto_ptr<Geometry> intersection_triangles_( const Triangle& tria, const Triangle& trib )
	{
		CGAL::Object obj = CGAL::intersection( tria.toTriangle_2(), trib.toTriangle_2() );
		if (obj.empty()) {
			return std::auto_ptr<Geometry>(new GeometryCollection());
		}
		return std::auto_ptr<Geometry>(Geometry::fromCGAL( obj ));
	}

	static std::auto_ptr<Geometry> intersection_polygons_( const Polygon& polya, const Polygon& polyb )
	{
		std::list<CGAL::Polygon_with_holes_2<Kernel> > opolys;
		CGAL::Polygon_with_holes_2<Kernel> pa = polya.toPolygon_with_holes_2();
		CGAL::Polygon_with_holes_2<Kernel> pb = polyb.toPolygon_with_holes_2();
		CGAL::intersection( pa,
				    pb,
				    std::back_inserter(opolys) );

		if ( opolys.size() == 0 ) {
		    return std::auto_ptr<Geometry>( new GeometryCollection() );
		}
		if ( opolys.size() == 1 ) {
		    return std::auto_ptr<Geometry>(new Polygon( *opolys.begin() ));
		}

		MultiPolygon* mp = new MultiPolygon;
		std::list<CGAL::Polygon_with_holes_2<Kernel> >::const_iterator it;
		for ( it = opolys.begin(); it != opolys.end(); ++it ) {
			mp->addGeometry( new Polygon( *it));
		}
		return std::auto_ptr<Geometry>(mp);
	}

	///
	/// intersections involving Box_d
	///
	static std::auto_ptr<Geometry> intersection_box_d_( const Geometry& ga, const Geometry& gb )
	{
		std::vector<detail::Object2Box> aboxes, bboxes;
		std::list<detail::ObjectHandle> ahandles, bhandles;

		detail::to_boxes<2>( ga, ahandles, aboxes );
		detail::to_boxes<2>( gb, bhandles, bboxes );
		
		detail::intersection_cb<2> cb;
		CGAL::box_intersection_d( aboxes.begin(), aboxes.end(), 
					  bboxes.begin(), bboxes.end(),
					  cb );

		if ( cb.geometries->numGeometries() == 1 ) {
			// copy
			std::auto_ptr<Geometry> g(cb.geometries->geometryN(0).clone());
			delete cb.geometries;
			return g;
		}

		return collectionHomogenize( std::auto_ptr<Geometry>(cb.geometries) );
	}

	std::auto_ptr<Geometry> intersection( const Geometry& ga, const Geometry& gb )
	{
		//
		// return EMPTY if bboxes do not overlap
		if ( !Envelope::overlaps( ga.envelope(), gb.envelope() )) {
		    return std::auto_ptr<Geometry>(new GeometryCollection() );
		}

		// deal with geometry collection
		// call intersection on each geometry of the collection
		const GeometryCollection* coll;
		if ( (coll = dynamic_cast<const GeometryCollection*>( &ga )) ) {
			GeometryCollection *ret = new GeometryCollection();
			for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
				ret->addGeometry(intersection( coll->geometryN( i ), gb).release());
			}
			return std::auto_ptr<Geometry>( ret );
		}
		if ( (coll = dynamic_cast<const GeometryCollection*>( &gb )) ) {
			GeometryCollection *ret = new GeometryCollection();
			for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
				ret->addGeometry(intersection( coll->geometryN( i ), ga).release());
			}
			return std::auto_ptr<Geometry>( ret );
		}

		switch ( ga.geometryTypeId() ) {
		case TYPE_POINT:
			return intersection_point_x_( static_cast<const Point&>(ga), gb );
		case TYPE_LINESTRING:
			switch ( gb.geometryTypeId() ) {
			case TYPE_LINESTRING:
			case TYPE_TRIANGLE:
			case TYPE_TRIANGULATEDSURFACE:
				return intersection_box_d_( ga, gb );
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				// generic processing
				break;
			default:
				// symmetric call
				return intersection( gb, ga );
			}
			break;
		case TYPE_TRIANGLE:
			switch ( gb.geometryTypeId() ) {
			case TYPE_TRIANGLE:
				return intersection_triangles_( static_cast<const Triangle&>(ga), static_cast<const Triangle&>(gb) );
			case TYPE_TRIANGULATEDSURFACE:
				return intersection_box_d_( ga, gb );
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				break;
			default:
				// symmetric call
				return intersection( gb, ga );
			}
			break;
		case TYPE_POLYGON:
			switch ( gb.geometryTypeId() ) {
			case TYPE_POLYGON:
				return intersection_polygons_( static_cast<const Polygon&>(ga), static_cast<const Polygon&>(gb) );
			case TYPE_TRIANGULATEDSURFACE:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				{
					TriangulatedSurface surf;
					algorithm::triangulate( static_cast<const PolyhedralSurface&>(ga), surf);
					return intersection( ga, surf );
				} break;
			default:
				// symmetric call
				return intersection( gb, ga );
			}
		case TYPE_TRIANGULATEDSURFACE:
			switch ( gb.geometryTypeId() ) {
			case TYPE_TRIANGULATEDSURFACE:
				return intersection_box_d_( ga, gb );
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				break;
			default:
				// symmetric call
				return intersection( gb, ga );
			}
			break;
		case TYPE_POLYHEDRALSURFACE:
			{
				TriangulatedSurface surf;
				algorithm::triangulate( static_cast<const PolyhedralSurface&>(ga), surf);
				return intersection( surf, gb );
			} break;
		default:
			// TODO
			break;
		}

		if ( gb.geometryTypeId() == TYPE_POLYHEDRALSURFACE || gb.geometryTypeId() == TYPE_POLYGON ) {
			TriangulatedSurface surf;
			algorithm::triangulate( gb, surf);
			return intersection( ga, surf );
		}
		
		throw std::runtime_error( "intersection() not supported on " + ga.geometryType() + " x " + gb.geometryType() );
		// null object
		return std::auto_ptr<Geometry>();
	}
}
}
