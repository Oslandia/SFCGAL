GET_FILENAME_COMPONENT( EXAMPLE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME )
add_executable( example-${EXAMPLE_NAME} 
	main.cpp 
)
target_link_libraries( example-${EXAMPLE_NAME} 
	SFCGAL
	${CGAL_LIBRARIES}
	${MPFR_LIBRARIES} 
	${GMP_LIBRARIES}
	${Boost_LIBRARIES}
)
if(SFCGAL_WITH_OSG)
	target_link_libraries( example-${EXAMPLE_NAME}
		${OPENSCENEGRAPH_LIBRARIES}
	)
endif()
set_target_properties( example-${EXAMPLE_NAME} PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS example-${EXAMPLE_NAME} DESTINATION bin )