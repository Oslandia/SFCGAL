file( GLOB SFCGAL_BENCHS_SOURCES *.cpp )
add_executable( bench-SFCGAL ${SFCGAL_BENCHS_SOURCES} )

find_package(Boost REQUIRED COMPONENTS unit_test_framework timer serialization)

target_link_libraries( bench-SFCGAL SFCGAL_static ${Boost_LIBRARIES})
set_target_properties( bench-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS bench-SFCGAL DESTINATION bin )

