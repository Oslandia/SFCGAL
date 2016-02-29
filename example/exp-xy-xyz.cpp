#include <iostream>

#include <SFCGAL/kernels.h>
#include <SFCGAL/CoordinateType.h>

using namespace SFCGAL ;

template < typename K, typename CT >
struct CoordinateTraits {
    
} ;

struct XY {} ;
template <typename K>
struct CoordinateTraits<K,XY> {
    static const int dimension = 2 ;
    
    typedef CGAL::Point_2<K> Point ;
} ;

struct XYZ {} ;
template <typename K>
struct CoordinateTraits<K,XYZ> {
    static const int dimension = 2 ;
    
    typedef CGAL::Point_3<K> Point ;
} ;


template < typename K, typename CT >
using Point = typename CoordinateTraits<K,CT>::Point ;

template < typename K >
struct Encoder {

    template < typename CT >
    std::string encode( const Point<K,CT> & g ){
        std::ostringstream oss ;
        oss << "POINT(";
        oss << g ;
        oss << ")";
        return oss.str();
    }    
        
} ;




int main( int argc, char* argv[] ){
    typedef Point<Epeck,XY>  PointXY ;
    typedef Point<Epeck,XYZ> PointXYZ ;
    
    Encoder<Epeck> encoder ;
    
    PointXY a(3.0,4.0);
    std::cout << encoder.encode<XY>(a) << std::endl;

    PointXYZ b(3.0,4.0,5.0);
    std::cout << encoder.encode<XYZ>(b) << std::endl;

    return 0 ;
}

