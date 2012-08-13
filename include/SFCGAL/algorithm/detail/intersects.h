#ifndef _SFCGAL_ALGORITHM_DETAIL_INTERSECTS_
#define _SFCGAL_ALGORITHM_DETAIL_INTERSECTS_

namespace SFCGAL {
namespace algorithm {
namespace detail {

	///
	/// Structure used to store handle to a segment or to a triangle
	///
	struct ObjectHandle
	{
		enum
		{
			Segment,
			Triangle
		} ObjectType;
		int type;
		
		union
		{
			struct
			{
				const SFCGAL::Point* start_point;
				const SFCGAL::Point* end_point;
			} segment;

			const SFCGAL::Triangle* triangle;
		};

		///
		/// Constructors
		ObjectHandle( const SFCGAL::Point* spoint, const SFCGAL::Point* epoint ) :
			type(ObjectHandle::Segment)
		{
			segment.start_point = spoint;
			segment.end_point = epoint;
		}
		ObjectHandle( const SFCGAL::Triangle* triangle ) :
			type(ObjectHandle::Triangle), triangle(triangle) {}

		///
		/// 2D Bounding box conversion
		CGAL::Bbox_2 bbox_2() const;

		///
		/// 3D Bounding box conversion
		CGAL::Bbox_3 bbox_3() const;
	};

	typedef CGAL::Box_intersection_d::Box_with_handle_d<double, 2, ObjectHandle*> Object2Box;
	typedef CGAL::Box_intersection_d::Box_with_handle_d<double, 3, ObjectHandle*> Object3Box;

	struct found_segment_segment_intersection {};
	struct found_segment_triangle_intersection {};
	struct found_triangle_triangle_intersection {};

	///
	/// Callback function used with box_intersection_d for 2d intersections
	/// Throws an exception if a intersection has been found
	///
	template <typename Kernel>
	void intersects2_cb( const Object2Box& a, const Object2Box& b )
	{
		if ( a.handle()->type == ObjectHandle::Segment ) {
			CGAL::Segment_2<Kernel> sega( a.handle()->segment.start_point->toPoint_2<Kernel>(),
						      a.handle()->segment.end_point->toPoint_2<Kernel>());
			if ( b.handle()->type == ObjectHandle::Segment ) {
				CGAL::Segment_2<Kernel> segb( b.handle()->segment.start_point->toPoint_2<Kernel>(),
							      b.handle()->segment.end_point->toPoint_2<Kernel>());
				
				if ( CGAL::do_intersect( sega, segb )) {
					throw found_segment_segment_intersection();
				}
			} else {
				// Segment x Triangle
				CGAL::Triangle_2<Kernel> tri2( b.handle()->triangle->toTriangle_2<Kernel>() );
				if ( CGAL::do_intersect( sega, tri2 )) {
				 	throw found_segment_triangle_intersection();
				}
			}
		} else {
			// Triangle x Triangle intersection
			CGAL::Triangle_2<Kernel> tria( a.handle()->triangle->toTriangle_2<Kernel>() );
			CGAL::Triangle_2<Kernel> trib( b.handle()->triangle->toTriangle_2<Kernel>() );
			if (CGAL::do_intersect( tria, trib )) {
				throw found_triangle_triangle_intersection();
			}
		}
	}

	///
	/// Callback function used with box_intersection_d for 3d intersections
	/// Throws an exception if a intersection has been found
	///
	template <typename Kernel>
	void intersects3_cb( const Object3Box& a, const Object3Box& b )
	{
		if ( a.handle()->type == ObjectHandle::Segment ) {
			CGAL::Segment_3<Kernel> sega( a.handle()->segment.start_point->toPoint_3<Kernel>(),
						      a.handle()->segment.end_point->toPoint_3<Kernel>());
			if ( b.handle()->type == ObjectHandle::Segment ) {
				CGAL::Segment_3<Kernel> segb( b.handle()->segment.start_point->toPoint_3<Kernel>(),
							      b.handle()->segment.end_point->toPoint_3<Kernel>());
				
				if ( CGAL::do_intersect( sega, segb )) {
					throw found_segment_segment_intersection();
				}
			} else {
				// Segment x Triangle
				CGAL::Triangle_3<Kernel> tri3( b.handle()->triangle->toTriangle_3<Kernel>() );
				if ( CGAL::do_intersect( sega, tri3 )) {
				 	throw found_segment_triangle_intersection();
				}
			}
		} else {
			// Triangle x Triangle intersection
			CGAL::Triangle_3<Kernel> tria( a.handle()->triangle->toTriangle_3<Kernel>() );
			CGAL::Triangle_3<Kernel> trib( b.handle()->triangle->toTriangle_3<Kernel>() );
			if (CGAL::do_intersect( tria, trib )) {
				throw found_triangle_triangle_intersection();
			}
		}
	}

} // detail
} // algorithm
} // SFCGAL

#endif
