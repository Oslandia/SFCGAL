/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
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

#include <SFCGAL/algorithm/difference.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/Geometry.h>
#include <SFCGAL/detail/GeometrySet.h>

namespace SFCGAL {

namespace algorithm
{
	// get the point position on segment
	// in [0,1] if its on the interior
	static Kernel::FT point_position( const CGAL::Segment_2<Kernel>& seg, const CGAL::Point_2<Kernel>& pt )
	{
		CGAL::Point_2<Kernel> pA = seg.source();
		CGAL::Point_2<Kernel> pB = seg.target();
		Kernel::FT num = pt.x() - pA.x();
		Kernel::FT den = pB.x() - pA.x();
		if ( den == 0 ) {
			num = pt.y() - pA.y();
			den = pB.y() - pA.y();
		}
		return num / den;
	}

	template <int Dim>
	static void substract_from_segment( const PrimitiveHandle<Dim>& seg, const PrimitiveHandle<Dim>& prim, GeometrySet<Dim>& output )
	{
		if ( prim.handle.which() == PrimitiveSegment ) {
			const typename Segment_d<Dim>::Type* sega = seg.template as<typename Segment_d<Dim>::Type>();
			const typename Segment_d<Dim>::Type* segb = prim.template as<typename Segment_d<Dim>::Type>();
			typename Point_d<Dim>::Type pA( sega->source() );
			typename Point_d<Dim>::Type pB( sega->target() );
			typename Point_d<Dim>::Type pC( segb->source() );
			typename Point_d<Dim>::Type pD( segb->target() );
			Kernel::FT sC = point_position( *sega, pC );
			Kernel::FT sD = point_position( *sega, pD );
			if ( sC > sD ) {
				std::swap( sC, sD );
				std::swap( pC, pD );
			}
			
			if ( sC > 0 ) {
				output.addPrimitive( typename Segment_d<Dim>::Type( pA, pC ) );
			}
			if ( sD < 1 ) {
				output.addPrimitive( typename Segment_d<Dim>::Type( pD, pB ) );
			}
		}
		else {
			output.addPrimitive( seg );
		}
	}

	template <int Dim>
	// pa and pb intersects
	static void difference_primitive( const PrimitiveHandle<Dim>& pa, const PrimitiveHandle<Dim>& pb, GeometrySet<Dim>& output )
	{
		if ( pa.handle.which() == PrimitivePoint ) {
			// difference = empty
		}
		else if ( pa.handle.which() == PrimitiveSegment ) {
			GeometrySet<Dim> inter;
			algorithm::intersection( pa, pb, inter );
			if ( ! inter.segments().empty() ) {
				for ( typename GeometrySet<Dim>::SegmentCollection::const_iterator it = inter.segments().begin();
				      it != inter.segments().end();
				      ++it ) {
					PrimitiveHandle<Dim> p( &it->primitive() );
					substract_from_segment( pa, p, output );
				}
			}
			else {
				output.addPrimitive( pa );
			}
		}
	}
	
	template <int Dim>
	static void filter_self_intersection( const GeometrySet<Dim>& input, GeometrySet<Dim>& output )
	{
		{
			typedef std::list< CollectionElement<typename Point_d<Dim>::Type> > PointList;
			PointList points;

			std::copy( input.points().begin(), input.points().end(), std::back_inserter( points ) );

			typename PointList::iterator it = points.begin();
			while ( it != points.end() ) {
				bool intersectsA = false;
				for ( typename PointList::iterator it2 = points.begin(); it2 != points.end(); ++it2 ) {
					if ( it == it2 ) {
						continue;
					}
					
					PrimitiveHandle<Dim> pa1( &it->primitive() );
					PrimitiveHandle<Dim> pa2( &it2->primitive() );

					if ( CGAL::do_overlap( it->primitive().bbox(), it2->primitive().bbox() ) &&
					     algorithm::intersects( pa1, pa2 ) ) {
						intersectsA = true;
						GeometrySet<Dim> temp;
						algorithm::intersection( pa1, pa2, temp );
						std::copy( temp.points().begin(), temp.points().end(), std::back_inserter( points ) );
						// erase it2
						points.erase( it2 );
						break;
					}
				}
				if ( ! intersectsA ) {
					output.addPrimitive( it->primitive() );
				}
				// suppress A
				it = points.erase( it );
			}
		}
		{
			typedef std::list< CollectionElement<typename Segment_d<Dim>::Type> > SegmentList;
			SegmentList segments;

			std::copy( input.segments().begin(), input.segments().end(), std::back_inserter( segments ) );

			typename SegmentList::iterator it = segments.begin();
			while ( it != segments.end() ) {
				bool intersectsA = false;
				for ( typename SegmentList::iterator it2 = segments.begin(); it2 != segments.end(); ++it2 ) {
					if ( it == it2 ) {
						continue;
					}

					PrimitiveHandle<Dim> pa1( &it->primitive() );
					PrimitiveHandle<Dim> pa2( &it2->primitive() );

					if ( CGAL::do_overlap( it->primitive().bbox(), it2->primitive().bbox() ) &&
					     algorithm::intersects( pa1, pa2 ) ) {
						intersectsA = true;
						GeometrySet<Dim> temp;
						algorithm::intersection( pa1, pa2, temp );
						std::copy( temp.segments().begin(), temp.segments().end(), std::back_inserter( segments ) );
						// erase it2
						segments.erase( it2 );
						break;
					}
				}
				if ( ! intersectsA ) {
					output.addPrimitive( it->primitive() );
				}
				// suppress A
				it = segments.erase( it );
			}
		}
	}

	template <int Dim>
	void difference( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b, GeometrySet<Dim>& output )
	{
		typename SFCGAL::HandleCollection<Dim>::Type ahandles, bhandles;
		typename SFCGAL::BoxCollection<Dim>::Type aboxes, bboxes;
		a.computeBoundingBoxes( ahandles, aboxes );
		b.computeBoundingBoxes( bhandles, bboxes );

		for ( size_t i = 0; i < aboxes.size(); ++i ) {
			bool intersectsA = false;
			GeometrySet<Dim> tempOut;
			for ( size_t j = 0; j < bboxes.size(); ++j ) {
				if ( CGAL::do_overlap(aboxes[i].bbox(), bboxes[j].bbox()) ) {
					const PrimitiveHandle<Dim>* pa = aboxes[i].handle();
					const PrimitiveHandle<Dim>* pb = bboxes[j].handle();
					if ( algorithm::intersects( *pa, *pb ) ) {
						intersectsA = true;

						difference_primitive( *pa, *pb, tempOut );
					}
				}
			}
			if ( ! intersectsA ) {
				tempOut.addPrimitive( *aboxes[i].handle() );
			}

			filter_self_intersection( tempOut, output );
		}
	}

	template void difference<2>( const GeometrySet<2>& a, const GeometrySet<2>& b, GeometrySet<2>& );
	//	template void difference<3>( const GeometrySet<3>& a, const GeometrySet<3>& b, GeometrySet<3>& );

	std::auto_ptr<Geometry> difference( const Geometry& ga, const Geometry& gb )
	{
		GeometrySet<2> gsa( ga ), gsb( gb ), output;
		algorithm::difference( gsa, gsb, output );

		return output.recompose();
	}

}
}
