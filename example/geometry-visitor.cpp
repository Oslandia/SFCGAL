#include <iostream>

#include <SFCGAL/Geometry.h>

using namespace SFCGAL ;

/**
 * Visitor that retrieves the geometry type
 */
class GeometryTypeVisitor : public boost::static_visitor< std::string > {
public:
    template < typename K, int N >
    std::string operator() ( const Point<K,N> & p ) const {
        return "Point" ;
    }
    template < typename K, int N >
    std::string operator() ( const Triangle<K,N> & p ) const {
        return "Triangle" ;
    }

    template < typename OtherType >
    std::string operator() ( const OtherType & other ) const {
        return "UnknownType" ;
    }
} ;

int main( int argc, char* argv[] ){
    Point<Epeck,3> a(0.0,0.0,1.0);
    Point<Epeck,3> b(1.0,0.0,0.5);
    Point<Epeck,3> c(0.0,1.0,0.0);
    Triangle<Epeck,3> triangle(a,b,c) ;
    Polygon<Epeck,3> polygon ;

    std::vector< Geometry<Epeck,3> > geometries ;
    geometries.push_back(a);
    geometries.push_back(b);
    geometries.push_back(triangle);
    geometries.push_back(polygon);

    for ( const Geometry<Epeck,3> & geometry : geometries ){
        std::cout << boost::apply_visitor(GeometryTypeVisitor(),geometry) << std::endl;
    }

    return 0 ;
}
