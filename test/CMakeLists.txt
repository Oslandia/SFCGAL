#-- configure tests
set( SFCGAL_TEST_DIRECTORY "${CMAKE_SOURCE_DIR}/test" )
configure_file( ${CMAKE_SOURCE_DIR}/test/test_config.h.cmake ${CMAKE_SOURCE_DIR}/test/test_config.h )

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions( "-frounding-math" )
endif(CMAKE_COMPILER_IS_GNUCXX)

#-- build unit tests
file( GLOB_RECURSE SFCGAL_UNIT_TEST_SOURCES unit/*.cpp )
add_executable( test-SFCGAL ${SFCGAL_UNIT_TEST_SOURCES} )
target_link_libraries( test-SFCGAL 
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
)
set_target_properties( test-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS test-SFCGAL DESTINATION bin )


#-- build regression tests
file( GLOB_RECURSE SFCGAL_REGRESS_TEST_SOURCES regress/*.cpp )
add_executable( test-regress-SFCGAL ${SFCGAL_REGRESS_TEST_SOURCES} )
target_link_libraries( test-regress-SFCGAL 
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
)
set_target_properties( test-regress-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS test-regress-SFCGAL DESTINATION bin )
