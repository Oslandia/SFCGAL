#include <iostream>

#include <SFCGAL/kernels.h>
#include <SFCGAL/Point.h>

using namespace SFCGAL ;

namespace wkt {
    template < typename K, int N >
    std::string encode( const Point<K,N> & g ){
        std::ostringstream oss ;
        oss << "POINT(";
        oss << g ;
        oss << ")";
        return oss.str();
    }
}


int main( int argc, char* argv[] ){
    typedef Point<Epeck,2>  PointXY ;
    typedef Point<Epeck,3> PointXYZ ;

    PointXY a(3.0,4.0);
    std::cout << wkt::encode<Epeck,2>(a) << std::endl;

    PointXYZ b(3.0,4.0,5.0);
    std::cout << wkt::encode<Epeck,3>(b) << std::endl;

    return 0 ;
}
