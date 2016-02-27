file(GLOB_RECURSE EXAMPLE_SOURCES "${CMAKE_SOURCE_DIR}/example/*.cpp")
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
endforeach()
