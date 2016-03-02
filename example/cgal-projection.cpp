#include <iostream>

#include <SFCGAL/Geometry.h>

using namespace SFCGAL ;

int main( int argc, char* argv[] ){
    CGAL::Point_3<Epeck> a(0.0,0.0,1.0);
    CGAL::Point_3<Epeck> b(1.0,0.0,0.5);
    CGAL::Point_3<Epeck> c(0.0,1.0,0.0);

    CGAL::Triangle_3<Epeck> triangle(a,b,c) ;
    std::cout << CGAL::to_double( triangle.squared_area() ) << std::endl ;
    std::cout << CGAL::to_double( triangle.squared_area() ) << std::endl ;
    
    CGAL::Plane_3<Epeck> plane = triangle.supporting_plane() ;
    CGAL::Point_3<Epeck> projection = plane.projection( CGAL::Point_3<Epeck>(5.0,5.0,5.0) );
    std::cout << projection << std::endl;
    return 0 ;
}
