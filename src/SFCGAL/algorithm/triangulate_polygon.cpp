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
#include <SFCGAL/algorithm/triangulate.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
//#include <CGAL/Polygon_2.h>

#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Line_3.h>

#include <SFCGAL/algorithm/plane.h>
#include <SFCGAL/detail/GeometrySet.h>

#include <iostream>


namespace SFCGAL {
namespace algorithm {


/**
 * face information (depth)
 */
struct FaceInfo2 {
	FaceInfo2() {
	}
	int nesting_level;

	bool in_domain() {
		return nesting_level % 2 == 1;
	}
};


/**
 * vertex information with original coordinates
 */
struct VertexInfo2 {
	VertexInfo2() :
		original(NULL)
	{
	}


	const Point * original ;
};



typedef CGAL::Triangulation_vertex_base_with_info_2< VertexInfo2, Kernel >            triangulation_vertex_base ;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2, Kernel>                  triangulation_face_base;
typedef CGAL::Constrained_triangulation_face_base_2<Kernel, triangulation_face_base>  constrained_triangulation_face_base;
typedef CGAL::Triangulation_data_structure_2<
		triangulation_vertex_base,
		constrained_triangulation_face_base >                                         triangulation_data_structure;

//typedef CGAL::Exact_predicates_tag Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<
			Kernel,
			triangulation_data_structure,
			CGAL::Exact_predicates_tag >                                              CDT;

typedef CGAL::Delaunay_triangulation_2<
	Kernel,
	triangulation_data_structure>                                                 Triangulation;

typedef CDT::Point                                                                    triangulation_point;
typedef CGAL::Point_2<Kernel>                                                         Point_2;
//typedef CGAL::Polygon_2<Kernel>                                                       Polygon_2;



void mark_domains(CDT& ct, CDT::Face_handle start, int index, std::list<CDT::Edge>& border)
{
	if (start->info().nesting_level != -1) {
		return;
	}
	std::list<CDT::Face_handle> queue;
	queue.push_back(start);

	while (!queue.empty()) {
		CDT::Face_handle fh = queue.front();
		queue.pop_front();
		if (fh->info().nesting_level == -1) {
			fh->info().nesting_level = index;
			for (int i = 0; i < 3; i++) {
				CDT::Edge e(fh, i);
				CDT::Face_handle n = fh->neighbor(i);
				if (n->info().nesting_level == -1) {
					if (ct.is_constrained(e))
						border.push_back(e);
					else
						queue.push_back(n);
				}
			}
		}
	}
}

//explore set of facets connected with non constrained edges,
//and attribute to each such set a nesting level.
//We start from facets incident to the infinite vertex, with a nesting
//level of 0. Then we recursively consider the non-explored facets incident
//to constrained edges bounding the former set and increase the nesting level by 1.
//Facets in the domain are those with an odd nesting level.
void mark_domains(CDT& cdt) {
	for (CDT::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it) {
		it->info().nesting_level = -1;
	}

	int index = 0;
	std::list<CDT::Edge> border;
	mark_domains(cdt, cdt.infinite_face(), index++, border);
	while (!border.empty()) {
		CDT::Edge e = border.front();
		border.pop_front();
		CDT::Face_handle n = e.first->neighbor(e.second);
		if (n->info().nesting_level == -1) {
			mark_domains(cdt, n, e.first->info().nesting_level + 1, border);
		}
	}
}


///
///
///
void triangulate( const Geometry & g, TriangulatedSurface & triangulatedSurface )
{
	switch ( g.geometryTypeId() ){
	case TYPE_POINT:
		return ;
	case TYPE_POLYGON:
		triangulate( g.as< Polygon >(), triangulatedSurface ) ;
		return ;
	case TYPE_MULTIPOINT:
		triangulate( g.as< MultiPoint >(), triangulatedSurface ) ;
		return ;
	case TYPE_MULTIPOLYGON:
		triangulate( g.as< MultiPolygon >(), triangulatedSurface ) ;
		return ;
	case TYPE_TRIANGULATEDSURFACE:
		triangulatedSurface.addTriangles( g.as< TriangulatedSurface >() ) ;
		return ;
	case TYPE_POLYHEDRALSURFACE:
		triangulate( g.as< PolyhedralSurface >(), triangulatedSurface ) ;
		return ;
	case TYPE_SOLID:
		triangulate( g.as< Solid >().exteriorShell(), triangulatedSurface ) ;
		return ;
	default:
		break;
	}

	BOOST_THROW_EXCEPTION(
		Exception(
			( boost::format( "can't triangulate type '%1%'" ) % g.geometryType() ).str()
		)
	);
}


///
///
///
void triangulate2D( const Geometry & g, TriangulatedSurface & triangulatedSurface )
{
	switch ( g.geometryTypeId() ){
	case TYPE_POINT:
		return ;
	case TYPE_POLYGON:
		triangulate2D( g.as< Polygon >(), triangulatedSurface ) ;
		return ;
	case TYPE_MULTIPOINT:
		triangulate( g.as< MultiPoint >(), triangulatedSurface ) ;
		return ;
	case TYPE_MULTIPOLYGON:
		triangulate2D( g.as< MultiPolygon >(), triangulatedSurface ) ;
		return ;
	case TYPE_TRIANGULATEDSURFACE:
		triangulatedSurface.addTriangles( g.as< TriangulatedSurface >() ) ;
		return ;
	case TYPE_POLYHEDRALSURFACE:
		triangulate2D( g.as< PolyhedralSurface >(), triangulatedSurface ) ;
		return ;
	default:
		break;
	}

	BOOST_THROW_EXCEPTION(
		Exception(
			( boost::format( "can't triangulate type '%1%'" ) % g.geometryType() ).str()
		)
	);
}

///
///
///
void triangulate( const MultiPoint & geometry, TriangulatedSurface & triangulatedSurface )
{
	/*
	 * filter empty geometry
	 */
	if ( geometry.isEmpty() )
		return ;

	/*
	 * prepare a Constraint Delaunay Triangulation
	 */
	CDT cdt;

	/*
	 * insert each vertex in the triangulation
	 */
	for ( size_t j = 0; j < geometry.numGeometries(); j++ ) {
		const Point & point = geometry.pointN( j );

		/*
		 * insert into triangulation
		 */
		CDT::Vertex_handle vh = cdt.insert( point.toPoint_2() );
		vh->info().original = &point ;
	}


	/*
	 * Convert CDT to TriangulatedSurface
	 */
	for ( CDT::Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end(); ++it )
	{
		const Point * a = it->vertex(0)->info().original ;
		const Point * b = it->vertex(1)->info().original ;
		const Point * c = it->vertex(2)->info().original ;

		// check that vertex has an original vertex
		if ( a == NULL || b == NULL || c == NULL ){
			BOOST_THROW_EXCEPTION( Exception(
				( boost::format("can't triangulate %1% without adding vertex (constraint intersection found)") % geometry.asText() ).str()
			) ) ;
		}

		triangulatedSurface.addTriangle( Triangle( *a, *b, *c ) );
	}

}

///
///
///
void triangulate( const Polygon & polygon, TriangulatedSurface & triangulatedSurface )
{
	/*
	 * filter empty polygon
	 */
	if ( polygon.isEmpty() )
		return ;

	/*
	 * prepare a Constraint Delaunay Triangulation
	 */
	CDT cdt;

//	std::cout << "---------------------------------------------------------" << std::endl ;
//	std::cout << "triangulate polygon : " << polygon.asText() << std::endl;
	CGAL::Plane_3< Kernel > polygonPlane = plane3D< Kernel >( polygon, false ) ;
	if ( polygonPlane.is_degenerate() ){
		BOOST_THROW_EXCEPTION( Exception(
			( boost::format( "can't find plane for polygon %s" ) % polygon.asText() ).str()
		) );
	}

	/*
	 * insert each ring in the triangulation
	 */
	for ( size_t i = 0; i < polygon.numRings(); i++ ){
		const LineString & ring  = polygon.ringN( i );

		CDT::Vertex_handle v_prev ;
		for ( size_t j = 0; j < ring.numPoints(); j++ ) {
			const Point & point = ring.pointN( j );
			CGAL::Point_3< Kernel > p3d = point.toPoint_3();

			/*
			 * insert into triangulation
			 */
			CDT::Vertex_handle vh = cdt.insert( polygonPlane.to_2d( p3d ) );
			vh->info().original = &point ;

			// filter first point
			if ( j != 0 ){
				if ( vh != v_prev ){
					cdt.insert_constraint(vh, v_prev);
				}else{
					//@todo log
				}
			}
			v_prev = vh;
		}
	}


	/*
	 * Mark facets that are inside the domain bounded by the polygon
	 */
	mark_domains(cdt);

	/*
	 * Convert CDT to triangulated surface
	 */
	for ( CDT::Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end(); ++it )
	{
		//ignore holes
		if ( ! it->info().in_domain() ){
			continue ;
		}

		const Point * a = it->vertex(0)->info().original ;
		const Point * b = it->vertex(1)->info().original ;
		const Point * c = it->vertex(2)->info().original ;

		// check that vertex has an original vertex
		if ( a == NULL || b == NULL || c == NULL ){
			BOOST_THROW_EXCEPTION( Exception(
				( boost::format("can't triangulate %1% without adding vertex (constraint intersection found)") % polygon.asText() ).str()
			) ) ;
		}
		triangulatedSurface.addTriangle( Triangle( *a, *b, *c ) );
	}
}


///
///
///
void triangulate2D( const Polygon & polygon, TriangulatedSurface & triangulatedSurface )
{
	/*
	 * filter empty polygon
	 */
	if ( polygon.isEmpty() )
		return ;

	/*
	 * prepare a Constraint Delaunay Triangulation
	 */
	CDT cdt;

	/*
	 * insert each ring in the triangulation
	 */
	for ( size_t i = 0; i < polygon.numRings(); i++ ){
		const LineString & ring  = polygon.ringN( i );

		CDT::Vertex_handle v_prev ;
		for ( size_t j = 0; j < ring.numPoints(); j++ ) {
			const Point & point = ring.pointN( j );

			/*
			 * insert into triangulation
			 */
			CDT::Vertex_handle vh = cdt.insert( point.toPoint_2() );
			vh->info().original = &point ;

			// filter first point
			if ( j != 0 ){
				if ( vh != v_prev ){
					cdt.insert_constraint(vh, v_prev);
				}else{
					//@todo log
				}
			}
			v_prev = vh;
		}
	}


	/*
	 * Mark facets that are inside the domain bounded by the polygon
	 */
	mark_domains(cdt);

	/*
	 * Convert CDT to triangulated surface
	 */
	for ( CDT::Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end(); ++it )
	{
		//ignore holes
		if ( ! it->info().in_domain() ){
			continue ;
		}
//		assert( it->is_valid() );

		const Point * a = it->vertex(0)->info().original ;
		const Point * b = it->vertex(1)->info().original ;
		const Point * c = it->vertex(2)->info().original ;

		// check that vertex has an original vertex
		if ( a == NULL || b == NULL || c == NULL ){
			BOOST_THROW_EXCEPTION( Exception(
				( boost::format("can't triangulate %1% without adding vertex (constraint intersection found)") % polygon.asText() ).str()
			) ) ;
		}
		triangulatedSurface.addTriangle( Triangle( *a, *b, *c ) );
	}
}

///
///
///
void triangulate( const MultiPolygon & multiPolygon, TriangulatedSurface & triangulatedSurface )
{
	for ( size_t i = 0; i < multiPolygon.numGeometries(); i++ ){
		triangulate( multiPolygon.geometryN(i).as< Polygon >(), triangulatedSurface );
	}
}

///
///
///
void triangulate2D( const MultiPolygon & multiPolygon, TriangulatedSurface & triangulatedSurface )
{
	for ( size_t i = 0; i < multiPolygon.numGeometries(); i++ ){
		triangulate2D( multiPolygon.geometryN(i).as< Polygon >(), triangulatedSurface );
	}
}

///
///
///
void triangulate( const PolyhedralSurface & poly, TriangulatedSurface & triangulatedSurface )
{
	for ( size_t i = 0; i < poly.numPolygons(); i++ ){
		triangulate( poly.polygonN(i), triangulatedSurface );
	}	
}

///
///
///
void triangulate2D( const PolyhedralSurface & poly, TriangulatedSurface & triangulatedSurface )
{
	for ( size_t i = 0; i < poly.numPolygons(); i++ ){
		triangulate2D( poly.polygonN(i), triangulatedSurface );
	}	
}


///
///
///
void triangulate( const CGAL::Polyhedron_3<Kernel>& polyhedron, detail::GeometrySet<3>& geometry )
{
	typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
	
	Triangulation triangulation;
	
	for ( Polyhedron::Facet_const_iterator it = polyhedron.facets_begin(); it != polyhedron.facets_end(); ++it ) {
		Polyhedron::Facet::Halfedge_around_facet_const_circulator fit;
		
		triangulation.clear();
		
		const CGAL::Plane_3<Kernel>& plane = it->plane();
		
		fit = it->facet_begin();
		do {
			const CGAL::Point_3<Kernel>& pt3 = fit->vertex()->point();
			CGAL::Point_2<Kernel> pt2 = plane.to_2d( pt3 );

			Triangulation::Vertex_handle vh = triangulation.insert( pt2 );
			vh->info().original = reinterpret_cast<const SFCGAL::Point*>( &pt3 ); // well a pointer is a pointer ...

			fit ++;
		} while (fit != it->facet_begin() );

		
		for ( Triangulation::Finite_faces_iterator it = triangulation.finite_faces_begin();
		      it != triangulation.finite_faces_end();
		      ++it )
		{
			const CGAL::Point_3<Kernel> * a = reinterpret_cast<const CGAL::Point_3<Kernel>* >(it->vertex(0)->info().original) ;
			const CGAL::Point_3<Kernel> * b = reinterpret_cast<const CGAL::Point_3<Kernel>* >(it->vertex(1)->info().original) ;
			const CGAL::Point_3<Kernel> * c = reinterpret_cast<const CGAL::Point_3<Kernel>* >(it->vertex(2)->info().original) ;
			
			CGAL::Triangle_3<Kernel> tri( *a, *b, *c );
			geometry.addPrimitive( tri );
		}

	}
}

///
///
///
void triangulate( const CGAL::Polygon_with_holes_2<Kernel>& polygon, detail::GeometrySet<2>& output )
{
	Polygon poly( polygon );
	TriangulatedSurface surf;
	triangulate2D( poly, surf );
	for ( size_t i = 0; i < surf.numTriangles(); ++i ) {
		output.addGeometry( surf.triangleN(i) );
	}
}

}//algorithm
}//SFCGAL



