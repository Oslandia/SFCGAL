
find_path(CGAL_INCLUDE_DIRS CGAL/gmp.h
    HINTS $ENV{CGAL_DIR}/include ${CGAL_DIR}/include ${CGAL_INCLUDE_DIRS}
    PATH_SUFFIXES CGAL
)

if( MSVC10)
	set( CGAL_LIBRARY_POSTFIX "-vc100-mt-${CGAL_VERSION}" )
	set( CGAL_LIBRARY_POSTFIX_DEBUG "-vc100-mt-gd-${CGAL_VERSION}" )
else()
	set( CGAL_LIBRARY_POSTFIX "")
	set( CGAL_LIBRARY_POSTFIX_DEBUG "d")
endif()

#-- incidates if debug version are found
set( CGAL_DEBUG_FOUND ON )


#-- find CGAL library
find_library( CGAL_LIBRARY "CGAL${CGAL_LIBRARY_POSTFIX}" ${CGAL_LIBRARY}
	HINTS ${CGAL_LIBRARY_DIRS} $ENV{CGAL_DIR}/lib
)
if( CGAL_LIBRARY )
	get_filename_component(CGAL_LIBRARY_DIRS ${CGAL_LIBRARY} PATH)
endif()

if(CGAL_FIND_VERSION)
    find_file(version_file CGAL/version.h HINTS $ENV{CGAL_DIR}/include ${CGAL_DIR}/include ${CGAL_INCLUDE_DIRS} PATH_SUFFIXES CGAL)
    file(STRINGS ${version_file} version_str REGEX "# *define +CGAL_VERSION +")
    string( REGEX REPLACE "# *define +CGAL_VERSION +" "" CGAL_VERSION ${version_str})
    if("${CGAL_VERSION}" VERSION_LESS "${CGAL_FIND_VERSION}")
        message(FATAL_ERROR "CGAL " ${CGAL_FIND_VERSION} " is requiered (found " ${CGAL_VERSION} " in ${version_file})" )
    endif()

endif()

find_library( CGAL_LIBRARY_DEBUG "CGAL${CGAL_LIBRARY_POSTFIX_DEBUG}" ${CGAL_LIBRARY_DEBUG}
	HINTS ${CGAL_LIBRARY_DIRS}
)
if( NOT CGAL_LIBRARY_DEBUG )
	set( CGAL_DEBUG_FOUND OFF )
endif()

#-- CGAL components (Core, ImageIO)
foreach( CGAL_COMPONENT ${CGAL_FIND_COMPONENTS} )
	#-- find release
	find_library( CGAL_${CGAL_COMPONENT}_LIBRARY "CGAL_${CGAL_COMPONENT}${CGAL_LIBRARY_POSTFIX}" "${CGAL_${CGAL_COMPONENT}_LIBRARY}"
		HINTS ${CGAL_LIBRARY_DIRS} $ENV{CGAL_DIR}/lib
	)
	#-- find debug
	find_library( CGAL_${CGAL_COMPONENT}_LIBRARY_DEBUG "CGAL_${CGAL_COMPONENT}${CGAL_LIBRARY_POSTFIX_DEBUG}" "${CGAL_${CGAL_COMPONENT}_LIBRARY_DEBUG}"
		HINTS ${CGAL_LIBRARY_DIRS} $ENV{CGAL_DIR}/lib
	)
	
	if( NOT CGAL_${CGAL_COMPONENT}_LIBRARY_DEBUG )
		set( CGAL_DEBUG_FOUND OFF )
	endif()
endforeach()
 

#-- build variable CGAL_LIBRARIES

set( CGAL_LIBRARIES "" )
if( ${CGAL_DEBUG_FOUND} )
	list( APPEND CGAL_LIBRARIES optimized ${CGAL_LIBRARY} debug ${CGAL_LIBRARY_DEBUG} )
	foreach( CGAL_COMPONENT ${CGAL_FIND_COMPONENTS} )
		list( APPEND CGAL_LIBRARIES optimized "${CGAL_${CGAL_COMPONENT}_LIBRARY}" debug "${CGAL_${CGAL_COMPONENT}_LIBRARY_DEBUG}" )
	endforeach()
else()
	list( APPEND CGAL_LIBRARIES ${CGAL_LIBRARY} )
	foreach( CGAL_COMPONENT ${CGAL_FIND_COMPONENTS} )
		list( APPEND CGAL_LIBRARIES ${CGAL_${CGAL_COMPONENT}_LIBRARY} )
	endforeach()
endif()

#-- report/validate
set( CGAL_COMPONENT_LIBRARIES "" )
set( CGAL_COMPONENT_LIBRARIES_DEBUG "" )
foreach( CGAL_COMPONENT ${CGAL_FIND_COMPONENTS} )
	list( APPEND CGAL_COMPONENT_LIBRARIES CGAL_${CGAL_COMPONENT}_LIBRARY )
	list( APPEND CGAL_COMPONENT_LIBRARIES_DEBUG CGAL_${CGAL_COMPONENT}_LIBRARY_DEBUG )
endforeach()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	CGAL DEFAULT_MSG
	CGAL_INCLUDE_DIRS CGAL_LIBRARIES ${CGAL_COMPONENT_LIBRARIES}
)

mark_as_advanced( CGAL_LIBRARY_DIRS CGAL_LIBRARY_DEBUG ${CGAL_COMPONENT_LIBRARIES_DEBUG} )



