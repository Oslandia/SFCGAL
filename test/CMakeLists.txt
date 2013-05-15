#-- configure tests
set( SFCGAL_TEST_DIRECTORY "${CMAKE_SOURCE_DIR}/test" )
configure_file( ${SFCGAL_TEST_DIRECTORY}/test_config.h.cmake ${SFCGAL_TEST_DIRECTORY}/test_config.h )

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions( "-frounding-math" )
endif(CMAKE_COMPILER_IS_GNUCXX)

#-- build unit tests
if( SFCGAL_BUILD_UNIT_TESTS )
	add_subdirectory( unit )
endif()

#-- build regression tests
if( SFCGAL_BUILD_REGRESS_TESTS )
	add_subdirectory( regress )
endif()

if( SFCGAL_BUILD_BENCH )
	add_subdirectory( bench )
endif()



