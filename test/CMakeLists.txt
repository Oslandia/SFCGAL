#-- configure tests
set( SFCGAL_TEST_DIRECTORY "${CMAKE_SOURCE_DIR}/test" )
configure_file( ${SFCGAL_TEST_DIRECTORY}/test_config.h.cmake ${SFCGAL_TEST_DIRECTORY}/test_config.h )

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions( "-frounding-math" )
  
endif(CMAKE_COMPILER_IS_GNUCXX)

#-- build regression tests
if( SFCGAL_BUILD_TESTS )
	add_subdirectory( unit )
	add_subdirectory( regress )
    add_subdirectory( garden )
endif()

if( SFCGAL_BUILD_BENCH )
	add_subdirectory( bench )
endif()

#if(CMAKE_SYSTEM_NAME MATCHES "Linux")
#    add_test( style-test ${CMAKE_SOURCE_DIR}/script/test-style-SFCGAL.sh )
#endif()
  




