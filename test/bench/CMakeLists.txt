#-- build unit tests
set( SFCGAL_BENCHS 
	wkt 
	triangulation
)

foreach( SFCGAL_BENCH ${SFCGAL_BENCHS} )
	set( BENCH_NAME bench-SFCGAL-${SFCGAL_BENCH} )
	add_executable( ${BENCH_NAME} ${CMAKE_SOURCE_DIR}/test/bench/${SFCGAL_BENCH}.cpp )
	target_link_libraries( ${BENCH_NAME}
		SFCGAL
		${Boost_LIBRARIES}
		${CGAL_LIBRARIES}
	)
	if( ${SFCGAL_WITH_MPFR} )
	  target_link_libraries( ${BENCH_NAME} ${MPFR_LIBRARIES} )
	endif( ${SFCGAL_WITH_MPFR} )
	
	set_target_properties( ${BENCH_NAME} PROPERTIES DEBUG_POSTFIX "d" )
	install( TARGETS ${BENCH_NAME} DESTINATION bin )
endforeach()




