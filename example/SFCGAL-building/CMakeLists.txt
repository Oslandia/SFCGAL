GET_FILENAME_COMPONENT( EXAMPLE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME )
add_executable( example-${EXAMPLE_NAME} 
	main.cpp 
)

find_package(Boost REQUIRED COMPONENTS serialization)
target_link_libraries( example-${EXAMPLE_NAME} SFCGAL ${Boost_LIBRARIES} SFCGAL-osg)
set_target_properties( example-${EXAMPLE_NAME} PROPERTIES DEBUG_POSTFIX "d" )
install( TARGETS example-${EXAMPLE_NAME} DESTINATION bin )
