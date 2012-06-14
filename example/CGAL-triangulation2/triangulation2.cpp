#include <fstream>


#include <SFCGAL/Kernel.h>


using namespace SFCGAL ;


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

