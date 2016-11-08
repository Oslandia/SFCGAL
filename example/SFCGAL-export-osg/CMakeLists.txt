GET_FILENAME_COMPONENT( EXAMPLE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME )
add_executable( example-${EXAMPLE_NAME} 
	main.cpp 
)
target_link_libraries( example-${EXAMPLE_NAME}
        SFCGAL
        SFCGAL-osg
	${CGAL_LIBRARIES}
	${MPFR_LIBRARIES} 
	${GMP_LIBRARIES}
	${OPENSCENEGRAPH_LIBRARIES}
	${Boost_LIBRARIES}
)
set_target_properties( example-${EXAMPLE_NAME} PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS example-${EXAMPLE_NAME} DESTINATION bin )
