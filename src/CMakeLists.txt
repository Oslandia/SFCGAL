file( GLOB_RECURSE SFCGAL_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h" )
file( GLOB_RECURSE SFCGAL_SOURCES "${CMAKE_SOURCE_DIR}/src/SFCGAL/*.cpp" )

if( ${SFCGAL_USE_STATIC_LIBS} )
  add_definitions( "-fPIC" )
  add_library(
    SFCGAL
    ${SFCGAL_HEADERS}
    ${SFCGAL_SOURCES}
    )

else()
  add_library(
    SFCGAL SHARED
    ${SFCGAL_HEADERS}
    ${SFCGAL_SOURCES}
    )
endif()

if( ${SFCGAL_WITH_GMP} )
	target_link_libraries( SFCGAL ${GMP_LIBRARIES} )
endif( ${SFCGAL_WITH_GMP} )
if( ${SFCGAL_WITH_MPFR} )
	target_link_libraries( SFCGAL ${MPFR_LIBRARIES} )
endif( ${SFCGAL_WITH_MPFR} )
if( ${SFCGAL_WITH_OSG} )
	 target_link_libraries( SFCGAL ${OPENSCENEGRAPH_LIBRARIES} )
endif()

target_link_libraries( SFCGAL ${CGAL_LIBRARIES} )

target_link_libraries( SFCGAL ${Boost_LIBRARIES} )

if ( ${Use_precompiled_headers} )
  if(PCHSupport_FOUND)
    # Add "-fPIC" for shared library build
    if( ${SFCGAL_USE_STATIC_LIBS} )
      set( pch_option "" )
    else()
      set( pch_option "-fPIC" )
    endif()
    add_precompiled_header(SFCGAL
      ${CMAKE_CURRENT_SOURCE_DIR}/../include/CGAL/all.h
      ${pch_option}
      )
  endif()
endif()

# install library
install(
	TARGETS
	SFCGAL
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	BUNDLE DESTINATION lib
)

