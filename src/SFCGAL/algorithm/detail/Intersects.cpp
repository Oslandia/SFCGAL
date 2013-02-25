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
#include <CGAL/box_intersection_d.h>
#include <CGAL/intersections.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Triangle_2.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Envelope.h>
#include <SFCGAL/algorithm/detail/intersects.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/length.h>
#include <SFCGAL/algorithm/area.h>


namespace SFCGAL {
namespace algorithm {
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


	template <int Dim>
	class intersects_cb<detail::ObjectHandle::Point, detail::ObjectHandle::Triangle, Dim>
	{
	public:
		void operator()( const typename detail::ObjectBox<Dim>::Type& a,
				 const typename detail::ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Point Point_d;
			typedef typename TypeForDimension<Dim>::Triangle Triangle_d;
			Point_d point = a.handle()->point->template toPoint_d<Dim>();
			Triangle_d tri = b.handle()->triangle->template toTriangle_d<Dim>();
			if ( CGAL::do_intersect( point, tri )) {
				throw detail::found_point_triangle_intersection();
			}
		}
	};

	template <int Dim>
	class intersects_cb<detail::ObjectHandle::Segment, detail::ObjectHandle::Segment, Dim>
	{
	public:
		void operator()( const typename detail::ObjectBox<Dim>::Type& a,
				 const typename detail::ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Segment Segment_d;
			Segment_d sega( a.handle()->segment.start_point->template toPoint_d<Dim>(),
					a.handle()->segment.end_point->template toPoint_d<Dim>());
			Segment_d segb( b.handle()->segment.start_point->template toPoint_d<Dim>(),
					b.handle()->segment.end_point->template toPoint_d<Dim>());
			if ( CGAL::do_intersect( sega, segb )) {
				throw detail::found_segment_segment_intersection();
			}
		}
	};

	template <int Dim>
	class intersects_cb<detail::ObjectHandle::Segment, detail::ObjectHandle::Triangle, Dim>
	{
	public:
		void operator()( const typename detail::ObjectBox<Dim>::Type& a,
				 const typename detail::ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Segment Segment_d;
			typedef typename TypeForDimension<Dim>::Triangle Triangle_d;
			Segment_d sega( a.handle()->segment.start_point->template toPoint_d<Dim>(),
					a.handle()->segment.end_point->template toPoint_d<Dim>());
			Triangle_d tri = b.handle()->triangle->template toTriangle_d<Dim>();
			if ( CGAL::do_intersect( sega, tri )) {
				throw detail::found_segment_triangle_intersection();
			}
		}
	};

	template <int Dim>
	class intersects_cb<detail::ObjectHandle::Triangle, detail::ObjectHandle::Triangle, Dim>
	{
	public:
		void operator()( const typename detail::ObjectBox<Dim>::Type& a,
				 const typename detail::ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Triangle Triangle_d;
			Triangle_d tria( a.handle()->triangle->template toTriangle_d<Dim>() );
			Triangle_d trib( b.handle()->triangle->template toTriangle_d<Dim>() );
			if (CGAL::do_intersect( tria, trib )) {
				throw detail::found_triangle_triangle_intersection();
			}

		}
	};
	/// template instanciations
	template class intersects_cb<ObjectHandle::Point, ObjectHandle::Triangle, 2>;
	template class intersects_cb<ObjectHandle::Point, ObjectHandle::Triangle, 3>;
	template class intersects_cb<ObjectHandle::Segment, ObjectHandle::Segment, 2>;
	template class intersects_cb<ObjectHandle::Segment, ObjectHandle::Segment, 3>;
	template class intersects_cb<ObjectHandle::Segment, ObjectHandle::Triangle, 2>;
	template class intersects_cb<ObjectHandle::Segment, ObjectHandle::Triangle, 3>;
	template class intersects_cb<ObjectHandle::Triangle, ObjectHandle::Triangle, 2>;
	template class intersects_cb<ObjectHandle::Triangle, ObjectHandle::Triangle, 3>;

	template <int Dim>
	intersection_cb<Dim>::intersection_cb()
	{
		geometries = boost::shared_ptr< std::list<Geometry*> >( new std::list<Geometry*> );
	}

	//
	// Tell if the gA geometry is "larger" (or equal) than gB
	// It is larger if its dimension is larger (line over point)
	// Or if its length/area/volume is greater
	bool isLarger( const Geometry& gA, const Geometry& gB )
	{
		if ( gA.dimension() != gB.dimension() ) {
			return gA.dimension() > gB.dimension();
		}
		if ( gA.dimension() == 0 ) { // points
			return true;
		}
		else if ( gA.dimension() == 1 ) { // lines
			return algorithm::length( gA ) >= algorithm::length( gB );
		}
		else if ( gA.dimension() == 2 ) { // surfaces
			return algorithm::area( gA ) >= algorithm::area( gB );
		}
		// TODO
		// else if ( gA.dimension() == 3 ) { // solids
		// 	return algorithm::volume( gA ) > algorithm::volume( gB );
		// }		
		return false;
	}


	//
	// Choose between intersects and intersects3D based on a template parameter
	template <int Dim>
	struct intersectsF
	{
		inline bool operator()( const Geometry& g1, const Geometry& g2 )
		{
			return false;
		}
	};
	template <>
	struct intersectsF<2>
	{
		inline bool operator()( const Geometry& g1, const Geometry& g2 )
		{
			return algorithm::intersects( g1, g2 );
		}
	};
	template <>
	struct intersectsF<3>
	{
		inline bool operator()( const Geometry& g1, const Geometry& g2 )
		{
			return algorithm::intersects3D( g1, g2 );
		}
	};
	template <int Dim>
	bool inline intersectsD( const Geometry& g1, const Geometry& g2 )
	{
		return intersectsF<Dim>()( g1, g2 );
	}

	template <int Dim>
	struct collectionInsertF
	{
		void operator()( std::list<Geometry*>& coll, Geometry *g )
		{
			std::vector<Geometry*> toErase;
			for ( std::list<Geometry*>::iterator it = coll.begin(); it != coll.end(); ++it ) {
				if ( intersectsD<Dim>( *(*it), *g ) ) {
					if ( isLarger( *g, *(*it) ) ) {
						// if the candidate is larger than the intersecting geometry
						// erase it
						toErase.push_back( *it );
					}
					else if ( isLarger( *(*it), *g ) ) {
						// the candidate intersects with an already present geometry
						// that is larger, abort
						return;
					}
				}
			}
			for ( size_t i = 0; i < toErase.size(); ++i ) {
				delete toErase[i];
				coll.erase( std::find( coll.begin(), coll.end(), toErase[i] ) );
			}
			coll.push_back( g );
		}
	};
	template <int Dim>
	inline void collectionInsert( std::list<Geometry*>& coll, Geometry* g )
	{
		collectionInsertF<Dim>()( coll, g );
	}

	template <int Dim>
	std::auto_ptr<GeometryCollection> intersection_cb<Dim>::geometryCollection() const
	{
		std::auto_ptr<GeometryCollection> collect( new GeometryCollection );
		for ( std::list<Geometry*>::const_iterator it = geometries->begin(); it != geometries->end(); ++it ) {
			collect->addGeometry( *it );
		}
		return collect;
	}

	//
	// FIXME
	// There is no need to test the dynamic type here.
	// We should rely on a strongly-typed implementation.
	// i.e. intersects_cb<Point, Triangle>, intersects_cb<Segment, Segment>, etc.
	template <int Dim>
	void intersection_cb<Dim>::operator() ( const typename ObjectBox<Dim>::Type& a, const typename ObjectBox<Dim>::Type& b )
	{
		typedef typename TypeForDimension<Dim>::Point Point_d;
		typedef typename TypeForDimension<Dim>::Segment Segment_d;
		typedef typename TypeForDimension<Dim>::Triangle Triangle_d;
		
		if ( a.handle()->type == ObjectHandle::Segment ) {
			Segment_d sega( a.handle()->segment.start_point->template toPoint_d<Dim>(),
					a.handle()->segment.end_point->template toPoint_d<Dim>());
			if ( b.handle()->type == ObjectHandle::Segment ) {
				Segment_d segb( b.handle()->segment.start_point->template toPoint_d<Dim>(),
						b.handle()->segment.end_point->template toPoint_d<Dim>());
				
				CGAL::Object obj = CGAL::intersection( sega, segb );
				if ( !obj.empty()) {
					Geometry* g = Geometry::fromCGAL(obj);
					BOOST_ASSERT( g != 0 );
					collectionInsert<Dim>( *geometries, g );
				}
			} else {
				// Segment x Triangle
				Triangle_d tri2( b.handle()->triangle->template toTriangle_d<Dim>() );
				CGAL::Object obj = CGAL::intersection( sega, tri2 );
				if ( !obj.empty()) {
					Geometry* g = Geometry::fromCGAL(obj);
					BOOST_ASSERT( g != 0 );
					collectionInsert<Dim>( *geometries, g );
				}
			}
		} else {
			// Triangle x Triangle intersection
			Triangle_d tria( a.handle()->triangle->template toTriangle_d<Dim>() );
			Triangle_d trib( b.handle()->triangle->template toTriangle_d<Dim>() );
			CGAL::Object obj = CGAL::intersection( tria, trib );
			if ( !obj.empty()) {
				Geometry* g = Geometry::fromCGAL(obj);
				BOOST_ASSERT( g != 0 );
				collectionInsert<Dim>( *geometries, g );
			}
		}
	}
	/// template instanciations
	template struct intersection_cb<2>;
	template struct intersection_cb<3>;
} // detail
} // algorithm
} // SFCGAL
