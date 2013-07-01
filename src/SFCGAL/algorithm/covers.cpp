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
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/Kernel.h>
#include <SFCGAL/detail/TypeForDimension.h>
#include <SFCGAL/detail/GeometrySet.h>

#include <CGAL/box_intersection_d.h>

using namespace SFCGAL::detail;

namespace SFCGAL {
namespace algorithm
{

	bool covers( const PrimitiveHandle<3>&, const PrimitiveHandle<3>& )
	{
		return false;
	}

	template <int Dim>
	double segmentsLength( const GeometrySet<Dim>& gs )
	{
		double result = 0.0;
		for ( typename GeometrySet<Dim>::SegmentCollection::const_iterator it = gs.segments().begin();
			     it != gs.segments().end();
			     ++it ) {
			result = result + sqrt(CGAL::to_double(it->primitive().squared_length()));
		}
		return result;
	}

	double solidsVolume( const GeometrySet<3>& gs, bool planarSurface = false )
	{
		double result = 0.0;
		for ( GeometrySet<3>::VolumeCollection::const_iterator it = gs.volumes().begin();
		      it != gs.volumes().end();
		      ++it ) {
			// TODO : we use areas of surfaces here instead of volumes
			const MarkedPolyhedron& poly = it->primitive();
			if ( poly.is_closed() && planarSurface ) {
				continue;
			}
			if ( ! poly.is_closed() && ! planarSurface ) {
				continue;
			}
			BOOST_ASSERT( poly.is_pure_triangle() );

			CGAL::Point_3<Kernel> p1, p2, p3;
			for ( MarkedPolyhedron::Facet_const_iterator fit = poly.facets_begin();
			      fit != poly.facets_end();
			      ++fit ) {
				MarkedPolyhedron::Halfedge_around_facet_const_circulator cit = fit->facet_begin();
				p1 = cit->vertex()->point();
				cit++;
				p2 = cit->vertex()->point();
				cit++;
				p3 = cit->vertex()->point();
				CGAL::Triangle_3<Kernel> tri( p1, p2, p3 );
				result = result + sqrt(CGAL::to_double(tri.squared_area()));
			}
		}
		return result;
	}

	double surfacesArea( const GeometrySet<2>& gs )
	{
		Kernel::FT result = 0.0;
		for ( GeometrySet<2>::SurfaceCollection::const_iterator it = gs.surfaces().begin();
		      it != gs.surfaces().end();
		      ++it ) {
			const CGAL::Polygon_with_holes_2<Kernel>& polygon = it->primitive();
			result = result + CGAL::abs(polygon.outer_boundary().area());
			for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator hit = polygon.holes_begin();
			      hit != polygon.holes_end();
			      ++hit ) {
				result = result - CGAL::abs(hit->area());
			}
		}
		return CGAL::to_double(result);
	}

	double surfacesArea( const GeometrySet<3>& gs )
	{
		double result = 0.0;
		if ( gs.surfaces().empty() && !gs.volumes().empty() ) {
			result = solidsVolume( gs, /* planarSurfaces = */ true );
		}
		for ( GeometrySet<3>::SurfaceCollection::const_iterator it = gs.surfaces().begin();
		      it != gs.surfaces().end();
		      ++it ) {
			result = result + sqrt(CGAL::to_double(it->primitive().squared_area()));
		}
		return result;
	}

	double solidsVolume( const GeometrySet<2>& )
	{
		return 0.0;
	}

	template <int Dim>
	bool equalLength( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b, int dim )
	{
		// compare 'length' of primitives in A with 'length' of primitives in B
		// 'length' is :
		// - number of elements for points
		// - length for segments
		// - area for surfaces
		// - should be volume for volumes. We use area here

		double tol = 1e-9;
		switch ( dim ) {
		case 0: {
			if ( a.points().size() != b.points().size() ) {
				return false;
			}
		}
		case 1: {
			
			//
			// Compare lengths
			double lengthA = segmentsLength( a );
			double lengthB = segmentsLength( b );
			double cmp = (lengthA - lengthB) * (lengthA - lengthB);
			if ( cmp > tol ) {
				return false;
			}
		}
		case 2: {
			//
			// Compare areas
			double areaA = surfacesArea( a );
			double areaB = surfacesArea( b );
			double cmp = (areaA - areaB) * (areaA - areaB);
			if ( cmp > tol ) {
				return false;
			}
		}
		case 3: {
			// Compare volumes
			double volA = solidsVolume( a );
			double volB = solidsVolume( b );
			double cmp = (volA - volB) * (volA - volB);
			if ( cmp > tol ) {
				return false;
			}
		}
		}

		return true;
	}

	template <int Dim>
	bool covers( const GeometrySet<Dim>& a, const GeometrySet<Dim>& b )
	{
		int dimA = a.dimension();
		int dimB = b.dimension();

		if ( dimA == -1 || dimB == -1 ) {
			return false;
		}

		if ( dimB > dimA ) {
			return false;
		}

		GeometrySet<Dim> inter;
		algorithm::intersection( a, b, inter );

		if ( ! b.points().empty() && ! equalLength( b, inter, 0 ) ) {
				return false;
		}
		if ( ! b.segments().empty() && ! equalLength( b, inter, 1 ) ) {
				return false;
		}
		if ( ! b.surfaces().empty() && ! equalLength( b, inter, 2 ) ) {
				return false;
		}
		if ( ! b.volumes().empty() && ! equalLength( b, inter, 3 ) ) {
				return false;
		}
		return true;
	}	

	template bool covers<2>( const GeometrySet<2>& a, const GeometrySet<2>& b );
	template bool covers<3>( const GeometrySet<3>& a, const GeometrySet<3>& b );

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
