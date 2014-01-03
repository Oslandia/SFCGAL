# - Find PostgreSQL
# Find the PostgreSQL includes and client library
# This module defines
#  POSTGRESQL_INCLUDE_DIR, where to find POSTGRESQL.h
#  POSTGRESQL_LIBRARIES, the libraries needed to use POSTGRESQL.
#  POSTGRESQL_FOUND, If false, do not try to use PostgreSQL.
#
# Copyright (c) 2006, Jaroslaw Staniek, <js@iidea.pl>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# Add the postgresql and mysql include paths here

if(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)

  set(POSTGRESQL_FOUND TRUE)
  message(STATUS "Enabled PostgreSQL: ${POSTGRESQL_INCLUDE_DIR}, ${POSTGRESQL_LIBRARIES}")

else(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)

  FIND_PATH(POSTGRESQL_INCLUDE_DIR libpq-fe.h
    /usr/include/postgresql*/
    /usr/pgsql*/include
    /usr/local/pgsql/include
    /usr/local/include
    /opt/local/include
    /usr/include/pgsql
    $ENV{ProgramFiles}/PostgreSQL/*/include
    $ENV{SystemDrive}/PostgreSQL/*/include
    )

  FIND_LIBRARY(POSTGRESQL_LIBRARIES NAMES pq libpq
  PATHS
    /usr/lib
    /usr/lib/postgresql
    /usr/local/lib
    /usr/pgsql*/lib
    /usr/lib64
    /usr/lib64/postgresql
    /usr/local/lib64
    /usr/pgsql*/lib64
    $ENV{ProgramFiles}/PostgreSQL/*/lib/ms
    $ENV{SystemDrive}/PostgreSQL/*/lib/ms
  )

  if(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
    set(POSTGRESQL_FOUND TRUE)
    message(STATUS "Found PostgreSQL: ${POSTGRESQL_INCLUDE_DIR}, ${POSTGRESQL_LIBRARIES}")
  else(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
    set(POSTGRESQL_FOUND FALSE)
    message(STATUS "PostgreSQL not found.")
  endif(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)

endif(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
