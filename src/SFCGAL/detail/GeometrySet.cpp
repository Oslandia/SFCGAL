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

#include <SFCGAL/detail/GeometrySet.h>

#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/Envelope.h>
#include <SFCGAL/TypeForDimension.h>
#include <SFCGAL/detail/GeometrySet.h>

#include <SFCGAL/algorithm/covers.h>

#include <CGAL/Bbox_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>

namespace SFCGAL {
namespace detail {

	void _decompose_triangle( const Triangle& tri, typename GeometrySet<2>::SurfaceCollection& surfaces, dim_t<2> )
	{
		CGAL::Polygon_2<Kernel> outer;
		outer.push_back( tri.vertex(0).toPoint_2() );
		outer.push_back( tri.vertex(1).toPoint_2() );
		outer.push_back( tri.vertex(2).toPoint_2() );
		if ( outer.orientation() == CGAL::CLOCKWISE ) {
			outer.reverse_orientation();
		}
		surfaces.push_back( CGAL::Polygon_with_holes_2<Kernel>( outer ) );
	}
	void _decompose_triangle( const Triangle& tri, typename GeometrySet<3>::SurfaceCollection& surfaces, dim_t<3> )
	{
		CGAL::Triangle_3<Kernel> outtri( tri.vertex(0).toPoint_3(),
						 tri.vertex(1).toPoint_3(),
						 tri.vertex(2).toPoint_3() );
		surfaces.push_back( outtri );
	}

	void _decompose_polygon( const Polygon& poly, typename GeometrySet<2>::SurfaceCollection& surfaces, dim_t<2> )
	{
		surfaces.push_back( poly.toPolygon_with_holes_2() );
	}
	void _decompose_polygon( const Polygon& poly, typename GeometrySet<3>::SurfaceCollection& surfaces, dim_t<3> )
	{
		TriangulatedSurface surf;
		algorithm::triangulate( poly, surf );
		for ( size_t i = 0; i < surf.numTriangles(); ++i ) {
			const Triangle& tri = surf.triangleN( i );
			surfaces.push_back( CGAL::Triangle_3<Kernel>( tri.vertex(0).toPoint_3(),
								      tri.vertex(1).toPoint_3(),
									      tri.vertex(2).toPoint_3() )
					    );
		}
	}

	void _decompose_solid( const Solid& solid, typename GeometrySet<2>::VolumeCollection& volumes, dim_t<2> )
	{
		// nothing
	}
	void _decompose_solid( const Solid& solid, typename GeometrySet<3>::VolumeCollection& volumes, dim_t<3> )
	{
		volumes.push_back( *solid.exteriorShell().toPolyhedron_3<Kernel, MarkedPolyhedron >() );
	}

	template <int Dim>
	GeometrySet<Dim>::GeometrySet( )
	{
	}

	template <int Dim>
	GeometrySet<Dim>::GeometrySet( const Geometry& g )
	{
		_decompose( g );
	}

	template <int Dim>
	void GeometrySet<Dim>::addGeometry( const Geometry& g )
	{
		_decompose( g );
	}

	template <int Dim>
	void GeometrySet<Dim>::addPrimitive( const PrimitiveHandle<Dim>& p )
	{
		switch ( p.handle.which() )
		{
		case PrimitivePoint:
			_points.push_back( *boost::get<const typename TypeForDimension<Dim>::Point*>(p.handle) );
			break;
		case PrimitiveSegment:
			_segments.push_back( *boost::get<const typename TypeForDimension<Dim>::Segment*>(p.handle) );
			break;
		case PrimitiveSurface:
			_surfaces.push_back( *boost::get<const typename TypeForDimension<Dim>::Surface*>(p.handle) );
			break;
		case PrimitiveVolume:
			_volumes.push_back( *boost::get<const typename TypeForDimension<Dim>::Volume*>(p.handle) );
			break;
		}
	}

	template <int Dim>
	void GeometrySet<Dim>::addPrimitive( const CGAL::Object& o )
	{
		typedef typename TypeForDimension<Dim>::Point TPoint;
		typedef typename TypeForDimension<Dim>::Segment TSegment;
		typedef typename TypeForDimension<Dim>::Surface TSurface;
		typedef typename TypeForDimension<Dim>::Volume TVolume;
		if ( const TPoint * p = CGAL::object_cast<TPoint>( &o ) ) {
			_points.push_back( TPoint( *p ) );
		}
		else if ( const std::vector<TPoint> * pts = CGAL::object_cast<std::vector<TPoint> >( &o ) ) {
			std::copy( pts->begin(), pts->end(), std::back_inserter( _points ) );
	        }
		else if ( const TSegment * p = CGAL::object_cast<TSegment>( &o ) ) {
			_segments.push_back( TSegment( *p ) );
		}
		else if ( const TSurface * p = CGAL::object_cast<TSurface>( &o ) ) {
			_surfaces.push_back( TSurface( *p ) );
		}
		else if ( const TVolume * p = CGAL::object_cast<TVolume>( &o ) ) {
			_volumes.push_back( TVolume( *p ) );
		}
	}

	template <int Dim>
	void GeometrySet<Dim>::addPrimitive( const typename TypeForDimension<Dim>::Point& p )
	{
		_points.push_back( p );
	}

	template <int Dim>
	void GeometrySet<Dim>::addPrimitive( const typename TypeForDimension<Dim>::Segment& p )
	{
		_segments.push_back( p );
	}

	template <int Dim>
	void GeometrySet<Dim>::addPrimitive( const typename TypeForDimension<Dim>::Surface& p )
	{
		_surfaces.push_back( p );
	}

	template <int Dim>
	void GeometrySet<Dim>::addPrimitive( const typename TypeForDimension<Dim>::Volume& p )
	{
		_volumes.push_back( p );
	}

	template <int Dim>
	void GeometrySet<Dim>::_decompose( const Geometry& g )
	{
		if ( g.is<GeometryCollection>() ) {
			const GeometryCollection& collect = g.as<GeometryCollection>();
			for ( size_t i = 0; i < g.numGeometries(); ++i ) {
				_decompose( collect.geometryN(i) );
			}
			return;
		}
		switch ( g.geometryTypeId() ) {
		case TYPE_POINT:
			_points.push_back( g.as<Point>().toPoint_d<Dim>() );
			break;
		case TYPE_LINESTRING: {
			const LineString& ls = g.as<LineString>();
			for ( size_t i = 0; i < ls.numPoints() - 1; ++i ) {
				typename TypeForDimension<Dim>::Segment seg( ls.pointN(i).toPoint_d<Dim>(),
									     ls.pointN(i+1).toPoint_d<Dim>() );
				_segments.push_back( seg );
			}
			break;
		}
		case TYPE_TRIANGLE: {
			_decompose_triangle( g.as<Triangle>(), _surfaces, dim_t<Dim>() );
			break;
		}
		case TYPE_POLYGON: {
			_decompose_polygon( g.as<Polygon>(), _surfaces, dim_t<Dim>() );
			break;
		}
		case TYPE_TRIANGULATEDSURFACE: {
			const TriangulatedSurface& tri = g.as<TriangulatedSurface>();
			for ( size_t i = 0; i < tri.numTriangles(); ++i ) {
				_decompose( tri.triangleN( i ) );
			}
			break;
		}
		case TYPE_POLYHEDRALSURFACE: {
			const PolyhedralSurface& tri = g.as<PolyhedralSurface>();
			for ( size_t i = 0; i < tri.numPolygons(); ++i ) {
				_decompose( tri.polygonN( i ) );
			}
			break;
		}
		case TYPE_SOLID: {
			const Solid& solid = g.as<Solid>();
			_decompose_solid( solid, _volumes, dim_t<Dim>() );
			break;
		}
		default:
			break;
		}
	}

	// bbox of a 'volume' for 2D, will never be called
	CGAL::Bbox_2 compute_solid_bbox( const NoVolume&, dim_t<2> )
	{
		return CGAL::Bbox_2();
	}

	CGAL::Bbox_3 compute_solid_bbox( const typename TypeForDimension<3>::Volume& vol, dim_t<3> )
	{
		CGAL::Bbox_3 ret;
		MarkedPolyhedron::Point_const_iterator pit;
		for ( pit = vol.points_begin(); pit != vol.points_end(); ++pit ) {
			ret = ret + pit->bbox();
		}
		return ret;
	}

	template <int Dim>
	void GeometrySet<Dim>::computeBoundingBoxes( typename HandleCollection<Dim>::Type& handles,
					       typename BoxCollection<Dim>::Type& boxes ) const
	{
		boxes.clear();
		for ( typename PointCollection::const_iterator it = _points.begin(); it != _points.end(); ++it ) {
			const typename TypeForDimension<Dim>::Point* pt = &(*it);
			PrimitiveHandle<Dim> h( pt );
			handles.push_back( h );
			boxes.push_back( typename PrimitiveBox<Dim>::Type( it->bbox(), &handles.back() ) );
		}
		for ( typename SegmentCollection::const_iterator it = _segments.begin(); it != _segments.end(); ++it ) {
			handles.push_back( PrimitiveHandle<Dim>( &(*it) ) );
			boxes.push_back( typename PrimitiveBox<Dim>::Type( it->bbox(), &handles.back() ) );
		}
		for ( typename SurfaceCollection::const_iterator it = _surfaces.begin(); it != _surfaces.end(); ++it ) {
			handles.push_back( PrimitiveHandle<Dim>( &(*it) ) );
			boxes.push_back( typename PrimitiveBox<Dim>::Type( it->bbox(), &handles.back() ) );
		}
		for ( typename VolumeCollection::const_iterator it = _volumes.begin(); it != _volumes.end(); ++it ) {
			handles.push_back( PrimitiveHandle<Dim>( &(*it) ) );
			boxes.push_back( typename PrimitiveBox<Dim>::Type( compute_solid_bbox( *it, dim_t<Dim>() ),
									    &handles.back() ) );
		}
	}

	template <int Dim>
	void recompose_points( const typename GeometrySet<Dim>::PointCollection& points,
			       std::vector<Geometry *>& rpoints,
			       dim_t<Dim> )
	{
		if ( points.empty() ) {
			return;
			//			rpoints.push_back( new Point() );
		}
		else {
			for ( typename GeometrySet<Dim>::PointCollection::const_iterator it = points.begin();
			      it != points.end();
			      ++it ) {
				rpoints.push_back( new Point( *it ) );
			}
		}
	}


	template <int Dim>
	void recompose_segments( const typename GeometrySet<Dim>::SegmentCollection& segments,
				 std::vector<Geometry*>& lines,
				 dim_t<Dim> )
	{
		if ( segments.empty() ) {
			//			lines.push_back( new LineString );
			return;
		}

		// convert segments to linestring / multi linestring
		LineString* ls = new LineString;
		bool first = true;
		typename TypeForDimension<Dim>::Segment lastSeg;
		for ( typename GeometrySet<Dim>::SegmentCollection::const_iterator it = segments.begin();
		      it != segments.end();
		      ++it ) {
			std::cout << "segment " << *it << std::endl;
			if ( !first && lastSeg.target() != it->source() ) {
				lines.push_back( ls );
				ls = new LineString;
				first = true;
				std::cout << "new LineString" << std::endl;
			}
			if ( first ) {
				ls->addPoint( new Point( it->source() ) );
				std::cout << "add first point " << it->source() << std::endl;
				first = false;
			}
			ls->addPoint( new Point( it->target() ) );
			std::cout << "add point " << it->target() << std::endl;
			lastSeg = *it;
		}
		lines.push_back( ls );
	}

	void recompose_surfaces( const GeometrySet<2>::SurfaceCollection& surfaces, std::vector<Geometry*>& output, dim_t<2> )
	{
		for ( GeometrySet<2>::SurfaceCollection::const_iterator it = surfaces.begin(); it != surfaces.end(); ++it ) {
			if ( it->holes_begin() == it->holes_end() && it->outer_boundary().size() == 3 ) {
				CGAL::Polygon_2<Kernel>::Vertex_iterator vit = it->outer_boundary().vertices_begin();
				CGAL::Point_2<Kernel> p1( *vit++ );
				CGAL::Point_2<Kernel> p2( *vit++ );
				CGAL::Point_2<Kernel> p3( *vit++ );
				output.push_back( new Triangle(CGAL::Triangle_2<Kernel>( p1, p2, p3 )) );
			}
			else {
				output.push_back( new Polygon( *it ) );
			}
		}
	}

	void recompose_surfaces( const GeometrySet<3>::SurfaceCollection& surfaces, std::vector<Geometry*>& output, dim_t<3> )
	{
		if ( surfaces.empty() ) {
			return;
		}
		// TODO : regroup triangles of the same mesh
		TriangulatedSurface *tri = new TriangulatedSurface;
		for ( GeometrySet<3>::SurfaceCollection::const_iterator it = surfaces.begin(); it != surfaces.end(); ++it ) {
			tri->addTriangle( new Triangle( *it ) );
		}
		output.push_back( tri );
	}

	void recompose_volumes( const GeometrySet<2>::VolumeCollection& volumes, std::vector<Geometry*>& output, dim_t<2> )
	{
	}

	void recompose_volumes( const GeometrySet<3>::VolumeCollection& volumes, std::vector<Geometry*>& output, dim_t<3> )
	{
		if ( volumes.empty() ) {
			return;
		}
		for ( GeometrySet<3>::VolumeCollection::const_iterator vit = volumes.begin(); vit != volumes.end(); ++vit ) {
			if ( vit->is_planar() ) {
				// extract the boundary
				std::list<CGAL::Point_3<Kernel> > boundary;
				for ( MarkedPolyhedron::Halfedge_const_iterator it = vit->halfedges_begin(); it != vit->halfedges_end(); ++it ) {
					if ( !it->is_border() )
						continue;
					CGAL::Point_3<Kernel> p1 = it->prev()->vertex()->point();
					CGAL::Point_3<Kernel> p2 = it->vertex()->point();
					// TODO: test for colinearity
					// Temporary vertice may have been introduced during triangulations
					// and since we expect here a planar surface, it is safe to simplify the boundary
					// by eliminating collinear points.
					if ( boundary.size() == 0 ) {
						boundary.push_back( p1 );
						boundary.push_back( p2 );
					}
					else if ( boundary.back() == p1 ) {
						boundary.push_back( p2 );
					}
					else if ( boundary.front() == p2 ) {
						boundary.push_front( p1 );
					}
				}
				if ( boundary.size() == 3 ) {
					// It is a triangle
					
					Point p[3];
					std::list<CGAL::Point_3<Kernel> >::const_iterator it = boundary.begin();
					for ( size_t i = 0; i < 3; ++i, ++it ) {
						p[i] = *it;
					}
					output.push_back( new Triangle( p[0], p[1], p[2] ) );
				}
				else {
					// Else it is a polygon
					LineString ls;
					for ( std::list<CGAL::Point_3<Kernel> >::const_iterator it = boundary.begin(); it != boundary.end(); ++it ) {
					ls.addPoint( *it );
					}
					output.push_back( new Polygon( ls ) );
				}
			}
			else {
				
				PolyhedralSurface *shell = new PolyhedralSurface( *vit );
				// TODO: test open / closed
				output.push_back( new Solid( shell ) );
			}
		}
	}

	template <int Dim>
	std::auto_ptr<Geometry> GeometrySet<Dim>::recompose() const
	{
		std::vector<Geometry*> points, lines, surfaces, volumes;

		recompose_points( _points, points, dim_t<Dim>() );
		recompose_segments( _segments, lines, dim_t<Dim>() );
		recompose_surfaces( _surfaces, surfaces, dim_t<Dim>() );
		recompose_volumes( _volumes, volumes, dim_t<Dim>() );

		if ( points.size() && !lines.size() && !surfaces.size() && !volumes.size() ) {
			// we have only points
			if ( points.size() == 1 ) {
				return std::auto_ptr<Geometry>( points[0] );
			}
			MultiPoint* ret = new MultiPoint;
			for ( size_t i = 0; i < points.size(); ++i ) {
				ret->addGeometry( points[i] );
			}
			return std::auto_ptr<Geometry>(ret);
		}
		if ( !points.size() && lines.size() && !surfaces.size() && !volumes.size() ) {
			if ( lines.size() == 1 ) {
				return std::auto_ptr<Geometry>( lines[0] );
			}
			MultiLineString* ret = new MultiLineString;
			for ( size_t i = 0; i < lines.size(); ++i ) {
				ret->addGeometry( lines[i] );
			}
			return std::auto_ptr<Geometry>(ret);
		}
		if ( !points.size() && !lines.size() && surfaces.size() && !volumes.size() ) {
			if ( surfaces.size() == 1 ) {
				return std::auto_ptr<Geometry>( surfaces[0] );
			}
			MultiPolygon* ret = new MultiPolygon;
			for ( size_t i = 0; i < surfaces.size(); ++i ) {
				ret->addGeometry( surfaces[i] );
			}
			return std::auto_ptr<Geometry>(ret);
		}
		if ( !points.size() && !lines.size() && !surfaces.size() && volumes.size() ) {
			if ( volumes.size() == 1 ) {
				return std::auto_ptr<Geometry>( volumes[0] );
			}
			GeometryCollection* ret = new GeometryCollection;
			for ( size_t i = 0; i < volumes.size(); ++i ) {
				ret->addGeometry( volumes[i] );
			}
			return std::auto_ptr<Geometry>(ret);
		}

		// else we have a mix of different types
		GeometryCollection* ret = new GeometryCollection;
		for ( size_t i = 0; i < points.size(); ++i ) {
			ret->addGeometry( points[i] );
		}
		for ( size_t i = 0; i < lines.size(); ++i ) {
			ret->addGeometry( lines[i] );
		}
		for ( size_t i = 0; i < surfaces.size(); ++i ) {
			ret->addGeometry( surfaces[i] );
		}
		for ( size_t i = 0; i < volumes.size(); ++i ) {
			ret->addGeometry( volumes[i] );
		}
		return std::auto_ptr<Geometry>( ret );
	}

	void _collect_points( const CGAL::Polygon_with_holes_2<Kernel>& poly, GeometrySet<2>::PointCollection& points )
	{
		for ( CGAL::Polygon_2<Kernel>::Vertex_iterator vit = poly.outer_boundary().vertices_begin();
		      vit != poly.outer_boundary().vertices_end();
		      ++vit ) {
			points.push_back( *vit );
		}
		for ( CGAL::Polygon_with_holes_2<Kernel>::Hole_const_iterator hit = poly.holes_begin();
		      hit != poly.holes_end();
		      ++hit ) {
			for ( CGAL::Polygon_2<Kernel>::Vertex_iterator vit = hit->vertices_begin();
			      vit != hit->vertices_end();
			      ++vit ) {
				points.push_back( *vit );
			}
		}
	}

	void _collect_points( const CGAL::Triangle_3<Kernel>& tri, GeometrySet<3>::PointCollection& points )
	{
		points.push_back( tri.vertex(0) );
		points.push_back( tri.vertex(1) );
		points.push_back( tri.vertex(2) );
	}

	void _collect_points( const NoVolume&, GeometrySet<2>::PointCollection& points )
	{
		// nothing
	}

	void _collect_points( const MarkedPolyhedron& poly, GeometrySet<3>::PointCollection& points )
	{
		for ( MarkedPolyhedron::Vertex_const_iterator vit = poly.vertices_begin();
		      vit != poly.vertices_end();
		      ++vit ) {
			points.push_back( vit->point() );
		}
	}

	template <int Dim>
	void GeometrySet<Dim>::collectPoints( const PrimitiveHandle<Dim>& pa )
	{
		typedef typename TypeForDimension<Dim>::Point TPoint;
		typedef typename TypeForDimension<Dim>::Segment TSegment;
		typedef typename TypeForDimension<Dim>::Surface TSurface;
		typedef typename TypeForDimension<Dim>::Volume TVolume;

		switch ( pa.handle.which() ) {
		case PrimitivePoint: {
			const TPoint *pt = boost::get<const TPoint*>( pa.handle );
			_points.push_back( *pt );
			break;
		}
		case PrimitiveSegment: {
			const TSegment* seg = boost::get<const TSegment*>( pa.handle );
			_points.push_back( seg->source() );
			// TODO : store points in a set
			_points.push_back( seg->target() );
			break;
		}
		case PrimitiveSurface: {
			_collect_points( *boost::get<const TSurface*>( pa.handle ), _points );
			break;
		}
		case PrimitiveVolume: {
			_collect_points( *boost::get<const TVolume*>( pa.handle ), _points );
			break;
		}
		}
	}

	template <int Dim, class IT>
	void _filter_covered( IT ibegin, IT iend, GeometrySet<Dim>& output )
	{
		for ( IT it = ibegin; it != iend; ++it ) {
			GeometrySet<Dim> v1;
			v1.addPrimitive( *it );
			bool v1_covered = false;
			for ( IT it2 = it; it2 != iend; ++it2 ) {
				if ( it == it2 ) {
					continue;
				}

				GeometrySet<Dim> v2;
				v2.addPrimitive( *it2 );
				if ( algorithm::covers( v2, v1 ) ) {
					v1_covered = true;
					break;
				}
			}
			// if its not covered by another volume
			if ( !v1_covered ) {
				// and not covered by another already inserted primitive
				bool b = algorithm::covers( output, v1 );
				if ( !b ) {
					output.addPrimitive( *it );
				}
			}
		}
	}

	template <int Dim>
	void GeometrySet<Dim>::filterCovered( GeometrySet<Dim>& output ) const
	{
		_filter_covered( _volumes.begin(), _volumes.end(), output );
		_filter_covered( _surfaces.begin(), _surfaces.end(), output );
		_filter_covered( _segments.begin(), _segments.end(), output );
		_filter_covered( _points.begin(), _points.end(), output );
	}

	std::ostream& operator<<( std::ostream& ostr, const GeometrySet<2>& g )
	{
		ostr << "points: ";
		std::ostream_iterator<TypeForDimension<2>::Point> out_pt (ostr,", ");
		std::copy( g.points().begin(), g.points().end(), out_pt );
		ostr << std::endl << "segments: ";
		std::ostream_iterator<TypeForDimension<2>::Segment> out_seg (ostr,", ");
		std::copy( g.segments().begin(), g.segments().end(), out_seg );
		ostr << std::endl << "surfaces: ";
		std::ostream_iterator<TypeForDimension<2>::Surface> out_surf (ostr,", ");
		std::copy( g.surfaces().begin(), g.surfaces().end(), out_surf );
		ostr << std::endl;
		return ostr;
	}

	std::ostream& operator<<( std::ostream& ostr, const GeometrySet<3>& g )
	{
		ostr << "points: ";
		std::ostream_iterator<TypeForDimension<3>::Point> out_pt (ostr,", ");
		std::copy( g.points().begin(), g.points().end(), out_pt );
		ostr << std::endl << "segments: ";
		std::ostream_iterator<TypeForDimension<3>::Segment> out_seg (ostr,", ");
		std::copy( g.segments().begin(), g.segments().end(), out_seg );
		ostr << std::endl << "surfaces: ";
		std::ostream_iterator<TypeForDimension<3>::Surface> out_surf (ostr,", ");
		std::copy( g.surfaces().begin(), g.surfaces().end(), out_surf );
		ostr << std::endl << "volumes: ";
		std::ostream_iterator<TypeForDimension<3>::Volume> out_vol (ostr,", ");
		std::copy( g.volumes().begin(), g.volumes().end(), out_vol );
		ostr << std::endl;
		return ostr;
	}

	template class GeometrySet<2>;
	template class GeometrySet<3>;
} // detail
} // SFCGAL
