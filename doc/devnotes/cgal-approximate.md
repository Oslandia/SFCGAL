# Approximate computation

## before

```
double length3D( const LineString& g )
{
    double result = 0.0 ;

    for ( size_t i = 0; i < g.numSegments(); i++ ) {
        Epeck::Segment_3 segment(
            g.pointN( i ).toPoint_3(),
            g.pointN( i+1 ).toPoint_3()
        );
        result += CGAL::sqrt( CGAL::to_double( segment.squared_length() ) ) ;
    }

    return result ;
}
```

## after

http://doc.cgal.org/latest/Epeck_23/classCGAL_1_1Filtered__kernel.html#a12bfea68b2a93842fd576c914f8aaf1c


```
double length3D( const LineString& g )
{
    Epeck::Approximate_kernel::FT result = 0.0 ;

    for ( size_t i = 0; i < g.numSegments(); i++ ) {
        Epeck::Approximate_kernel::Segment_3 segment(
            CGAL::to_approx( g.pointN( i ).toPoint_3() ),
            CGAL::to_approx( g.pointN( i+1 ).toPoint_3() )
        );
        result += CGAL::sqrt( segment.squared_length() ) ;  
    }

    return CGAL::to_double( result ) ;
}
```


