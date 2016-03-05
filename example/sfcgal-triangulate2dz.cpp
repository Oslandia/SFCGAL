#include <iostream>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/triangulate/triangulate2DZ.h>
#include <SFCGAL/log.h>

#include <CGAL/point_generators_3.h>

#ifdef SFCGAL_WITH_OSG
#include <SFCGAL/io/osg.h>
#endif

using namespace SFCGAL ;


template < typename K >
MultiPoint<K> generateMultiPoint(const int & n){
    CGAL::default_random = CGAL::Random(0);
    typedef CGAL::Creator_uniform_3< double, Point<K> > Creator ;
    CGAL::Random_points_in_sphere_3< Point<K>, Creator > g( 150.0 );
    MultiPoint<K> points;
    points.reserve(n);
    CGAL::cpp11::copy_n( g, n, std::back_inserter(points));
    // squeeze Z
    for ( Point<K> & p : points ){
        p = Point<K>(p.x(),p.y(),p.z()/100.0);
    }
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
