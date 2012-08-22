#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/detail/intersects.h>
#include <SFCGAL/algorithm/collect.h>

#include <CGAL/Triangle_3_Triangle_3_intersection.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <CGAL/Point_inside_polyhedron_3.h>
#include <CGAL/intersection_of_Polyhedra_3.h>
#include <CGAL/intersection_of_Polyhedra_3_refinement_visitor.h>

//
// Intersection kernel
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Exact_predicates_inexact_constructions_kernel ExactKernel;

typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Segment_2<Kernel> Segment_2;
typedef CGAL::Triangle_2<Kernel> Triangle_2;
typedef CGAL::Point_3<Kernel> Point_3;
typedef CGAL::Segment_3<Kernel> Segment_3;
typedef CGAL::Triangle_3<Kernel> Triangle_3;

template <class Refs>
struct Halfedge_with_mark : public CGAL::HalfedgeDS_halfedge_base<Refs> {
  Halfedge_with_mark()
    : CGAL::HalfedgeDS_halfedge_base<Refs>(), m(false)
  {}

  bool m;   // A boundary marker for faces with different status
  void set_mark() 
  {
    m = true;
  }
};

// An items type using my halfedge.
struct Items_with_mark_on_hedge : public CGAL::Polyhedron_items_3 
{
  template <class Refs, class Traits>
  struct Halfedge_wrapper {
      typedef Halfedge_with_mark<Refs> Halfedge;
  };
};

typedef CGAL::Polyhedron_3<Kernel,Items_with_mark_on_hedge> Polyhedron;
typedef CGAL::Node_visitor_refine_polyhedra<Polyhedron, Kernel> Split_visitor;
typedef std::vector<Kernel::Point_3> Polyline_3;

struct Is_not_marked{
  bool operator()(Polyhedron::Halfedge_const_handle h) const{
      return !h->m;
  }
};

namespace SFCGAL {
namespace algorithm
{
	std::auto_ptr<Geometry> intersection_point_x_( const Point& pt, const Geometry& gb )
	{
		if ( intersects3D( pt, gb )) {
			return std::auto_ptr<Geometry>(new Point(pt));
		}
		return std::auto_ptr<Geometry>(new GeometryCollection());
	}

	std::auto_ptr<Geometry> intersection_triangles_( const Triangle& tria, const Triangle& trib )
	{
		CGAL::Object obj = CGAL::intersection( tria.toTriangle_3<Kernel>(), trib.toTriangle_3<Kernel>() );
		if (obj.empty()) {
			return std::auto_ptr<Geometry>(new GeometryCollection());
		}
		return std::auto_ptr<Geometry>(Geometry::fromCGAL<Kernel>( obj ));
	}

	///
	/// intersections involving Box_d
	///
	std::auto_ptr<Geometry> intersection_box_d_( const Geometry& ga, const Geometry& gb )
	{
		std::vector<detail::Object3Box> aboxes, bboxes;
		std::list<detail::ObjectHandle> ahandles, bhandles;

		detail::to_boxes<3>( ga, ahandles, aboxes );
		detail::to_boxes<3>( gb, bhandles, bboxes );
		
		detail::intersection_cb<Kernel,3> cb;
		CGAL::box_intersection_d( aboxes.begin(), aboxes.end(), 
					  bboxes.begin(), bboxes.end(),
					  cb );

		if ( cb.geometries->numGeometries() == 1 ) {
			// copy
			std::auto_ptr<Geometry> g(cb.geometries->geometryN(0).clone());
			delete cb.geometries;
			return g;
		}
		return std::auto_ptr<Geometry>(cb.geometries);
	}

	std::auto_ptr<Polyhedron> intersection3D_polyhedra( Polyhedron& polya, Polyhedron& polyb )
	{
		Split_visitor visitor( NULL, true );

		std::list<Polyline_3> polylines;
		CGAL::Intersection_of_Polyhedra_3<Polyhedron,Kernel, Split_visitor> intersect_polys(visitor);
		intersect_polys( polya, polyb, std::back_inserter(polylines) );
		if ( polylines.size() == 0 ) {
			// no intersection
			return std::auto_ptr<Polyhedron>();
		}

		std::list<Polyhedron> decomposition;

		Is_not_marked criterion;
		CGAL::internal::extract_connected_components( polyb, criterion, std::back_inserter(decomposition));

		CGAL::Point_inside_polyhedron_3<Polyhedron, Kernel> point_inside_q( const_cast<Polyhedron&>(polyb) );

		for ( std::list<Polyhedron>::iterator it=decomposition.begin();it!=decomposition.end();++it)
		{
			// take a point on the component and tests if its inside the other polyhedron
			//
			CGAL::Point_3<Kernel> test_point;
			if ( it->size_of_facets() == 1 ) {
				//
				// If we only have a facet, take the centroid (cannot take a point on the intersection)
				CGAL::Point_3<Kernel> q[3];
				Polyhedron::Point_iterator pit = it->points_begin();
				for ( size_t i = 0; i < 3; ++i, ++pit ) {
					q[i] = *pit;
				}
				test_point = CGAL::centroid( q[0], q[1], q[2] );
			}
			else {
				//
				// Take a point not an the border
				for ( Polyhedron::Halfedge_iterator hit = it->halfedges_begin(); hit != it->halfedges_end(); ++hit ) {
					if ( !hit->is_border() ) {
						// take the center of the edge
						test_point = CGAL::midpoint( hit->vertex()->point(), hit->prev()->vertex()->point() );
					}
				}
			}
			
			if ( point_inside_q( test_point ) ) {
				return std::auto_ptr<Polyhedron>(new Polyhedron( *it ));
			}
		}
		// empty
		return std::auto_ptr<Polyhedron>();
	}

	std::auto_ptr<Geometry> intersection3D_solid_( const Geometry& ga, const Solid& solid )
	{
		// FIXME : only consider the exteriorShell for now
		std::auto_ptr<Polyhedron> polyb = solid.exteriorShell().toPolyhedron_3<Kernel, Polyhedron>();

		switch ( ga.geometryTypeId() ) {
		case TYPE_LINESTRING:
			// TODO
			break;
		case TYPE_TRIANGLE: {
			const Triangle& tri = static_cast<const Triangle&>(ga);
			TriangulatedSurface surf;
			surf.addTriangle( tri );
			std::auto_ptr<Polyhedron> polya = surf.toPolyhedron_3<Kernel, Polyhedron>();

			std::auto_ptr<Polyhedron> rpoly = intersection3D_polyhedra( *polya, *polyb );
			if ( rpoly.get() != 0 ) {
				// extract the boundary
				std::list<Point_3> boundary;
				for ( Polyhedron::Halfedge_iterator it = rpoly->halfedges_begin(); it != rpoly->halfedges_end(); ++it ) {
					if ( !it->is_border() )
						continue;
					Point_3 p1 = it->prev()->vertex()->point();
					Point_3 p2 = it->vertex()->point();
					if ( boundary.size() == 0 ) {
						boundary.push_back( p1 );
						boundary.push_back( p2 );
					}
					else if ( boundary.back() == p1 ) {
						boundary.push_back( p2 );
					}
					else if ( boundary.front() == p2 ) {
						boundary.push_front( p1 );
					}
				}
				if ( boundary.size() == 4 ) {
					// It is a triangle

					Point p[3];
					std::list<Point_3>::const_iterator it = boundary.begin();
					for ( size_t i = 0; i < 3; ++i, ++it ) {
						p[i] = *it;
					}
					return std::auto_ptr<Geometry>(new Triangle( p[0], p[1], p[2] ));
				}

				// It is a polygon
				LineString ls;
				for ( std::list<Point_3>::const_iterator it = boundary.begin(); it != boundary.end(); ++it ) {
					ls.addPoint( *it );
				}
				return std::auto_ptr<Geometry>(new Polygon( ls ));
			}
		} break;			
		}

		return std::auto_ptr<Geometry>();
	}

	std::auto_ptr<Geometry> intersection3D( const Geometry& ga, const Geometry& gb )
	{
		// deal with geometry collection
		// call intersection on each geometry of the collection
		const GeometryCollection* coll;
		if ( (coll = dynamic_cast<const GeometryCollection*>( &ga )) ) {
			GeometryCollection *ret = new GeometryCollection();
			for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
				ret->addGeometry(intersection3D( coll->geometryN( i ), gb).release());
			}
			return std::auto_ptr<Geometry>( ret );
		}
		if ( (coll = dynamic_cast<const GeometryCollection*>( &gb )) ) {
			GeometryCollection *ret = new GeometryCollection();
			for ( size_t i = 0; i < coll->numGeometries(); i++ ) {
				ret->addGeometry(intersection3D( coll->geometryN( i ), ga).release());
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
			case TYPE_TIN:
				return intersection_box_d_( ga, gb );
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				// generic processing
				break;
			default:
				// symmetric call
				return intersection3D( gb, ga );
			}
			break;
		case TYPE_TRIANGLE:
			switch ( gb.geometryTypeId() ) {
			case TYPE_TRIANGLE:
				return intersection_triangles_( static_cast<const Triangle&>(ga), static_cast<const Triangle&>(gb) );
			case TYPE_TIN:
				return intersection_box_d_( ga, gb );
			case TYPE_SOLID:
				return intersection3D_solid_( ga, static_cast<const Solid&>(gb) );
			case TYPE_POLYGON:
			case TYPE_POLYHEDRALSURFACE:
				break;
			default:
				// symmetric call
				return intersection3D( gb, ga );
			}
		case TYPE_POLYGON:
			{
				TriangulatedSurface surf;
				algorithm::triangulate( static_cast<const PolyhedralSurface&>(ga), surf);
				return intersection3D( ga, surf );
			} break;

		case TYPE_TIN:
			switch ( gb.geometryTypeId() ) {
			case TYPE_TIN:
				return intersection_box_d_( ga, gb );
			case TYPE_POLYHEDRALSURFACE:
			case TYPE_SOLID:
				break;
			default:
				// symmetric call
				return intersection3D( gb, ga );
			}
		case TYPE_POLYHEDRALSURFACE:
			{
				TriangulatedSurface surf;
				algorithm::triangulate( static_cast<const PolyhedralSurface&>(ga), surf);
				return intersection3D( surf, gb );
			} break;
		case TYPE_SOLID:
			switch ( gb.geometryTypeId() ) {
			case TYPE_SOLID:
				//				return intersection3D_coref_( static_cast<const Solid&>(ga), static_cast<const Solid&>(gb));
				break;
			default:
				// symmetric call
				return intersection3D( gb, ga );
			}
			break;
		default:
			// TODO
			break;
		}

		if ( gb.geometryTypeId() == TYPE_POLYHEDRALSURFACE || gb.geometryTypeId() == TYPE_POLYGON ) {
			TriangulatedSurface surf;
			algorithm::triangulate( static_cast<const PolyhedralSurface&>(gb), surf);
			return intersection3D( ga, surf );
		}
		
		throw std::runtime_error( "intersection3D() not supported on " + ga.geometryType() + " x " + gb.geometryType() );
		// null object
		return std::auto_ptr<Geometry>();
	}
}
}
