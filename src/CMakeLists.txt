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

#add_definitions( "-fPIC" )

add_library(objlib OBJECT ${SFCGAL_HEADERS} ${SFCGAL_SOURCES})
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library( SFCGAL_static $<TARGET_OBJECTS:objlib> )

add_library( SFCGAL SHARED $<TARGET_OBJECTS:objlib> )

add_dependencies( SFCGAL copy )
add_dependencies( SFCGAL_static copy )

# set VERSION and SOVERSION
set_target_properties( SFCGAL PROPERTIES VERSION ${SFCGAL_VERSION}
                                         SOVERSION ${SFCGAL_VERSION_MAJOR} )

target_link_libraries( SFCGAL CGAL::CGAL CGAL::CGAL_Core)

target_link_libraries( SFCGAL_static CGAL::CGAL CGAL::CGAL_Core)

target_link_libraries( SFCGAL ${Boost_LIBRARIES} )

target_link_libraries( SFCGAL_static ${Boost_LIBRARIES} )

# install library
install(
	TARGETS
	SFCGAL
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	BUNDLE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

