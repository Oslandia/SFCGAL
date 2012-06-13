set( EXAMPLE_NAMES
	basic_manip
	cartesian_kernel
	point_generator
	triangulation2
	polygon_triangulation2
	save_obj_file
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

