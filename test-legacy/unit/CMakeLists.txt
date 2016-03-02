#-- build unit tests
file( GLOB_RECURSE SFCGAL_UNIT_TEST_SOURCES *.cpp )
add_executable( unit-test-SFCGAL ${SFCGAL_UNIT_TEST_SOURCES} )
target_link_libraries( unit-test-SFCGAL 
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
)
if( ${SFCGAL_WITH_MPFR} )
  target_link_libraries( unit-test-SFCGAL ${MPFR_LIBRARIES} ${GMP_LIBRARIES} )
endif( ${SFCGAL_WITH_MPFR} )


#include( PrecompiledHeader )
#if(PCHSupport_FOUND)
#  add_precompiled_header(unit-test-SFCGAL
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/all.h
##    "-fPIC"
#    )
#endif()

set_target_properties( unit-test-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS unit-test-SFCGAL DESTINATION bin )

if (CMAKE_BUILD_TYPE STREQUAL "Debug" )
    add_test( unit-testd ${CMAKE_CURRENT_BINARY_DIR}/unit-test-SFCGALd --auto_start_dbg=y --log_level=all)
else()
    add_test( unit-test ${CMAKE_CURRENT_BINARY_DIR}/unit-test-SFCGAL --auto_start_dbg=y --log_level=all)
endif()

