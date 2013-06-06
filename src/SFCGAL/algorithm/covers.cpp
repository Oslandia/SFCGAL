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

#include <SFCGAL/algorithm/covers.h>
#include <SFCGAL/Geometry.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/Kernel.h>
#include <SFCGAL/detail/TypeForDimension.h>
#include <SFCGAL/detail/GeometrySet.h>

#include <CGAL/box_intersection_d.h>

using namespace SFCGAL::detail;

namespace SFCGAL {
namespace algorithm
{
	//
	// True if no points of pb is outside pa
	template <int Dim>
	bool covers( const PrimitiveHandle<Dim>& pa, const PrimitiveHandle<Dim>& pb )
	{
		typedef typename TypeForDimension<Dim>::Point TPoint;
		typedef typename TypeForDimension<Dim>::Segment TSegment;
		typedef typename TypeForDimension<Dim>::Surface TSurface;
		typedef typename TypeForDimension<Dim>::Volume TVolume;

		if ( pa.handle.which() < pb.handle.which() ) {
			// no geometry can cover a geometry of greater dimension
			return false;
		}

		// if one point of B does not intersect A, return false
		// else return true
		GeometrySet<Dim> ps;
		ps.collectPoints( pb );
		typename GeometrySet<Dim>::PointCollection& points = ps.points();
		for ( typename GeometrySet<Dim>::PointCollection::const_iterator it = points.begin(); it != points.end(); ++it ) {
			PrimitiveHandle<Dim> ppt( &it->primitive() );
			if ( !algorithm::intersects( pa, ppt ) ) {
				return false;
			}
		}
		return true;
	}

	struct not_covered{};

	template <int Dim>
	struct covers_cb
	{
		bool* tested;
		covers_cb( bool* t ) : tested(t) {}
		void operator()( const typename PrimitiveBox<Dim>::Type& a,
				 const typename PrimitiveBox<Dim>::Type& b )
		{
			*tested = true;
			if ( !covers( *a.handle(), *b.handle() ) ) {
				throw not_covered();
			}
		}
	};

	template <int Dim>
	bool covers( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b )
	{
		typename HandleCollection<Dim>::Type ahandles, bhandles;
		typename BoxCollection<Dim>::Type aboxes, bboxes;
		a.computeBoundingBoxes( ahandles, aboxes );
		b.computeBoundingBoxes( bhandles, bboxes );
		if ( aboxes.empty() || bboxes.empty() ) {
			return false;
		}

		bool tested = false;
		covers_cb<Dim> cb( &tested );
		try {
			CGAL::box_intersection_d( aboxes.begin(), aboxes.end(),
						  bboxes.begin(), bboxes.end(),
						  cb );
		}
		catch ( not_covered& e ) {
			return false;
		}
		return *cb.tested;
	}

	template bool covers<2>( const GeometrySet<2>& a, const GeometrySet<2>& b );
	template bool covers<3>( const GeometrySet<3>& a, const GeometrySet<3>& b );

	template bool covers<2>( const PrimitiveHandle<2>& a, const PrimitiveHandle<2>& b );
	template bool covers<3>( const PrimitiveHandle<3>& a, const PrimitiveHandle<3>& b );

	bool covers( const Geometry& ga, const Geometry& gb )
	{
		if ( ga.isEmpty() || gb.isEmpty() ) {
			return false;
		}
		GeometrySet<2> gsa( ga );
		GeometrySet<2> gsb( gb );

		return covers( gsa, gsb );
	}

	bool covers3D( const Geometry& ga, const Geometry& gb )
	{
		if ( ga.isEmpty() || gb.isEmpty() ) {
			return false;
		}
		GeometrySet<3> gsa( ga );
		GeometrySet<3> gsb( gb );

		return covers( gsa, gsb );
	}
}
}
