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
			return CGAL::Bbox_3( xmin, ymin, xmax, ymax, zmin, zmax );
		}
		// else
		return triangle->envelope().toBbox_3();
	}

} // detail
} // algorithm
} // SFCGAL
