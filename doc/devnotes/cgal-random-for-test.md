# Generate the same random points in tests 


```
// reset random
CGAL::default_random = CGAL::Random(0);

typedef CGAL::Creator_uniform_2< double, CGAL::Point_2<K> > Creator ;
typedef CGAL::Point_2<K> Point_2 ;

CGAL::Random_points_in_disc_2< Point_2, Creator > g( 150.0 );
std::vector< Point_2 > points ;
points.reserve(N_POINTS);
CGAL::cpp11::copy_n( g, N_POINTS, std::back_inserter(points));
```
