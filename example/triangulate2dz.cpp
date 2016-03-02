#include <iostream>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/algorithm/triangulate2DZ.h>
#include <SFCGAL/log.h>

#include <CGAL/point_generators_2.h>
#include <CGAL/point_generators_3.h>

using namespace SFCGAL ;

template < typename K, int N >
struct MultiPointGenerator {

} ;


template < typename K >
struct MultiPointGenerator<K,2> {
    MultiPoint<K,2> generate( const int & n) const {
        CGAL::default_random = CGAL::Random(0);
        typedef CGAL::Creator_uniform_2< double, Point<K,2> > Creator ;
        CGAL::Random_points_in_disc_2< Point<K,2>, Creator > g( 150.0 );
        MultiPoint<K,2> points;
        points.reserve(n);
        CGAL::cpp11::copy_n( g, n, std::back_inserter(points));
        return points ;
    }
} ;

template < typename K >
struct MultiPointGenerator<K,3> {
    MultiPoint<K,3> generate( const int & n) const {
        CGAL::default_random = CGAL::Random(0);
        typedef CGAL::Creator_uniform_3< double, Point<K,3> > Creator ;
        CGAL::Random_points_in_sphere_3< Point<K,3>, Creator > g( 150.0 );
        MultiPoint<K,3> points;
        points.reserve(n);
        CGAL::cpp11::copy_n( g, n, std::back_inserter(points));
        return points ;
    }
} ;


template < typename K, int N >
MultiPoint<K,N> generateMultiPoint(const int & n){
    MultiPointGenerator<K,N> generator;
    return generator.generate(n);
}



int main( int argc, char* argv[] ){
    int n = 10000 ;

    BOOST_LOG_TRIVIAL(info) << "2D triangulation..." ;
    {
        BOOST_LOG_TRIVIAL(info) << "generate " << n << " points..." ;
        MultiPoint<Epick,2> g = generateMultiPoint<Epick,2>(n);
        BOOST_LOG_TRIVIAL(info) << "algorithm::triangulate2DZ..." ;
        TriangulatedSurface<Epick,2> tin = algorithm::triangulate2DZ<Epick,2>( g );
        BOOST_LOG_TRIVIAL(info) << "TIN : " << tin.size() ;
    }

    BOOST_LOG_TRIVIAL(info) << "2DZ triangulation..." ;
    {
        BOOST_LOG_TRIVIAL(info) << "generate " << n << " points..." ;
        MultiPoint<Epick,3> g = generateMultiPoint<Epick,3>(n);
        BOOST_LOG_TRIVIAL(info) << "algorithm::triangulate2DZ..." ;
        TriangulatedSurface<Epick,3> tin = algorithm::triangulate2DZ<Epick,3>( g );
        BOOST_LOG_TRIVIAL(info) << "TIN : " << tin.size() ;
    }

    return 0 ;
}
