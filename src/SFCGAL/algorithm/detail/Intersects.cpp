#include <CGAL/box_intersection_d.h>
#include <CGAL/intersections.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Triangle_2.h>

#include <SFCGAL/Point.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Envelope.h>
#include <SFCGAL/algorithm/detail/intersects.h>

namespace SFCGAL {
namespace algorithm {
namespace detail {
    
	CGAL::Bbox_2 ObjectHandle::bbox_2() const
	{
		if ( type == Segment ) {
			double xmin, xmax, ymin, ymax;
			if ( segment.start_point->x() < segment.end_point->x() ) {
				xmin = segment.start_point->x();
				xmax = segment.end_point->x();
			} else {
				xmin = segment.end_point->x();
				xmax = segment.start_point->x();
			}
			if ( segment.start_point->y() < segment.end_point->y() ) {
				ymin = segment.start_point->y();
				ymax = segment.end_point->y();
			} else {
				ymin = segment.end_point->y();
				ymax = segment.start_point->y();
			}
			return CGAL::Bbox_2( xmin, ymin, xmax, ymax );
		}
		// else
		return triangle->envelope().toBbox_2();
	}

	CGAL::Bbox_3 ObjectHandle::bbox_3() const
	{
		if ( type == Segment ) {
			double xmin, xmax, ymin, ymax, zmin, zmax;
			if ( segment.start_point->x() < segment.end_point->x() ) {
				xmin = segment.start_point->x();
				xmax = segment.end_point->x();
			} else {
				xmin = segment.end_point->x();
				xmax = segment.start_point->x();
			}
			if ( segment.start_point->y() < segment.end_point->y() ) {
				ymin = segment.start_point->y();
				ymax = segment.end_point->y();
			} else {
				ymin = segment.end_point->y();
				ymax = segment.start_point->y();
			}
			if ( segment.start_point->z() < segment.end_point->z() ) {
				zmin = segment.start_point->z();
				zmax = segment.end_point->z();
			} else {
				zmin = segment.end_point->z();
				zmax = segment.start_point->z();
			}
			return CGAL::Bbox_3( xmin, ymin, zmin, xmax, ymax, zmax );
		}
		// else
		return triangle->envelope().toBbox_3();
	}

	///
	/// Auxiliary function used to fill up vectors of handle and boxes for segments, triangle and triangulated surfaces
	///
	void to_boxes_( const LineString& ls, std::list<detail::ObjectHandle>& handles, std::vector<detail::Object2Box>& boxes )
	{
		for ( size_t i = 0; i < ls.numPoints() - 1; ++i ) {
			handles.push_back( detail::ObjectHandle( &ls.pointN(i), &ls.pointN(i+1) ));
			boxes.push_back( detail::Object2Box( handles.back().bbox_2(), &handles.back() ));
		}
	}
	void to_boxes_( const Triangle& tri, std::list<detail::ObjectHandle>& handles, std::vector<detail::Object2Box>& boxes )
	{
		handles.push_back( detail::ObjectHandle( &tri ));
		boxes.push_back( detail::Object2Box( handles.back().bbox_2(), &handles.back() ));
	}
	void to_boxes_( const TriangulatedSurface& surf, std::list<detail::ObjectHandle>& handles, std::vector<detail::Object2Box>& boxes )
	{
		for ( size_t i = 0; i < surf.numTriangles(); ++i ) {
			handles.push_back( &surf.triangleN(i));
			boxes.push_back( detail::Object2Box( handles.back().bbox_2(), &handles.back() ));
		}
	}

	///
	/// Generic function
	void to_boxes( const Geometry& g, std::list<detail::ObjectHandle>& handles, std::vector<detail::Object2Box>& boxes )
	{
		switch ( g.geometryTypeId() ){
		case TYPE_LINESTRING:
			to_boxes_( static_cast<const LineString&>(g), handles, boxes );
			break;
		case TYPE_TRIANGLE:
			to_boxes_( static_cast<const Triangle&>(g), handles, boxes );
			break;
		case TYPE_TIN:
			to_boxes_( static_cast<const TriangulatedSurface&>(g), handles, boxes );
			break;
		default:
			BOOST_THROW_EXCEPTION( Exception( "Trying to call to_boxes() with an incompatible type" ));
		}
	}

} // detail
} // algorithm
} // SFCGAL
