add_subdirectory( CGAL-basic_manip )
add_subdirectory( CGAL-cartesian_kernel )
add_subdirectory( CGAL-point_generator )
add_subdirectory( CGAL-triangulation2 )
add_subdirectory( CGAL-polygon_triangulation2 )
add_subdirectory( SFCGAL-offset )

if ( SFCGAL_BUILD_OSG )
	add_subdirectory( SFCGAL-export-osg )
	add_subdirectory( SFCGAL-asc2osg )
	add_subdirectory( SFCGAL-building )
endif()


