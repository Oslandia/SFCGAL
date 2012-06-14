#-- build unit tests
file( GLOB_RECURSE SFCGAL_UNIT_TEST_SOURCES *.cpp )
add_executable( test-SFCGAL ${SFCGAL_UNIT_TEST_SOURCES} )
target_link_libraries( test-SFCGAL 
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
)
set_target_properties( test-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS test-SFCGAL DESTINATION bin )

