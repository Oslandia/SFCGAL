#-- bench
file( GLOB_RECURSE SFCGAL_BENCH_SOURCES *.cpp )
add_executable( test-bench-SFCGAL ${SFCGAL_BENCH_SOURCES} )

target_link_libraries( test-bench-SFCGAL 
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
)
if( ${SFCGAL_WITH_MPFR} )
  target_link_libraries( test-bench-SFCGAL ${MPFR_LIBRARIES} )
endif( ${SFCGAL_WITH_MPFR} )

set_target_properties( test-bench-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS test-bench-SFCGAL DESTINATION bin )
