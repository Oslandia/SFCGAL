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
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Triangle_2.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Envelope.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/detail/intersection.h>
#include <SFCGAL/detail/GeometrySet.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/length.h>
#include <SFCGAL/algorithm/area.h>

namespace SFCGAL {
namespace algorithm {
namespace detail {
    
	using namespace SFCGAL::detail;

#if 0
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
#endif
	void intersection( const PrimitiveHandle<3>& pa, const PrimitiveHandle<3>& pb,
			   GeometrySet<3>& output, dim_t<3> );
	void intersection( const PrimitiveHandle<2>& pa, const PrimitiveHandle<2>& pb,
			   GeometrySet<2>& output, dim_t<2> );
	//
	// We deal here with symmetric call
	template <int Dim>
	void dispatch_intersection_sym( const PrimitiveHandle<Dim>& pa, const PrimitiveHandle<Dim>& pb,
					     GeometrySet<Dim>& output )
	{
		// assume types are ordered by dimension within the boost::variant
		if ( pa.handle.which() >= pb.handle.which() ) {
			intersection( pa, pb, output, dim_t<Dim>() );
		}
		else {
			intersection( pb, pa, output, dim_t<Dim>() );
		}
	}

	template <int Dim>
	struct intersection_cb
	{
		intersection_cb( GeometrySet<Dim>& out ) : output(out) {}

		void operator()( const typename PrimitiveBox<Dim>::Type& a,
				 const typename PrimitiveBox<Dim>::Type& b )
		{
			dispatch_intersection_sym<Dim>( *a.handle(), *b.handle(), output );
		}

		GeometrySet<Dim>& output;
	};

	template <int Dim>
	void intersection( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b, GeometrySet<Dim>& output )
	{
		typename SFCGAL::detail::HandleCollection<Dim>::Type ahandles, bhandles;
		typename SFCGAL::detail::BoxCollection<Dim>::Type aboxes, bboxes;
		a.compute_bboxes( ahandles, aboxes );
		b.compute_bboxes( bhandles, bboxes );

		intersection_cb<Dim> cb( output );
		CGAL::box_intersection_d( aboxes.begin(), aboxes.end(),
					  bboxes.begin(), bboxes.end(),
					  cb );
	}

	template void intersection<2>( const GeometrySet<2>& a, const GeometrySet<2>& b, GeometrySet<2>& );
	template void intersection<3>( const GeometrySet<3>& a, const GeometrySet<3>& b, GeometrySet<3>& );

} // detail
} // algorithm
} // SFCGAL
