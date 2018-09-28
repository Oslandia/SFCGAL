#-- polygon_triangulator test
file( GLOB_RECURSE SFCGAL_REGRESS_CONVEX_HULL_TEST_SOURCES *.cpp )

set( REGRESS_NAME test-regress-convex_hull )
add_executable( ${REGRESS_NAME} ${SFCGAL_REGRESS_CONVEX_HULL_TEST_SOURCES} )

find_package(Boost REQUIRED COMPONENTS program_options filesystem serialization)

target_link_libraries( ${REGRESS_NAME} SFCGAL_static )

target_link_libraries( ${REGRESS_NAME} ${CGAL_3RD_PARTY_LIBRARIES} ${Boost_LIBRARIES})

set_target_properties( ${REGRESS_NAME} PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS ${REGRESS_NAME} DESTINATION bin )



