#include <iostream>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/triangulate/triangulate2DZ.h>
#include <SFCGAL/log.h>

#include <CGAL/point_generators_2.h>
#include <CGAL/point_generators_3.h>

using namespace SFCGAL ;


template < typename K >
MultiPoint<K> generateMultiPoint(const int & n){
    CGAL::default_random = CGAL::Random(0);
    typedef CGAL::Creator_uniform_3< double, Point<K> > Creator ;
    CGAL::Random_points_in_sphere_3< Point<K>, Creator > g( 150.0 );
    MultiPoint<K> points;
    points.reserve(n);
    CGAL::cpp11::copy_n( g, n, std::back_inserter(points));
    return points ;
}



int main( int argc, char* argv[] ){
    int n = 10000 ;

    BOOST_LOG_TRIVIAL(info) << "2DZ triangulation..." ;
    {
        BOOST_LOG_TRIVIAL(info) << "generate " << n << " points..." ;
        MultiPoint<Epick> g = generateMultiPoint<Epick>(n);
        BOOST_LOG_TRIVIAL(info) << "algorithm::triangulate2DZ..." ;
        TriangulatedSurface<Epick> tin = algorithm::triangulate2DZ<Epick>( g );
        BOOST_LOG_TRIVIAL(info) << "TIN : " << tin.size() ;
    }

    return 0 ;
}
