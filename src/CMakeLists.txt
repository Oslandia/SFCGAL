file( GLOB_RECURSE SFCGAL_SOURCES "*.cpp" )
file( GLOB_RECURSE SFCGAL_HEADERS "*.h" )

file( GLOB_RECURSE SFCGAL_HEADERS_COPIED RELATIVE ${CMAKE_SOURCE_DIR}/src "*.h" )
add_custom_target(copy)
foreach (header ${SFCGAL_HEADERS_COPIED})
    add_custom_command(TARGET copy
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/${header} ${CMAKE_BINARY_DIR}/include/SFCGAL/${header} DEPENDS ${CMAKE_SOURCE_DIR}/src/${header})
endforeach()

if (CGAL_VERSION VERSION_GREATER 4.13)
  include_directories(../CGAL_patches)
endif()

add_definitions( "-fPIC" )
add_library(
SFCGAL_static
${SFCGAL_HEADERS}
${SFCGAL_SOURCES}
)

add_library(
SFCGAL SHARED
${SFCGAL_HEADERS}
${SFCGAL_SOURCES}
)

add_dependencies( SFCGAL copy )
add_dependencies( SFCGAL_static copy )

# set VERSION and SOVERSION
set_target_properties( SFCGAL PROPERTIES VERSION ${SFCGAL_VERSION}
                                         SOVERSION ${SFCGAL_VERSION_MAJOR} )

target_link_libraries( SFCGAL CGAL::CGAL CGAL::CGAL_Core)

target_link_libraries( SFCGAL_static CGAL::CGAL CGAL::CGAL_Core)

target_link_libraries( SFCGAL ${Boost_LIBRARIES} )

target_link_libraries( SFCGAL_static ${Boost_LIBRARIES} )

if ( ${Use_precompiled_headers} )
  if(PCHSupport_FOUND)
    # Add "-fPIC" for shared library build
    if( ${SFCGAL_USE_STATIC_LIBS} )
      set( pch_option "" )
    else()
      set( pch_option "-fPIC" )
    endif()
    add_precompiled_header(SFCGAL
      include/CGAL/all.h
      ${pch_option}
      )
  endif()
endif()

# install library
install(
	TARGETS
	SFCGAL
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	BUNDLE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

