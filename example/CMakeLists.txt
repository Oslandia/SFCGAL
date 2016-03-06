#-------------------------------------------------------------
# find OpenSceneGraph (optional)
# note : SFCGAL_WITH_OSG is defined when OSG 
# is found and examples should take it in account.
#-------------------------------------------------------------
find_package(OpenSceneGraph 3.2.0 COMPONENTS osgViewer osgGA osgDB osgUtil)
if(OPENSCENEGRAPH_FOUND)
    add_definitions("-DSFCGAL_WITH_OSG")
    include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
endif()

#-------------------------------------------------------------
#
#-------------------------------------------------------------
file(GLOB EXAMPLE_SOURCES "${CMAKE_SOURCE_DIR}/example/*.cpp")
foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME}
        ${EXAMPLE_SOURCE}
    )
    target_link_libraries(${EXAMPLE_NAME}
        SFCGAL
        ${CGAL_LIBRARIES}
        ${Boost_LIBRARIES}
        ${MPFR_LIBRARIES}
        ${GMP_LIBRARIES}
    )
    if(OPENSCENEGRAPH_FOUND)
        target_link_libraries( ${EXAMPLE_NAME}
            ${OPENSCENEGRAPH_LIBRARIES} 
        )
    endif()
endforeach()
