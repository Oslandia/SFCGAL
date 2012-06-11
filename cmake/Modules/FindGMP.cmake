#-- GMP finder
#
# search variables :
#     GMP_DIR(cmake/environment) : gmp install directory
#     GMP_INCLUDE_DIR(cmake) : directory containing gmp.h      
#     GMP_LIBRARY_DIR(cmake) : directory containing gmp libraries
#
# find options :
#     GMP_ENABLE_CXX : search for gmpxx library?
#
# defined variables :
#     GMP_FOUND : true if library found
#     GMP_INCLUDE_DIRS : full path to gmp libraries
#     GMP_LIBRARIES : full path to gmp libraries
#

option( GMP_ENABLE_CXX "enable cxx library" OFF )

find_path(GMP_INCLUDE_DIRS gmp.h
    HINTS $ENV{GMP_DIR}/include ${GMP_DIR}/include
    PATH_SUFFIXES gmp
)

find_library(GMP_LIBRARY NAMES gmp gmp-10 
	HINTS $ENV{GMP_DIR}/lib ${GMP_DIR}/lib
)
set( GMP_LIBRARIES ${GMP_LIBRARY} )

if( ${GMP_ENABLE_CXX} )
	find_library(GMP_CXX_LIBRARY NAMES gmpxx gmpxx-4
		HINTS $ENV{GMP_DIR}/lib ${GMP_DIR}/lib 
	)
	set( GMP_LIBRARIES "${GMP_CXX_LIBRARY};${GMP_LIBRARIES}" )
endif()

# handle the QUIETLY and REQUIRED arguments and set <NAME>_FOUND to TRUE
# if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	GMP DEFAULT_MSG
    GMP_INCLUDE_DIRS GMP_LIBRARIES
)

mark_as_advanced(GMP_INCLUDE_DIRS GMP_LIBRARIES)


