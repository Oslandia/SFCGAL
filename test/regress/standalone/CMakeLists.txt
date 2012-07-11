#-- standalone test
file( GLOB_RECURSE SFCGAL_REGRESS_STANDALONE_TEST_SOURCES *.cpp )
add_executable( test-regress-SFCGAL ${SFCGAL_REGRESS_STANDALONE_TEST_SOURCES} )

target_link_libraries( test-regress-SFCGAL 
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
)
if( ${SFCGAL_WITH_MPFR} )
  target_link_libraries( test-regress-SFCGAL ${MPFR_LIBRARIES} )
endif( ${SFCGAL_WITH_MPFR} )

set_target_properties( test-regress-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS test-regress-SFCGAL DESTINATION bin )



