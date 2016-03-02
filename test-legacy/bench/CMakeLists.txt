file( GLOB SFCGAL_BENCHS_SOURCES *.cpp )
add_executable( bench-SFCGAL ${SFCGAL_BENCHS_SOURCES} )
target_link_libraries( bench-SFCGAL
       SFCGAL
       ${Boost_LIBRARIES}
       ${CGAL_LIBRARIES}
)
if( SFCGAL_WITH_MPFR )
	target_link_libraries( bench-SFCGAL ${MPFR_LIBRARIES} ${GMP_LIBRARIES} )
endif()
set_target_properties( bench-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS bench-SFCGAL DESTINATION bin )

