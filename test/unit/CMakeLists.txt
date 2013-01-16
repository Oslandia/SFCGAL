#-- build unit tests
file( GLOB_RECURSE SFCGAL_UNIT_TEST_SOURCES *.cpp )
add_executable( test-SFCGAL ${SFCGAL_UNIT_TEST_SOURCES} )
target_link_libraries( test-SFCGAL 
	SFCGAL
	${Boost_LIBRARIES}
	${CGAL_LIBRARIES}
)
if( ${SFCGAL_WITH_MPFR} )
  target_link_libraries( test-SFCGAL ${MPFR_LIBRARIES} ${GMP_LIBRARIES} )
endif( ${SFCGAL_WITH_MPFR} )


#include( PrecompiledHeader )
#if(PCHSupport_FOUND)
#  add_precompiled_header(test-SFCGAL
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/all.h
##    "-fPIC"
#    )
#endif()

set_target_properties( test-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS test-SFCGAL DESTINATION bin )

