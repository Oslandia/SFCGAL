#
# Original message : http://www.cmake.org/pipermail/cmake/2006-December/012360.html

# - Try to find precompiled headers support for GCC 3.4 and 4.x
# Once done this will define:
#
# Variable:
#   PCHSupport_FOUND
#
# Macro:
#   ADD_PRECOMPILED_HEADER

if (CMAKE_CXX_COMPILER MATCHES ".*clang.*")
    set(CMAKE_COMPILER_IS_CLANGXX TRUE)
endif ()

IF(CMAKE_COMPILER_IS_GNUCXX)
  EXEC_PROGRAM(
    ${CMAKE_CXX_COMPILER} 
    ARGS 			--version 
    OUTPUT_VARIABLE _compiler_output)
  STRING(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1" 
    gcc_compiler_version ${_compiler_output})
  #MESSAGE("GCC Version: ${gcc_compiler_version}")
  IF(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
    SET(PCHSupport_FOUND TRUE)
  ELSE(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")
    IF(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
      SET(PCHSupport_FOUND TRUE)
    ENDIF(gcc_compiler_version MATCHES "3\\.4\\.[0-9]")
  ENDIF(gcc_compiler_version MATCHES "4\\.[0-9]\\.[0-9]")

  IF (PCHSupport_FOUND)
    SET( PCH_Generate_cmd -x c++-header)
    SET( PCH_Include_cmd "-Winvalid-pch -include" )
    SET( PCH_Include_file _name )
  ENDIF(PCHSupport_FOUND)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(CMAKE_COMPILER_IS_CLANGXX)
  set(PCHSupport_FOUND TRUE)
  SET( PCH_Generate_cmd "")
  SET( PCH_Include_cmd "-include-pch" )
  SET( PCH_Include_file _output )
ENDIF(CMAKE_COMPILER_IS_CLANGXX)

MACRO(ADD_PRECOMPILED_HEADER _targetName _input )

  IF(NOT CMAKE_BUILD_TYPE)
    MESSAGE(FATAL_ERROR 
      "This is the ADD_PRECOMPILED_HEADER macro. " 
      "You must set CMAKE_BUILD_TYPE!"
      )
  ENDIF(NOT CMAKE_BUILD_TYPE)
  
  GET_FILENAME_COMPONENT(_name ${_input} NAME)
  GET_FILENAME_COMPONENT(_path ${_input} PATH)
  SET(_outdir "${CMAKE_CURRENT_BINARY_DIR}/${_name}.gch")
  SET(_output "${_outdir}/${CMAKE_BUILD_TYPE}.pch")
  
  ADD_CUSTOM_COMMAND(
    OUTPUT ${_outdir}
    COMMAND mkdir ${_outdir} # TODO: {CMAKE_COMMAND} -E ... 
    )
  #MAKE_DIRECTORY(${_outdir})
  
  STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
  SET(_compile_FLAGS ${${_flags_var_name}})
  
  GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
  
  
  SET(_CMAKE_CURRENT_BINARY_DIR_included_before_path FALSE)
  FOREACH(item ${_directory_flags})
    IF(${item}  STREQUAL ${_path} AND NOT _CMAKE_CURRENT_BINARY_DIR_included_before_path )
      MESSAGE(FATAL_ERROR 
	"This is the ADD_PRECOMPILED_HEADER macro. "
	"CMAKE_CURREN_BINARY_DIR has to mentioned at INCLUDE_DIRECTORIES's argument list before ${_path}, where ${_name} is located"
	)	
    ENDIF(${item}  STREQUAL ${_path} AND NOT _CMAKE_CURRENT_BINARY_DIR_included_before_path )
    
    IF(${item}  STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
      SET(_CMAKE_CURRENT_BINARY_DIR_included_before_path TRUE)
    ENDIF(${item}  STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    
    LIST(APPEND _compile_FLAGS "-I${item}")
  ENDFOREACH(item)
  
  GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
  LIST(APPEND _compile_FLAGS ${_directory_flags})
  LIST(APPEND _compile_FLAGS ${CMAKE_CXX_FLAGS} )
  # get additional flags from the macro
  LIST(APPEND _compile_FLAGS ${ARGN})
  
  SEPARATE_ARGUMENTS(_compile_FLAGS)
#  MESSAGE("_compiler_FLAGS: ${_compiler_FLAGS}")
#  message(${_compile_FLAGS} )
#  message("COMMAND ${CMAKE_CXX_COMPILER}	${_compile_FLAGS} -x c++-header -o ${_output} ${_input}")
  
  ADD_CUSTOM_COMMAND(
    OUTPUT	${CMAKE_CURRENT_BINARY_DIR}/${_name} 
    COMMAND ${CMAKE_COMMAND} -E copy  ${_input} ${CMAKE_CURRENT_BINARY_DIR}/${_name} # ensure same directory! Required by gcc
    )
  
  ADD_CUSTOM_COMMAND(
    OUTPUT ${_output} 	
    COMMAND ${CMAKE_CXX_COMPILER}
    ${_compile_FLAGS}
    ${PCH_Generate_cmd}
    -o ${_output} 
    ${_input}
    DEPENDS ${_input} ${_outdir} ${CMAKE_CURRENT_BINARY_DIR}/${_name}	
    )
  ADD_CUSTOM_TARGET(${_targetName}_gch 
    DEPENDS	${_output} 	
    )
  ADD_DEPENDENCIES(${_targetName} ${_targetName}_gch )
  SET_TARGET_PROPERTIES(${_targetName} 
    PROPERTIES	
#    COMPILE_FLAGS "${PCH_Include_cmd} ${_name}"
#    COMPILE_FLAGS "${PCH_Include_cmd} ${_output}"
    COMPILE_FLAGS "${PCH_Include_cmd} ${${PCH_Include_file}}"
    )
  
ENDMACRO(ADD_PRECOMPILED_HEADER)