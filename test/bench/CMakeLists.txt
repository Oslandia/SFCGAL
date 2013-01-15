set(SFCGAL_HME_BENCHS
  IntersectionPerfTest.cpp
  KernelPerfTest.cpp
  BenchWKT.cpp
  BenchTriangulation.cpp
  TestModule.cpp
)
add_executable( bench-hme ${SFCGAL_HME_BENCHS} )
target_link_libraries( bench-hme
       SFCGAL
       ${Boost_LIBRARIES}
       ${CGAL_LIBRARIES}
)
if( ${SFCGAL_WITH_MPFR} )
	target_link_libraries( bench-hme ${MPFR_LIBRARIES} )
endif( ${SFCGAL_WITH_MPFR} )
set_target_properties( bench-hme PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS bench-hme DESTINATION bin )

