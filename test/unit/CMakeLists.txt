#-- build unit tests

find_package(Boost REQUIRED COMPONENTS unit_test_framework serialization)

file( GLOB_RECURSE SFCGAL_UNIT_TEST_SOURCES *.cpp )
add_executable( unit-test-SFCGAL ${SFCGAL_UNIT_TEST_SOURCES} )
target_link_libraries( unit-test-SFCGAL SFCGAL_static)
target_link_libraries(unit-test-SFCGAL ${CGAL_3RD_PARTY_LIBRARIES} ${Boost_LIBRARIES})

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

