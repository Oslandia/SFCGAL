file( GLOB_RECURSE SFCGAL_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h" )
file( GLOB_RECURSE SFCGAL_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" )


if( ${SFCGAL_USE_STATIC_LIBS} )
	add_library(
		SFCGAL
		${SFCGAL_HEADERS}
		${SFCGAL_SOURCES}
	)
else()
	#todo add links	
	add_library(
		SFCGAL SHARED
		${SFCGAL_HEADERS}
		${SFCGAL_SOURCES}
	)
endif()

if( ${SFCGAL_WITH_GMP} )
  target_link_libraries( SFCGAL ${GMP_LIBRARIES} )
endif( ${SFCGAL_WITH_GMP} )

include( PrecompiledHeader )
if(PCHSupport_FOUND)
  add_precompiled_header(SFCGAL
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/CGAL/all.h
#    "-fPIC"
    )
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

