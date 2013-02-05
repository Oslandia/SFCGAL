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
#include <fstream>


#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>


typedef CGAL::Cartesian< double >            Kernel ;
typedef Kernel::Vector_2                     Vector_2 ;
typedef Kernel::Vector_3                     Vector_3 ;
typedef Kernel::Point_2                      Point_2 ;
typedef Kernel::Point_3                      Point_3 ;
typedef CGAL::Segment_2< Kernel >            Segment_2 ;
typedef CGAL::Segment_3< Kernel >            Segment_3 ;
typedef CGAL::Polygon_2< Kernel >            Polygon_2 ;
typedef CGAL::Polygon_with_holes_2< Kernel > Polygon_with_holes_2 ;


#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

//-- Info on vertex (unsigned int, an idenfier)
typedef CGAL::Triangulation_vertex_base_with_info_2< unsigned int, Kernel >  Vb ;
typedef CGAL::Triangulation_data_structure_2< Vb >                           Tds ;

//-- Triangulation
typedef CGAL::Delaunay_triangulation_2< Kernel, Tds >  Delaunay_triangulation_2 ;


//-- point generator
#include <CGAL/point_generators_2.h>
typedef CGAL::Creator_uniform_2< double, Point_2 >  Creator_2;




/**
 * Generate a terrain and export to wavefront obj format
 */
int main( int argc, char* argv[] )
{
	std::ofstream ofs("triangulation2.obj");
	if ( ! ofs.good() ){
		std::cout << "can't open file" << std::endl;
		return 1 ;
	}

	const size_t n = 1000 ;

	CGAL::Random_points_in_disc_2< Point_2, Creator_2 > g( 5.0 );

	//-- triangulate points
	Delaunay_triangulation_2 tri;

	// create points and associates ids
	std::vector< Point_2 > points;
	for ( size_t i = 0; i < n; i++ ){
		Point_2 point2d = *(++g) ;
		Delaunay_triangulation_2::Vertex_handle v = tri.insert( point2d );
		v->info() = i ;
		points.push_back( point2d );
	}


	// print vertices
	ofs << "# " << tri.number_of_vertices() << " vertices"<< std::endl ;
	for ( size_t i = 0; i < n; i++ ){
		ofs << "v " << points[i] << " 0.0" << std::endl;
	}

	// print faces
	ofs << "# " << tri.number_of_faces() << " faces"<< std::endl ;

	//-- print triangles
	// warning : Delaunay_triangulation_2::All_faces_iterator iterator over infinite faces
	for ( Delaunay_triangulation_2::Finite_faces_iterator it = tri.finite_faces_begin();
			it != tri.finite_faces_end(); ++it )
	{
		size_t ia = it->vertex(0)->info();
		size_t ib = it->vertex(1)->info();
		size_t ic = it->vertex(2)->info();

		assert( it->is_valid() );
		assert ( ia < tri.number_of_vertices() || ib < tri.number_of_vertices() || ic < tri.number_of_vertices() ) ;

		ofs << "f " << ( ia + 1 ) << " " << ( ib + 1 ) << " " << ( ic + 1 ) << std::endl;
	}


	return 0;
}

