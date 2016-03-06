#-- build unit tests
#-- TODO replace by an exhaustive list if absolute path is not enougth
#file( GLOB_RECURSE SFCGAL_UNIT_TEST_SOURCES "${CMAKE_SOURCE_DIR}/test/unit/*.cpp" )

set( SFCGAL_UNIT_TEST_SOURCES
    "TestModule.cpp"
    
    "SFCGAL/PointTest.cpp"
    "SFCGAL/LineStringTest.cpp"
    "SFCGAL/PolygonTest.cpp"
    "SFCGAL/MultiPointTest.cpp"
    "SFCGAL/GeometryCollectionTest.cpp"
    "SFCGAL/GeometryVisitorTest.cpp"
    
    "SFCGAL/io/WktReaderTest.cpp"
)

add_executable( unit-test-SFCGAL ${SFCGAL_UNIT_TEST_SOURCES} )
target_link_libraries( unit-test-SFCGAL 
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
    ${MPFR_LIBRARIES}
    ${GMP_LIBRARIES}
)

# mb : no need to install tests or to change the postfix
#set_target_properties( unit-test-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
#install( TARGETS unit-test-SFCGAL DESTINATION bin )
if (CMAKE_BUILD_TYPE STREQUAL "Debug" )
    add_test( unit-test ${CMAKE_CURRENT_BINARY_DIR}/unit-test-SFCGAL --auto_start_dbg=y --log_level=all)
else()
    add_test( unit-test ${CMAKE_CURRENT_BINARY_DIR}/unit-test-SFCGAL --log_level=all)
endif()
