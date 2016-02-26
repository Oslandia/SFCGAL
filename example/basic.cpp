#include <iostream>

#include <SFCGAL/Geometry.h>

using namespace SFCGAL ;

int main( int argc, char* argv[] ){
    Point<Epeck> a(0.0,0.0,1.0);
    Point<Epeck> b(1.0,0.0,0.5);
    Point<Epeck> c(0.0,1.0,0.0);
    
    Triangle<Epeck> triangle(a,b,c) ;
    std::cout << CGAL::to_double( triangle.squared_area() ) << std::endl ;
    std::cout << CGAL::to_double( triangle.squared_area() ) << std::endl ;
    
    CGAL::Plane_3<Epeck> plane = triangle.supporting_plane() ;
    Point<Epeck> projection = plane.projection( Point<Epeck>(5.0,5.0,5.0) );
    std::cout << projection << std::endl;
    return 0 ;
}

