file( GLOB_RECURSE SFCGAL_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h" )
file( GLOB_RECURSE SFCGAL_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" )


if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions( "-frounding-math" )
endif(CMAKE_COMPILER_IS_GNUCXX)

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

# install library
install(
	TARGETS
	SFCGAL
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	BUNDLE DESTINATION lib
)




