#-- MPFR finder
#
# search variables :
#     MPFR_DIR(cmake/environment) : gmp install directory
#     MPFR_INCLUDE_DIR(cmake) : directory containing gmp.h      
#     MPFR_LIBRARY_DIR(cmake) : directory containing gmp libraries
#
# defined variables :
#     MPFR_FOUND : true if library found
#     MPFR_INCLUDE_DIRS : full path to gmp libraries
#     MPFR_LIBRARIES : full path to gmp libraries
#


find_path(MPFR_INCLUDE_DIRS mpfr.h
    HINTS $ENV{MPFR_DIR}/include ${MPFR_DIR}/include
    PATH_SUFFIXES mpfr
)

find_library(MPFR_LIBRARY NAMES mpfr
	HINTS $ENV{MPFR_DIR}/lib ${MPFR_DIR}/lib
)
set( MPFR_LIBRARIES ${MPFR_LIBRARY} )


# handle the QUIETLY and REQUIRED arguments and set <NAME>_FOUND to TRUE
# if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	MPFR DEFAULT_MSG
    MPFR_INCLUDE_DIRS MPFR_LIBRARIES
)

mark_as_advanced(MPFR_INCLUDE_DIRS MPFR_LIBRARIES )


