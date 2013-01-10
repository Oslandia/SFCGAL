#-- configure tests
set( SFCGAL_TEST_DIRECTORY "${CMAKE_SOURCE_DIR}/test" )
configure_file( ${SFCGAL_TEST_DIRECTORY}/test_config.h.cmake ${SFCGAL_TEST_DIRECTORY}/test_config.h )

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions( "-frounding-math" )
endif(CMAKE_COMPILER_IS_GNUCXX)

#-- build unit tests
add_subdirectory( unit )

#-- build regression tests
add_subdirectory( regress )

option( Compile_Bench "Compile benchmarks" OFF )
if( ${Compile_Bench} )
	add_subdirectory( bench )
endif()



