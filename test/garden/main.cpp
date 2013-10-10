/**
 *   SFCGAL
 *
 *   Copyright (C) 2012-2013 Oslandia <infos@oslandia.com>
 *   Copyright (C) 2012-2013 IGN (http://www.ign.fr)
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <fstream>
#include <set>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/chrono.hpp>

#include <SFCGAL/Exception.h>
#include <SFCGAL/Point.h>
#include <SFCGAL/LineString.h>
#include <SFCGAL/Polygon.h>
#include <SFCGAL/Triangle.h>
#include <SFCGAL/PolyhedralSurface.h>
#include <SFCGAL/TriangulatedSurface.h>
#include <SFCGAL/Solid.h>
#include <SFCGAL/GeometryCollection.h>
#include <SFCGAL/MultiPoint.h>
#include <SFCGAL/MultiLineString.h>
#include <SFCGAL/MultiPolygon.h>
#include <SFCGAL/MultiSolid.h>
#include <SFCGAL/version.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/vtk.h>
#include <SFCGAL/detail/TestGeometry.h>
#include <SFCGAL/algorithm/area.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/algorithm/intersection.h>
#include <SFCGAL/algorithm/plane.h>
#include <SFCGAL/algorithm/minkowskiSum.h>
#include <SFCGAL/algorithm/tesselate.h>
#include <SFCGAL/algorithm/isValid.h>
#include <SFCGAL/algorithm/distance.h>
#include <SFCGAL/algorithm/distance3d.h>
#include <SFCGAL/algorithm/straightSkeleton.h>
#include <SFCGAL/algorithm/extrude.h>
#include <SFCGAL/triangulate/triangulate2DZ.h>
#include <SFCGAL/PreparedGeometry.h>

using namespace SFCGAL ;

namespace po = boost::program_options ;

/*
 * Monkey testing of the SFCGAL pulic api
 */
int main( int argc, char* argv[] )
{
    /*
     * declare options
     */
    po::options_description desc( "garden test options : " );
    desc.add_options()
    ( "help", "produce help message" )
    ( "progress", "display progress" )
    ( "verbose",  "verbose mode" )
    ;

    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );

    if ( vm.count( "help" ) ) {
        std::cout << desc << std::endl ;
        return 0;
    }

    bool verbose  = vm.count( "verbose" ) != 0 ;
    bool progress = vm.count( "progress" ) != 0 ;


    //boost::timer timer ;
    boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

    if ( verbose ) {
        std::cout << "testing SFCGAL version " << Version() << "\n";
    }


    // construction of a list valid and invalid test geometries
    GeometryCollection testCollection;
    {
        if ( verbose ) {
            std::cout << "creating test geometries\n";
        }

        // start with a collection of valid and invalid geometries
        const std::vector< TestGeometry > testGeom( createTestGeometries() ) ;

        for ( std::vector< TestGeometry >::const_iterator tg=testGeom.begin();
                tg!=testGeom.end(); ++tg ) {
            std::auto_ptr< Geometry > g( io::readWkt( tg->wkt ) );
            testCollection.addGeometry( g.release() );
        }

        // default constructed geometries
        testCollection.addGeometry( new Point ) ;
        testCollection.addGeometry( new LineString ) ;
        testCollection.addGeometry( new Triangle ) ;
        testCollection.addGeometry( new Polygon ) ;
        testCollection.addGeometry( new MultiLineString ) ;
        testCollection.addGeometry( new MultiPoint ) ;
        testCollection.addGeometry( new MultiPolygon ) ;
        testCollection.addGeometry( new PolyhedralSurface ) ;
        testCollection.addGeometry( new TriangulatedSurface ) ;
        testCollection.addGeometry( new Solid ) ;
        testCollection.addGeometry( new GeometryCollection ) ;

        // non default constructed
        testCollection.addGeometry( new Point( 67.8,-57.6568 ) ) ;
        testCollection.addGeometry( new Point( 67.8,-57.6568, 589.5679 ) ) ;
        testCollection.addGeometry( new Polygon( LineString() ) ) ;
        testCollection.addGeometry( new Solid( PolyhedralSurface() ) ) ;
    }

    // member function calls
    typedef GeometryCollection::const_iterator GeomIter;
    const size_t numGeom = testCollection.numGeometries();

    if ( verbose ) {
        std::cout << "testing member functions on " << numGeom << " geometries\n";
    }

    for ( size_t g=0; g!=numGeom; g++ ) {
        const Geometry& geom = testCollection.geometryN( g ); // make copy to modify
        ( void )geom.is3D() ;
        ( void )geom.isEmpty() ;

        {
            srid_t srid = 12; // whatever
            PreparedGeometry prep( geom.clone(), srid );
            ( void )prep.geometry();
            BOOST_ASSERT( prep.SRID() == srid );
        }

        if ( verbose ) {
            std::cout << ( boost::format( "% 8d" )%g ) << " - " << ( geom.is3D()?"3D":"2D" ) << " " << geom.geometryType() << ( geom.isEmpty()?" (empty)":"" ) << ( algorithm::isValid( geom )?"":" (invalid)" );
        }

        switch ( geom.geometryTypeId() ) {
        case TYPE_POINT : {
            Point p( geom.as<Point>() ) ;

            try {
                ( void )p.x();
                ( void )p.y();
                ( void )p.z();
            }
            catch ( Exception ) {
                BOOST_ASSERT( p.isEmpty() );
            }
        }
        break;
        case TYPE_LINESTRING : {
            LineString l( geom.as<LineString>() );
            const size_t numPoints = l.numPoints();

            if ( verbose ) {
                std::cout << ", " << numPoints << " points";
            }

            for ( GeomIter i=testCollection.begin(); i!=testCollection.end(); ++i ) {
                if ( i->is<Point>() ) {
                    l.addPoint( i->as<Point>() );
                }
            }

            if ( verbose ) {
                std::cout << ", added " << l.numPoints()-numPoints << " points";
            }

            ( void )l.pointN( 0 );
            ( void )l.pointN( l.numPoints()-1 );
#               ifdef DEBUG
            bool threw = false ;

            try {
                ( void )l.pointN( l.numPoints() );
            }
            catch ( std::exception ) {
                threw = true;
            }

            BOOST_ASSERT( threw );
#               endif
        }
        break;
        case TYPE_POLYGON : {
            Polygon p( geom.as<Polygon>() ) ;

            for ( GeomIter i=testCollection.begin(); i!=testCollection.end(); ++i ) {
                if ( i->is<LineString>() ) {
                    p.addRing( i->as<LineString>() );
                    ( void )Polygon(  i->as<LineString>() );
                }
            }

            ( void )p.exteriorRing();

            if ( p.numInteriorRings() ) {
                ( void )p.interiorRingN( 0 );
                ( void )p.interiorRingN( p.numInteriorRings()-1 );
            }

            ( void )p.isCounterClockWiseOriented();
#               ifdef DEBUG
            bool threw = false ;

            try {
                ( void )p.interiorRingN( p.numInteriorRings() );
            }
            catch ( std::exception ) {
                threw = true;
            }

            BOOST_ASSERT( threw );
#               endif
        }
        break;
        case TYPE_MULTIPOINT :
            geom.as<MultiPoint>() ;
            break;
        case TYPE_MULTILINESTRING :
            geom.as<MultiLineString>() ;
            break;
        case TYPE_MULTIPOLYGON :
            geom.as<MultiPolygon>() ;
            break;
        case TYPE_GEOMETRYCOLLECTION :
            geom.as<GeometryCollection>() ;
            break;
        case TYPE_POLYHEDRALSURFACE : {
            PolyhedralSurface s( geom.as<PolyhedralSurface>() ) ;

            for ( GeomIter i=testCollection.begin(); i!=testCollection.end(); ++i ) {
                if ( i->is<Polygon>() ) {
                    s.addPolygon( i->as<Polygon>() );
                }
            }

            if ( s.numPolygons() ) {
                ( void )s.polygonN( 0 );
                ( void )s.polygonN( s.numPolygons()-1 );
            }

#               ifdef DEBUG
            bool threw = false ;

            try {
                ( void )s.polygonN( s.numPolygons() );
            }
            catch ( std::exception ) {
                threw = true;
            }

            BOOST_ASSERT( threw );
#               endif
        }
        break;
        case TYPE_TRIANGULATEDSURFACE : {
            TriangulatedSurface s( geom.as<TriangulatedSurface>() ) ;

            for ( GeomIter i=testCollection.begin(); i!=testCollection.end(); ++i ) {
                if ( i->is<Triangle>() ) {
                    s.addTriangle( i->as<Triangle>() );
                }
            }

            if ( s.numTriangles() ) {
                ( void )s.triangleN( 0 );
                ( void )s.triangleN( s.numTriangles()-1 );
            }

#               ifdef DEBUG
            bool threw = false ;

            try {
                ( void )s.triangleN( s.numTriangles() );
            }
            catch ( std::exception ) {
                threw = true;
            }

            BOOST_ASSERT( threw );
#               endif
        }
        break;
        case TYPE_TRIANGLE : {
            Triangle t( geom.as<Triangle>() ) ;

            for ( int j=0; j<3; ++j ) {
                ( void )geom.as<Triangle>().vertex( j ) ; // to execise const

                for ( GeomIter i=testCollection.begin(); i!=testCollection.end(); ++i ) {
                    if ( i->is<Point>() ) {
                        t.vertex( j ) = i->as<Point>() ;
                        BOOST_ASSERT( t.vertex( j ) ==  i->as<Point>() );
                    }
                }
            }
        }
        break;
        case TYPE_SOLID : {
            Solid s( geom.as<Solid>() ) ;

            for ( GeomIter i=testCollection.begin(); i!=testCollection.end(); ++i ) {
                if ( i->is<PolyhedralSurface>() ) {
                    ( void )Solid( i->as<PolyhedralSurface>() );
                }
            }

            if ( s.numShells() ) {
                ( void )s.shellN( 0 );
                ( void )s.shellN( s.numShells()-1 );
            }

#               ifdef DEBUG

            try {
                ( void )s.shellN( s.numShells() );
            }
            catch ( std::exception ) {
                threw = true;
            }

            BOOST_ASSERT( threw );
#               endif
        }
        break;
        case TYPE_MULTISOLID :
            geom.as<MultiSolid>() ;
            break;
        }

        if ( verbose ) {
            std::cout << "\n";
        }
    }

    size_t numFailure = 0;
    size_t calls = 0;

    // function calls
    std::set< std::string > notImplemented ;
#define CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( call )\
    {\
    ++calls ;\
    try{ call }\
    catch ( GeometryInvalidityException ) {}\
    catch ( InappropriateGeometryException ) {}\
    catch ( NotImplementedException e ) { notImplemented.insert(e.what()) ; }\
    catch ( std::exception& e )\
    { \
        try{\
        ++numFailure;\
        std::cerr << "\n" ;\
        std::cerr << "error " << numFailure << " in " << #call << ": "\
                  << e.what() << "\n";\
        std::cerr << "error with " << (algorithm::isValid(*geom1)?"valid":"invalid")\
                  << " geometry " << geom1->asText() ; \
        io::vtk( *geom1, (boost::format("/tmp/geom1_failure%d.vtk") % numFailure).str() );\
        if (geom2!=testCollection.end() ) {\
            std::cerr << " and " << (algorithm::isValid(*geom1)?"valid":"invalid")\
                      << " geometry " << geom2->asText();\
            io::vtk( *geom2, (boost::format("/tmp/geom2_failure%d.vtk") % numFailure).str() );\
        }\
        std::cerr << "\n";\
        }\
        catch ( NotImplementedException e ) { notImplemented.insert(e.what()) ; }\
    }\
    }

    for ( GeomIter geom1=testCollection.begin(); geom1!=testCollection.end(); ++geom1 ) {

        GeomIter geom2 = testCollection.end();

        CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::area3D( *geom1 ) ; )
        CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::area( *geom1 ) ; )

        if ( geom1->is<Polygon>() ) {
            CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::hasPlane3D<Kernel>( geom1->as<Polygon>() ) ; )
        }

        CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::straightSkeleton( *geom1 ) ; )
        CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::tesselate( *geom1 ) ; )

        CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED(
            triangulate::ConstraintDelaunayTriangulation cdt;
            triangulate::triangulate2DZ( *geom1, cdt );
            TriangulatedSurface surf;
            cdt.getTriangles( surf );
        )

        for ( geom2=testCollection.begin() ; geom2!=testCollection.end(); ++geom2 ) {
            if ( geom2->is<Point>() && !geom2->isEmpty() ) {
                CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED(
                    const Point& p = geom2->as<Point>() ;
                    ( void )algorithm::extrude( *geom1, p.x(), p.y(), p.z() ) ;
                )
            }

            CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::distance3D( *geom1, *geom2 ) ; )
            CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::distance( *geom1, *geom2 ) ; )
            CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::intersection3D( *geom1, *geom2 ) ; )
            CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::intersection( *geom1, *geom2 ) ; )
            CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::intersects3D( *geom1, *geom2 ) ; )
            CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::intersects( *geom1, *geom2 ) ; )

            if ( geom2->is<Polygon>() ) {
                CATCH_INVALID_GEOM_AND_NOT_IMPLEMENTED( ( void )algorithm::minkowskiSum( *geom1, geom2->as<Polygon>() ) ; )
            }
        }

        if ( progress || verbose ) {
            std::cout << "performed " << calls << " function calls\n";
        }
    }

    if ( !notImplemented.empty() ) {
        std::cout << "Missing implementations\n";

        for ( std::set< std::string >::const_iterator i=notImplemented.begin();
                i!=notImplemented.end(); ++i ) {
            std::cout << "    " << *i << "\n" ;
        }
    }

    boost::chrono::duration<double> elapsed = boost::chrono::system_clock::now() - start;

    if ( verbose ) {
        std::cout << "elapsed " << elapsed << "\n";
    }

    if ( numFailure ) {
        std::cerr << "\n\n**** " << numFailure << " test failed\n";
    }

    return numFailure==0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Functions called from lwgeom_sfcgal.c and corresponding C++ api calls
 *
sfcgal_geometry_collection_add_geometry(ret_geom, g)
    *GeometryCollection::addGeometry(geom)
sfcgal_geometry_collection_create()
    *GeometryCollection::GeometryCollection()
sfcgal_geometry_collection_geometry_n(geom, i)
    *GeometryCollection::geometryN(i)
sfcgal_geometry_collection_num_geometries(geom)
    *GeometryCollection::numGeometries()
sfcgal_geometry_delete(converted)
    *Geometry::~Geometry()
sfcgal_geometry_is_3d(geom)
    *Geometry::is3D()
sfcgal_geometry_is_empty(geom)
    *Geometry::isEmpty()
sfcgal_linestring_add_point(line,
    *LineString::addPoint()
sfcgal_linestring_create()
    *LineString::LineString()
sfcgal_linestring_num_points(geom)
    *LineString::numPoints()
sfcgal_linestring_point_n(geom, i)
    *LineString::pointN(i)
sfcgal_multi_linestring_create()
    *MultiLineString::MultiLineString()
sfcgal_multi_point_create()
    *MultiPoint::MultiPoint()
sfcgal_multi_polygon_create()
    *MultiPolygon::MultiPolygon()
sfcgal_point_create()
    *Point::Point()
sfcgal_point_create_from_xy(point.x, point.y)
    *Point::Point(x,y)
sfcgal_point_create_from_xyz(point.x, point.y, point.z)
    *Point::Point(x,y,z)
sfcgal_point_x(pt)
    *Point::x()
sfcgal_point_y(pt)
    *Point::y()
sfcgal_point_z(pt)
    *Point::z()
sfcgal_polygon_add_interior_ring(ret_geom, ring)
    *Polygon::addRing( ring )
sfcgal_polygon_create()
    *Polygon::Polygon()
sfcgal_polygon_create_from_exterior_ring(exterior_ring)
    *Polygon::Polygon(exterior_ring)
sfcgal_polygon_exterior_ring(geom)
    *Polygon::exteriorRing()
sfcgal_polygon_interior_ring_n(geom, i)
    *Polygon::interiorRingN(i)
sfcgal_polygon_num_interior_rings(geom)
    *Polygon::numInteriorRings()
sfcgal_polyhedral_surface_add_polygon(ret_geom, poly)
    *PolyhedralSurface::addPolygon(poly)
sfcgal_polyhedral_surface_create()
    *PolyhedralSurface::PolyhedralSurface()
sfcgal_polyhedral_surface_num_polygons(geom)
    *PolyhedralSurface::numPolygons()
sfcgal_polyhedral_surface_polygon_n( geom, i )
    *PolyhedralSurface::polygonN(i)
sfcgal_solid_create_from_exterior_shell(shell)
    *Solid::Solid(shell)
sfcgal_solid_num_shells(geom)
    *Solid::numShells()
sfcgal_solid_shell_n(geom, i)
    *Solid::shellN(i)
sfcgal_triangle_create()
    *Triangle::Triangle()
sfcgal_triangle_set_vertex_from_xy (triangle, i, point.x, point.y)
    *Triangle::vertex(i)
    *Point(x,y)
sfcgal_triangle_set_vertex_from_xyz(triangle, 0, point.x, point.y, point.z)
    *Triangle::vertex(i)
    *Point(x,y,z)
sfcgal_triangle_vertex(geom, (i%3))
    *Triangle::vertex(i) const
sfcgal_triangulated_surface_add_triangle(ret_geom, triangle)
    *TriangulatedSurface::addTriangle(triangle)
sfcgal_triangulated_surface_create()
    *TriangulatedSurface::TriangulatedSurface()
sfcgal_triangulated_surface_num_triangles(geom)
    *TriangulatedSurface::numTriangles()
sfcgal_triangulated_surface_triangle_n(geom, i)
    *TriangulatedSurface::triangleN(i)
sfcgal_version()
    *Version()
sfcgal_geometry_area_3d(geom)
    *algorithm::area3D(geom)
sfcgal_geometry_area(geom)
    *algorithm::area(geom)
sfcgal_geometry_distance_3d(geom0, geom1)
    *algorithm::distance3D(geom0, geom1)
sfcgal_geometry_distance(geom0, geom1)
    *algorithm::distance(geom0, geom1)
sfcgal_geometry_extrude(geom, dx, dy, dz)
    *algorithm::extrude(geom, dx, dy, dz)
sfcgal_geometry_force_lhr(geom)
    Geometry::clone(geom)
    Geometry::accept(SFCGAL::transform::ForceOrderPoints)
sfcgal_geometry_intersection_3d(geom0, geom1)
    *algorithm::intersection3D(geom0, geo1)
sfcgal_geometry_intersection(geom0, geom1)
    *algorithm::intersection(geom0, geo1)
sfcgal_geometry_intersects_3d(geom0, geom1)
    *algorithm::intersects3D(geom0, geo1)
sfcgal_geometry_intersects(geom0, geom1)
    *algorithm::intersects(geom0, geo1)
sfcgal_geometry_is_planar(geom)
    *algorithm::hasPlane3D< SFCGAL::Kernel >(Polygon)
sfcgal_geometry_minkowski_sum(geom0, geom1)
    *algorithm::minkowskiSum(geom0, Polygon)
sfcgal_geometry_orientation(geom)
    *Polygon::isCounterClockWiseOriented(poly)
sfcgal_geometry_straight_skeleton(geom)
    *algorithm::straightSkeleton(geom)
sfcgal_geometry_tesselate(geom)
    *algorithm::tesselate(geom)
sfcgal_geometry_triangulate_2dz(geom)
    *triangulate::ConstraintDelaunayTriangulation::ConstraintDelaunayTriangulation()
    *triangulate::triangulate2DZ( geom, ConstraintDelaunayTriangulation)
    *TriangulatedSurface::TriangulatedSurface() -> surf
    *triangulate::ConstraintDelaunayTriangulation::getTriangles( surf )
sfcgal_prepared_geometry_create_from_geometry(geom,srid)
    *PreparedGeometry::PreparedGeometry(geom, srid)
sfcgal_prepared_geometry_delete( geom )
    *PreparedGeometry::~PreparedGeometry()
sfcgal_prepared_geometry_geometry(geom)
    PreparedGeometry::geometry()
sfcgal_prepared_geometry_srid(geom))
    PreparedGeometry::SRID()

*/
