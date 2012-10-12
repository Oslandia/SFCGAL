#include <SFCGAL/algorithm/triangulate.h>

#include <SFCGAL/Kernel.h>
#include <SFCGAL/all.h>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
//#include <CGAL/Polygon_2.h>

#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Line_3.h>

#include <SFCGAL/algorithm/plane.h>

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
	VertexInfo2() {
	}


	Point original ;
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

		CGAL::Point_2< Kernel > p2d(
			point.x(),
			point.y()
		);

		/*
		 * insert into triangulation
		 */
		CDT::Vertex_handle vh = cdt.insert( p2d );
		vh->info().original = point ;
	}


	/*
	 * Convert CDT to TriangulatedSurface
	 */
	for ( CDT::Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end(); ++it )
	{
		//ignore holes
		if ( ! it->info().in_domain() ){
			continue ;
		}

		const Point & a = it->vertex(0)->info().original ;
		const Point & b = it->vertex(1)->info().original ;
		const Point & c = it->vertex(2)->info().original ;

		// check that vertex has an original vertex
		if ( a.isEmpty() || b.isEmpty() || c.isEmpty() ){
			BOOST_THROW_EXCEPTION( Exception(
				( boost::format("can't triangulate %1% without adding vertex (constraint intersection found)") % geometry.asText() ).str()
			) ) ;
		}

		triangulatedSurface.addTriangle( Triangle( a, b, c ) );
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
	CGAL::Plane_3< Kernel > polygonPlane = plane3D< Kernel >( polygon ) ;

	/*
	 * insert each ring in the triangulation
	 */
	for ( size_t i = 0; i < polygon.numRings(); i++ ){
		const LineString & ring  = polygon.ringN( i );

		CDT::Vertex_handle v_prev ;
		for ( size_t j = 0; j < ring.numPoints(); j++ ) {
			const Point & point = ring.pointN( j );

			CGAL::Point_3< Kernel > p3d(
				point.x(),
				point.y(),
				point.is3D()?point.z():0.0
			);

			/*
			 * insert into triangulation
			 */
			CDT::Vertex_handle vh = cdt.insert( polygonPlane.to_2d( p3d ) );
			vh->info().original = point ;

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

		const Point & a = it->vertex(0)->info().original ;
		const Point & b = it->vertex(1)->info().original ;
		const Point & c = it->vertex(2)->info().original ;

		// check that vertex has an original vertex
		if ( a.isEmpty() || b.isEmpty() || c.isEmpty() ){
			BOOST_THROW_EXCEPTION( Exception(
				( boost::format("can't triangulate %1% without adding vertex (constraint intersection found)") % polygon.asText() ).str()
			) ) ;
		}
		triangulatedSurface.addTriangle( Triangle( a, b, c ) );
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
void triangulate( const PolyhedralSurface & poly, TriangulatedSurface & triangulatedSurface )
{
	for ( size_t i = 0; i < poly.numPolygons(); i++ ){
		triangulate( poly.polygonN(i), triangulatedSurface );
	}	
}


}//algorithm
}//SFCGAL



