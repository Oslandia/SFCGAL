#-- build unit tests
file( GLOB_RECURSE TOOL_SOURCES *.cpp )

set( TOOLNAME wkt2obj )
add_executable( ${TOOLNAME} ${TOOL_SOURCES} )
target_link_libraries( ${TOOLNAME}
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
)
set_target_properties( ${TOOLNAME} PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS ${TOOLNAME} DESTINATION bin )

