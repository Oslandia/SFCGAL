#include <SFCGAL/all.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/detail/intersects.h>
#include <SFCGAL/algorithm/collect.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

//
// Intersection kernel
typedef CGAL::Exact_predicates_inexact_constructions_kernel IKernel;

typedef CGAL::Point_2<IKernel> Point_2;
typedef CGAL::Segment_2<IKernel> Segment_2;
typedef CGAL::Triangle_2<IKernel> Triangle_2;

namespace SFCGAL {
namespace algorithm
{
	std::auto_ptr<Geometry> intersection_point_x_( const Point& pt, const Geometry& gb )
	{
		if ( intersects( pt, gb )) {
			return std::auto_ptr<Geometry>(new Point(pt));
		}
		return std::auto_ptr<Geometry>(new GeometryCollection());
	}

	std::auto_ptr<Geometry> intersection_triangles_( const Triangle& tria, const Triangle& trib )
	{
		CGAL::Object obj = CGAL::intersection( tria.toTriangle_2<IKernel>(), trib.toTriangle_2<IKernel>() );
		if (obj.empty()) {
			return std::auto_ptr<Geometry>(new GeometryCollection());
		}
		return std::auto_ptr<Geometry>(Geometry::fromCGAL<IKernel>( obj ));
	}

	///
	/// intersections involving Box_d
	///
	std::auto_ptr<Geometry> intersection_box_d_( const Geometry& ga, const Geometry& gb )
	{
		std::vector<detail::Object2Box> aboxes, bboxes;
		std::list<detail::ObjectHandle> ahandles, bhandles;

		detail::to_boxes<2>( ga, ahandles, aboxes );
		detail::to_boxes<2>( gb, bhandles, bboxes );
		
		detail::intersection2_cb<IKernel> cb;
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

	std::auto_ptr<Geometry> intersection( const Geometry& ga, const Geometry& gb )
	{
		switch ( ga.geometryTypeId() ) {
		case TYPE_POINT:
			return intersection_point_x_( static_cast<const Point&>(ga), gb );
		case TYPE_LINESTRING:
			switch ( gb.geometryTypeId() ) {
			case TYPE_LINESTRING:
			case TYPE_TRIANGLE:
			case TYPE_TIN:
				return intersection_box_d_( ga, gb );
			default:
				// TODO
				break;
			}
			break;
		case TYPE_TRIANGLE:
			switch ( gb.geometryTypeId() ) {
			case TYPE_TRIANGLE:
				return intersection_triangles_( static_cast<const Triangle&>(ga), static_cast<const Triangle&>(gb) );
			case TYPE_TIN:
				return intersection_box_d_( ga, gb );
			default:
				// TODO
				break;
			}
		case TYPE_TIN:
			switch ( gb.geometryTypeId() ) {
			case TYPE_TIN:
				return intersection_box_d_( ga, gb );
			default:
				// TODO
				break;
			}
		default:
			// TODO
			break;
		}

		// null object
		return std::auto_ptr<Geometry>();
	}
}
}
