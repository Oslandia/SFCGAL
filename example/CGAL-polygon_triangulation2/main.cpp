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
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>

#include <iostream>
#include <fstream>

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


typedef CGAL::Exact_predicates_inexact_constructions_kernel                           Kernel;
typedef CGAL::Triangulation_vertex_base_with_info_2< unsigned int, Kernel >           triangulation_vertex_base ;
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
typedef CGAL::Polygon_2<Kernel>                                                       Polygon_2;



void mark_domains(CDT& ct, CDT::Face_handle start, int index, std::list<CDT::Edge>& border) {
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
	for (CDT::All_faces_iterator it = cdt.all_faces_begin(); it
			!= cdt.all_faces_end(); ++it) {
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

void insert_polygon( CDT& cdt, const Polygon_2& polygon ) {
	if (polygon.is_empty())
		return;
	CDT::Vertex_handle v_prev = cdt.insert( *CGAL::cpp0x::prev( polygon.vertices_end() ) );
	for (Polygon_2::Vertex_iterator vit = polygon.vertices_begin(); vit != polygon.vertices_end(); ++vit) {
		CDT::Vertex_handle vh = cdt.insert(*vit);
		cdt.insert_constraint(vh, v_prev);
		v_prev = vh;
	}
}

int main() {
	//construct two non-intersecting nested polygons
	Polygon_2 polygon1;
	polygon1.push_back(Point_2(0.0, 0.0));
	polygon1.push_back(Point_2(2.0, 0.0));
	polygon1.push_back(Point_2(1.7, 1.0));
	polygon1.push_back(Point_2(2.0, 2.0));
	polygon1.push_back(Point_2(0.0, 2.0));
	Polygon_2 polygon2;
	polygon2.push_back(Point_2(0.5, 0.5));
	polygon2.push_back(Point_2(1.5, 0.5));
	polygon2.push_back(Point_2(1.5, 1.5));
	polygon2.push_back(Point_2(0.5, 1.5));

	//Insert the polyons into a constrained triangulation
	CDT cdt;
	insert_polygon(cdt, polygon1);
	insert_polygon(cdt, polygon2);

	//Extract point and provide the an index
	std::vector< triangulation_point > points ;
	for ( CDT::Vertex_iterator it = cdt.vertices_begin(); it != cdt.vertices_end(); ++it ){
		it->info() = points.size() ;
		points.push_back( it->point() );
	}


	//Mark facets that are inside the domain bounded by the polygon
	mark_domains(cdt);

	//
	int count = 0;
	for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit) {
		if (fit->info().in_domain()){
			++count;
		}
	}


	/*
	 * export
	 */

	std::ofstream ofs("polygon_triangulation2.obj");
	if ( ! ofs.good() ){
		std::cout << "can't open file" << std::endl;
		return 1 ;
	}

	//-- print vertices
	ofs << "# " << points.size() << " vertices"<< std::endl ;
	for ( size_t i = 0; i < points.size(); i++ ){
		ofs << "v " << points[i] << " 0.0" << std::endl;
	}

	//-- print faces
	ofs << "# " << cdt.number_of_faces() << " faces"<< std::endl ;
	// warning : Delaunay_triangulation_2::All_faces_iterator iterator over infinite faces
	for ( CDT::Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end(); ++it )
	{
		//ignore holes
		if ( ! it->info().in_domain() ){
			continue ;
		}
		size_t ia = it->vertex(0)->info();
		size_t ib = it->vertex(1)->info();
		size_t ic = it->vertex(2)->info();

		assert( it->is_valid() );
		//assert ( ia < cdt.number_of_vertices() || ib < tri.number_of_vertices() || ic < tri.number_of_vertices() ) ;

		ofs << "f " << ( ia + 1 ) << " " << ( ib + 1 ) << " " << ( ic + 1 ) << std::endl;
	}

	return 0;
}
