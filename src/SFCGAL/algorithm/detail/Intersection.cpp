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
#include <SFCGAL/algorithm/detail/intersection.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/length.h>
#include <SFCGAL/algorithm/area.h>


namespace SFCGAL {
namespace algorithm {
namespace detail {
    
	using namespace SFCGAL::detail;

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
	bool inline intersectsD( const Geometry& g1, const Geometry& g2, dim_t<2> )
	{
		return algorithm::intersects( g1, g2 );
	}
	bool inline intersectsD( const Geometry& g1, const Geometry& g2, dim_t<3> )
	{
		return algorithm::intersects3D( g1, g2 );
	}

	template <int Dim>
	inline void collectionInsert( std::list<Geometry*>& coll, Geometry* g )
	{
		std::vector<Geometry*> toErase;
		for ( std::list<Geometry*>::iterator it = coll.begin(); it != coll.end(); ++it ) {
			if ( intersectsD( *(*it), *g, dim_t<Dim>() ) ) {
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

	template <int Dim>
	intersection_cb_base<Dim>::intersection_cb_base()
	{
		geometries = boost::shared_ptr< std::list<Geometry*> >( new std::list<Geometry*> );
	}


	template <int Dim>
	std::auto_ptr<GeometryCollection> intersection_cb_base<Dim>::geometryCollection() const
	{
		std::auto_ptr<GeometryCollection> collect( new GeometryCollection );
		for ( std::list<Geometry*>::const_iterator it = geometries->begin(); it != geometries->end(); ++it ) {
			collect->addGeometry( *it );
		}
		return collect;
	}

	template struct intersection_cb_base<2>;
	template struct intersection_cb_base<3>;

	template <int Dim>
	struct intersection_cb<ObjectHandle::Segment,ObjectHandle::Segment, Dim>
	{
		intersection_cb_base<Dim> base;

		void operator() ( const typename ObjectBox<Dim>::Type& a, const typename ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Segment Segment_d;
			
			Segment_d sega( a.handle()->segment.start_point->template toPoint_d<Dim>(),
					a.handle()->segment.end_point->template toPoint_d<Dim>());
			Segment_d segb( b.handle()->segment.start_point->template toPoint_d<Dim>(),
					b.handle()->segment.end_point->template toPoint_d<Dim>());
			CGAL::Object obj = CGAL::intersection( sega, segb );
			if ( !obj.empty()) {
				Geometry* g = Geometry::fromCGAL(obj);
				BOOST_ASSERT( g != 0 );
				collectionInsert<Dim>( *base.geometries, g );
			}
		}
	};

	template <int Dim>
	struct intersection_cb<ObjectHandle::Segment,ObjectHandle::Triangle, Dim>
	{
		intersection_cb_base<Dim> base;

		void operator() ( const typename ObjectBox<Dim>::Type& a, const typename ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Segment Segment_d;
			typedef typename TypeForDimension<Dim>::Triangle Triangle_d;

			Segment_d sega( a.handle()->segment.start_point->template toPoint_d<Dim>(),
					a.handle()->segment.end_point->template toPoint_d<Dim>());
			Triangle_d tri2( b.handle()->triangle->template toTriangle_d<Dim>() );
			CGAL::Object obj = CGAL::intersection( sega, tri2 );
			if ( !obj.empty()) {
				Geometry* g = Geometry::fromCGAL(obj);
				BOOST_ASSERT( g != 0 );
				collectionInsert<Dim>( *base.geometries, g );
			}
		}
	};

	template <int Dim>
	struct intersection_cb<ObjectHandle::Triangle,ObjectHandle::Triangle, Dim>
	{
		intersection_cb_base<Dim> base;

		void operator() ( const typename ObjectBox<Dim>::Type& a, const typename ObjectBox<Dim>::Type& b )
		{
			typedef typename TypeForDimension<Dim>::Triangle Triangle_d;

			Triangle_d tria( a.handle()->triangle->template toTriangle_d<Dim>() );
			Triangle_d trib( b.handle()->triangle->template toTriangle_d<Dim>() );
			CGAL::Object obj = CGAL::intersection( tria, trib );
			if ( !obj.empty()) {
				Geometry* g = Geometry::fromCGAL(obj);
				BOOST_ASSERT( g != 0 );
				collectionInsert<Dim>( *base.geometries, g );
			}
		}
	};

	template struct intersection_cb<ObjectHandle::Segment, ObjectHandle::Segment, 2>;
	template struct intersection_cb<ObjectHandle::Segment, ObjectHandle::Segment, 3>;
	template struct intersection_cb<ObjectHandle::Segment, ObjectHandle::Triangle, 2>;
	template struct intersection_cb<ObjectHandle::Segment, ObjectHandle::Triangle, 3>;
	template struct intersection_cb<ObjectHandle::Triangle, ObjectHandle::Triangle, 2>;
	template struct intersection_cb<ObjectHandle::Triangle, ObjectHandle::Triangle, 3>;

} // detail
} // algorithm
} // SFCGAL
