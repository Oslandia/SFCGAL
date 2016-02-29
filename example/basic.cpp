#include <iostream>

#include <SFCGAL/Geometry.h>

using namespace SFCGAL ;

typedef Point<Epeck,3>    Point_3 ;
typedef Triangle<Epeck,3> Triangle_3 ;

typedef CGAL::Plane_3<Epeck> Plane_3 ;

int main( int argc, char* argv[] ){
    Point_3 a(0.0,0.0,1.0);
    Point_3 b(1.0,0.0,0.5);
    Point_3 c(0.0,1.0,0.0);

    Triangle_3 triangle(a,b,c) ;
    std::cout << CGAL::to_double( triangle.squared_area() ) << std::endl ;
    std::cout << CGAL::to_double( triangle.squared_area() ) << std::endl ;

    Plane_3 plane = triangle.supporting_plane() ;
    Point_3 projection = plane.projection( Point_3(5.0,5.0,5.0) );
    std::cout << projection << std::endl;
    return 0 ;
}
