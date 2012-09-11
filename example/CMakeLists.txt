set( EXAMPLE_NAMES
	CGAL-basic_manip
	CGAL-cartesian_kernel
	CGAL-point_generator
	CGAL-triangulation2
	CGAL-polygon_triangulation2
	
	SFCGAL-save_obj_file
	
	BOOST-transform-iterator
)
foreach( EXAMPLE_NAME ${EXAMPLE_NAMES} )
	file( GLOB_RECURSE EXAMPLE_SOURCES ${EXAMPLE_NAME}/*.cpp )
	add_executable( example-${EXAMPLE_NAME} ${EXAMPLE_SOURCES} )
	target_link_libraries( example-${EXAMPLE_NAME} 
		SFCGAL
		${CGAL_LIBRARIES}
		${MPFR_LIBRARIES} 
		${GMP_LIBRARIES}
		${Boost_LIBRARIES}
	)
	install( TARGETS example-${EXAMPLE_NAME} DESTINATION bin )
endforeach()

