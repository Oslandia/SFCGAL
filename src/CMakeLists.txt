file( GLOB_RECURSE SFCGAL_SOURCES "*.cpp" )
file( GLOB_RECURSE SFCGAL_HEADERS "*.h" )

file( GLOB_RECURSE SFCGAL_HEADERS_COPIED RELATIVE ${CMAKE_SOURCE_DIR}/src "*.h" )
add_custom_target(copy)
foreach (header ${SFCGAL_HEADERS_COPIED})
    add_custom_command(TARGET copy
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/${header} ${CMAKE_BINARY_DIR}/include/SFCGAL/${header} DEPENDS ${CMAKE_SOURCE_DIR}/src/${header})
endforeach()

# helper script to find the list of used includes
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/find_includes.sh.in ${CMAKE_CURRENT_BINARY_DIR}/find_includes.sh @ONLY )

set( SFCGAL_OSG_HEADERS
  ${CMAKE_SOURCE_DIR}/src/io/osg.h
  ${CMAKE_SOURCE_DIR}/src/detail/io/OsgFactory.h )
set( SFCGAL_OSG_SOURCES
  ${CMAKE_SOURCE_DIR}/src/io/osg.cpp
  ${CMAKE_SOURCE_DIR}/src/detail/io/OsgFactory.cpp )
message( STATUS "removing OSG dependencies from the library")
list(REMOVE_ITEM SFCGAL_HEADERS ${SFCGAL_OSG_HEADERS})
list(REMOVE_ITEM SFCGAL_SOURCES ${SFCGAL_OSG_SOURCES})

# common target, used by both the shared and the static library
add_library( objlib OBJECT ${SFCGAL_HEADERS} ${SFCGAL_SOURCES} )
add_dependencies( objlib copy )
set_target_properties( objlib PROPERTIES POSITION_INDEPENDENT_CODE 1)
# by default, all symbols are hidden
set_target_properties( objlib PROPERTIES CMAKE_CXX_VISIBILITY_PRESET hidden
                                         CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# the public, shared library
add_library(SFCGAL SHARED $<TARGET_OBJECTS:objlib>)
# set VERSION and SOVERSION
set_target_properties( SFCGAL PROPERTIES VERSION ${SFCGAL_VERSION}
                                         SOVERSION ${SFCGAL_VERSION_MAJOR} )
add_dependencies( SFCGAL copy )

# the static variant
add_library(SFCGAL_static STATIC $<TARGET_OBJECTS:objlib>)

target_link_libraries( SFCGAL ${Boost_LIBRARIES} )
target_link_libraries( SFCGAL gmp mpfr )

target_link_libraries( SFCGAL_static ${Boost_LIBRARIES} )
target_link_libraries( SFCGAL_static gmp mpfr )

# install library
install(
	TARGETS
	SFCGAL
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	BUNDLE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# SFCGAL-osg
if ( SFCGAL_WITH_OSG )
  add_library( osg-objlib OBJECT ${SFCGAL_OSG_HEADERS} ${SFCGAL_OSG_SOURCES} )
  set_target_properties( osg-objlib PROPERTIES POSITION_INDEPENDENT_CODE 1)

  add_library(SFCGAL-osg SHARED $<TARGET_OBJECTS:osg-objlib>)
  set_target_properties( SFCGAL-osg PROPERTIES VERSION ${SFCGAL_VERSION}
                                         SOVERSION ${SFCGAL_VERSION_MAJOR} )

  add_library(SFCGAL-osg_static STATIC $<TARGET_OBJECTS:osg-objlib>)
  
  target_link_libraries( SFCGAL-osg
    SFCGAL
    ${OPENSCENEGRAPH_LIBRARIES}
  )
  target_link_libraries( SFCGAL-osg_static
    SFCGAL
    ${OPENSCENEGRAPH_LIBRARIES}
  )
  install(
	TARGETS
	SFCGAL-osg
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	BUNDLE DESTINATION ${CMAKE_INSTALL_LIBDIR}
   )
endif()
