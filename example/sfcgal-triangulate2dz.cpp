#include <iostream>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/triangulate/triangulate2DZ.h>
#include <SFCGAL/log.h>

#include <CGAL/point_generators_2.h>

#ifdef SFCGAL_WITH_OSG
#include <SFCGAL/io/osg.h>
#endif

using namespace SFCGAL ;


template < typename K >
MultiPoint<K> generateMultiPoint(const int & n){
    CGAL::default_random = CGAL::Random(0);
    typedef CGAL::Creator_uniform_2< double, CGAL::Point_2<K> > Creator ;
    CGAL::Random_points_in_disc_2< CGAL::Point_2<K>, Creator > g( 150.0 );
    MultiPoint<K> points;
    points.reserve(n);
    CGAL::cpp11::copy_n( g, n, std::back_inserter(points));
    return points ;
}



int main( int argc, char* argv[] ){
    int n = 10000 ;

    BOOST_LOG_TRIVIAL(info) << "generate " << n << " points..." ;
    MultiPoint<Epick> g = generateMultiPoint<Epick>(n);
    BOOST_LOG_TRIVIAL(info) << "algorithm::triangulate2DZ..." ;
    TriangulatedSurface<Epick> tin = triangulate::triangulate2DZ<Epick>( g );
    BOOST_LOG_TRIVIAL(info) << "TIN : " << tin.size() ;
    
#ifdef SFCGAL_WITH_OSG
    std::string path("sfcgal-triangulation2dz.osg");
    BOOST_LOG_TRIVIAL(info) << "save TIN to osgfile " << path << "..." ;
    SFCGAL::io::osgWriteFile<Epick>(tin,path);
#endif    

    return 0 ;
}
