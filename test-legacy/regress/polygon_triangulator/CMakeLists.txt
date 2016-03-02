#-- polygon_triangulator test
file( GLOB_RECURSE SFCGAL_REGRESS_POLYGON_TRIANGULATOR_TEST_SOURCES *.cpp )

set( REGRESS_NAME test-regress-polygon_triangulator )
add_executable( ${REGRESS_NAME} ${SFCGAL_REGRESS_POLYGON_TRIANGULATOR_TEST_SOURCES} )

target_link_libraries( ${REGRESS_NAME}
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
)
if( ${SFCGAL_WITH_MPFR} )
  target_link_libraries( ${REGRESS_NAME} ${MPFR_LIBRARIES} )
endif( ${SFCGAL_WITH_MPFR} )

set_target_properties( ${REGRESS_NAME} PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS ${REGRESS_NAME} DESTINATION bin )



