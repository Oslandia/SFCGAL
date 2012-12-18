#ifndef _SFCGAL_ALGORITHM_DETAIL_INTERSECTS_
#define _SFCGAL_ALGORITHM_DETAIL_INTERSECTS_

#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/TypeForDimension.h>

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
			Point,
			Segment,
			Triangle
		} ObjectType;
		int type;
		
		union
		{
			const SFCGAL::Point* point;

			struct
			{
				const SFCGAL::Point* start_point;
				const SFCGAL::Point* end_point;
			} segment;

			const SFCGAL::Triangle* triangle;
		};

		///
		/// Constructors
		ObjectHandle( const SFCGAL::Point* p ) :
			type(ObjectHandle::Point)
		{
			point = p;
		}

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

		///
		/// Generic way to call bbox_2 or bbox_3
		template <int Dim>
		typename TypeForDimension<Dim>::Bbox bbox_d() const;
	};

	typedef CGAL::Box_intersection_d::Box_with_handle_d<double, 2, ObjectHandle*> Object2Box;
	typedef CGAL::Box_intersection_d::Box_with_handle_d<double, 3, ObjectHandle*> Object3Box;

	template <int Dim>
	struct ObjectBox
	{
		typedef void Type;
	};
	template <>
	struct ObjectBox<2>
	{
		typedef detail::Object2Box Type;
	};
	template <>
	struct ObjectBox<3>
	{
		typedef detail::Object3Box Type;
	};

	///
	/// Auxiliary function used to fill up vectors of handle and boxes for segments, triangle and triangulated surfaces
	///
	template <int Dim> // 2 or 3
	void to_boxes( const Geometry& g, std::list<ObjectHandle>& handles, std::vector<typename ObjectBox<Dim>::Type>& boxes );

	struct found_intersection {};
	struct found_point_triangle_intersection : public found_intersection {};
	struct found_point_segment_intersection : public found_intersection {};
	struct found_segment_segment_intersection : public found_intersection {};
	struct found_segment_triangle_intersection : public found_intersection {};
	struct found_triangle_triangle_intersection : public found_intersection {};
	
	///
	/// Callback function used with box_intersection_d for 2d and 3d intersection test
	/// Throws an exception if an intersection has been found
	///
	template <int Dim>
	void intersects_cb( const typename ObjectBox<Dim>::Type& a, const typename ObjectBox<Dim>::Type& b );

	///
	/// Callback functor used with box_intersection_d for 2d and 3d intersections
	///
	template <int Dim>
	struct intersection_cb
	{
		///
		/// The resulting intersection geometry
		GeometryCollection* geometries;
		
		intersection_cb();
		
		void operator() ( const typename ObjectBox<Dim>::Type& a, const typename ObjectBox<Dim>::Type& b );
	};

} // detail
} // algorithm
} // SFCGAL

#endif
