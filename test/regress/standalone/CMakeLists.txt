#-- standalone test
file( GLOB_RECURSE SFCGAL_REGRESS_STANDALONE_TEST_SOURCES *.cpp )
add_executable( standalone-regress-test-SFCGAL ${SFCGAL_REGRESS_STANDALONE_TEST_SOURCES} )

target_link_libraries( standalone-regress-test-SFCGAL 
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
)
if( ${SFCGAL_WITH_MPFR} )
  target_link_libraries( standalone-regress-test-SFCGAL ${MPFR_LIBRARIES} )
endif( ${SFCGAL_WITH_MPFR} )

set_target_properties( standalone-regress-test-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS standalone-regress-test-SFCGAL DESTINATION bin )



