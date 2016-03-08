#include <iostream>

#include <SFCGAL/Geometry.h>
#include <SFCGAL/io/wkt.h>

#include <SFCGAL/log.h>

using namespace SFCGAL ;

int main( int argc, char* argv[] ){
    std::vector< std::string > wkts{
        "POINT(6 10)",
        "LINESTRING(3 4,10 50,20 25)",
        "POLYGON((1 1,5 1,5 5,1 5,1 1))",
        "MULTIPOINT((3.5 5.6), (4.8 10.5))",
        "MULTILINESTRING((3 4,10 50,20 25),(-5 -8,-10 -8,-15 -4))",
        "MULTIPOLYGON(((1 1,5 1,5 5,1 5,1 1),(2 2,2 3,3 3,3 2,2 2)),((6 3,9 2,9 4,6 3)))",
        "GEOMETRYCOLLECTION(POINT(4 6),LINESTRING(4 6,7 10))",
        "POINT ZM (1 1 5 60)",
        "POINT M (1 1 80)",
        "POINT EMPTY",
        "MULTIPOLYGON EMPTY",
        "TRIANGLE EMPTY"
    };
    for ( const std::string & wkt : wkts ){
        BOOST_LOG_TRIVIAL(info) << "parse " << wkt ;
        std::unique_ptr< Geometry<Epick> > geometry = io::readWkt<Epick>(wkt);
        std::cout << io::toWkt<Epick>(*geometry) << std::endl ;
    }
    return 0 ;
}
