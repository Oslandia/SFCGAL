/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <contact@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <SFCGAL/detail/Box_d.h>

#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/Envelope.h>

namespace SFCGAL {
namespace detail {

	CGAL::Bbox_2 ObjectHandle::bbox_2() const
	{
		if ( type == Segment ) {
			double xmin, xmax, ymin, ymax;
			if ( segment.start_point->x() < segment.end_point->x() ) {
				xmin = CGAL::to_double( segment.start_point->x() ) ;
				xmax = CGAL::to_double( segment.end_point->x() ) ;
			} else {
				xmin = CGAL::to_double( segment.end_point->x() ) ;
				xmax = CGAL::to_double( segment.start_point->x() ) ;
			}
			if ( segment.start_point->y() < segment.end_point->y() ) {
				ymin = CGAL::to_double( segment.start_point->y() ) ;
				ymax = CGAL::to_double( segment.end_point->y() ) ;
			} else {
				ymin = CGAL::to_double( segment.end_point->y() ) ;
				ymax = CGAL::to_double( segment.start_point->y() ) ;
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
				xmin = CGAL::to_double( segment.start_point->x() ) ;
				xmax = CGAL::to_double( segment.end_point->x() ) ;
			} else {
				xmin = CGAL::to_double( segment.end_point->x() ) ;
				xmax = CGAL::to_double( segment.start_point->x() ) ;
			}
			if ( segment.start_point->y() < segment.end_point->y() ) {
				ymin = CGAL::to_double( segment.start_point->y() ) ;
				ymax = CGAL::to_double( segment.end_point->y() ) ;
			} else {
				ymin = CGAL::to_double( segment.end_point->y() ) ;
				ymax = CGAL::to_double( segment.start_point->y() ) ;
			}
			if ( !segment.start_point->is3D() ) {
				zmin = 0.0;
				zmax = 0.0;
			}
			else {
				if ( segment.start_point->z() < segment.end_point->z() ) {
					zmin = CGAL::to_double( segment.start_point->z() ) ;
					zmax = CGAL::to_double( segment.end_point->z() ) ;
				} else {
					zmin = CGAL::to_double( segment.end_point->z() ) ;
					zmax = CGAL::to_double( segment.start_point->z() ) ;
				}
			}
			return CGAL::Bbox_3( xmin, ymin, zmin, xmax, ymax, zmax );
		}
		// else
		return triangle->envelope().toBbox_3();
	}

	template <>
	CGAL::Bbox_2 ObjectHandle::bbox_d<2>() const
	{
		return bbox_2();
	}
	template <>
	CGAL::Bbox_3 ObjectHandle::bbox_d<3>() const
	{
		return bbox_3();
	}

	///
	/// Auxiliary function used to fill up vectors of handle and boxes for segments, triangle and triangulated surfaces
	///
	template <int Dim>
	void to_boxes_( const Point& pt, std::list<detail::ObjectHandle>& handles, std::vector<typename ObjectBox<Dim>::Type>& boxes )
	{
		handles.push_back( detail::ObjectHandle( &pt ));
		boxes.push_back( typename ObjectBox<Dim>::Type( handles.back().bbox_d<Dim>(), &handles.back() ));
	}

	template <int Dim>
	void to_boxes_( const LineString& ls, std::list<detail::ObjectHandle>& handles, std::vector<typename ObjectBox<Dim>::Type>& boxes )
	{
		for ( size_t i = 0; i < ls.numPoints() - 1; ++i ) {
			handles.push_back( detail::ObjectHandle( &ls.pointN(i), &ls.pointN(i+1) ));
			boxes.push_back( typename ObjectBox<Dim>::Type( handles.back().bbox_d<Dim>(), &handles.back() ));
		}
	}

	template <int Dim>
	void to_boxes_( const Triangle& tri, std::list<detail::ObjectHandle>& handles, std::vector<typename ObjectBox<Dim>::Type>& boxes )
	{
		handles.push_back( detail::ObjectHandle( &tri ));
		boxes.push_back( typename ObjectBox<Dim>::Type( handles.back().bbox_d<Dim>(), &handles.back() ));
	}

	template <int Dim>
	void to_boxes_( const TriangulatedSurface& surf, std::list<detail::ObjectHandle>& handles, std::vector<typename ObjectBox<Dim>::Type>& boxes )
	{
		for ( size_t i = 0; i < surf.numGeometries(); ++i ) {
			handles.push_back( &surf.geometryN(i));
			boxes.push_back( typename ObjectBox<Dim>::Type( handles.back().bbox_d<Dim>(), &handles.back() ));
		}
	}

	///
	/// Generic function
        template <int Dim>
	void to_boxes( const Geometry& g, std::list<detail::ObjectHandle>& handles, std::vector<typename ObjectBox<Dim>::Type>& boxes )
	{
		switch ( g.geometryTypeId() ){
		case TYPE_POINT:
			to_boxes_<Dim>( static_cast<const Point&>(g), handles, boxes );
			break;
		case TYPE_LINESTRING:
			to_boxes_<Dim>( static_cast<const LineString&>(g), handles, boxes );
			break;
		case TYPE_TRIANGLE:
			to_boxes_<Dim>( static_cast<const Triangle&>(g), handles, boxes );
			break;
		case TYPE_TRIANGULATEDSURFACE:
			to_boxes_<Dim>( static_cast<const TriangulatedSurface&>(g), handles, boxes );
			break;
		default:
			BOOST_THROW_EXCEPTION( Exception( "Trying to call to_boxes() with an incompatible type " + g.geometryType() ));
		}
	}
	// instanciation of templates
	template void to_boxes<2>( const Geometry& g, std::list<detail::ObjectHandle>& handles, std::vector<Object2Box>& boxes );
	template void to_boxes<3>( const Geometry& g, std::list<detail::ObjectHandle>& handles, std::vector<Object3Box>& boxes );


} // detail
} // SFCGAL
