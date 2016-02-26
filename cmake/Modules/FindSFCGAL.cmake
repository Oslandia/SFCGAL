#-- SFCGAL finder
#
# search variables :
#     SFCGAL_DIR(cmake/environment) : SFCGAL install directory
#     SFCGAL_INCLUDE_DIR(cmake) : directory containing SFCGAL.h      
#     SFCGAL_LIBRARY_DIR(cmake) : directory containing SFCGAL libraries
#
# defined variables :
#     SFCGAL_FOUND : true if library found
#     SFCGAL_INCLUDE_DIRS : full path to SFCGAL libraries
#     SFCGAL_LIBRARIES : full path to SFCGAL libraries
#

# -- parse version
find_path(SFCGAL_INCLUDE_DIR SFCGAL/Kernel.h
    HINTS $ENV{SFCGAL_DIR} ${SFCGAL_DIR} ${SFCGAL_INCLUDE_DIRS}
    PATH_SUFFIXES include
)

# -- parse version
IF(SFCGAL_INCLUDE_DIR)
	IF(EXISTS "${SFCGAL_INCLUDE_DIR}/SFCGAL/version.h")
		FILE(STRINGS "${SFCGAL_INCLUDE_DIR}/SFCGAL/version.h" sfcgal_version_str REGEX "^#define[\t ]+SFCGAL_VERSION[\t ]+\".*\"")

		STRING(REGEX REPLACE "^#define[\t ]+SFCGAL_VERSION[\t ]+\"([^\"]*)\".*" "\\1" SFCGAL_VERSION "${sfcgal_version_str}")
		
		# from http://www.cmake.org/Wiki/CMakeCompareVersionStrings 
		# Breaks up a string in the form n1.n2.n3 into three parts and stores
		# them in major, minor, and patch.  version should be a value, not a
		# variable, while major, minor and patch should be variables.
		IF(${SFCGAL_VERSION} MATCHES "[0-9]+\\.[0-9]+\\.[0-9]+")
			STRING(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" SFCGAL_MAJOR_VERSION "${SFCGAL_VERSION}")
			STRING(REGEX REPLACE "^[0-9]+\\.([0-9])+\\.[0-9]+" "\\1" SFCGAL_MINOR_VERSION "${SFCGAL_VERSION}")
			STRING(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" SFCGAL_PATCH_VERSION "${SFCGAL_VERSION}")
		ELSE()
			MESSAGE( WARNING "SFCGAL_VERSION (${SFCGAL_VERSION}) doesn't match *.*.* form" )
		ENDIF()		


		UNSET(sfcgal_version_str)
	ELSE()
		message( WARNING "can't parse SFCGAL version" )
	ENDIF()
ENDIF()



# find library (Release)
find_library(SFCGAL_LIBRARY NAMES SFCGAL
	HINTS $ENV{SFCGAL_DIR} ${SFCGAL_DIR}
	PATH_SUFFIXES lib
)
# find library (Debug)
find_library(SFCGAL_LIBRARY_DEBUG NAMES SFCGALd
	HINTS $ENV{SFCGAL_DIR} ${SFCGAL_DIR}
	PATH_SUFFIXES lib
)

# set output variables
set( SFCGAL_INCLUDE_DIRS "${SFCGAL_INCLUDE_DIR}" )
if ( SFCGAL_LIBRARY_DEBUG )
	set( SFCGAL_LIBRARIES "optimized;${SFCGAL_LIBRARY};debug;${SFCGAL_LIBRARY_DEBUG}" )
else()
	set( SFCGAL_LIBRARIES ${SFCGAL_LIBRARY} )
endif()

# handle the QUIETLY and REQUIRED arguments and set <NAME>_FOUND to TRUE
# if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	SFCGAL DEFAULT_MSG
    SFCGAL_INCLUDE_DIRS SFCGAL_LIBRARIES
)

mark_as_advanced(SFCGAL_INCLUDE_DIRS SFCGAL_LIBRARIES)


