# CMakeList.txt : CMake project for Explore-Module, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)

function(enable_module target)
  if (MSVC)
    set(BMI ${CMAKE_CURRENT_BINARY_DIR}/${target}.ifc)
    target_compile_options(${target}
      PRIVATE /interface /ifcOutput ${BMI}
      INTERFACE /reference ${target}=${BMI})
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(BMI ${CMAKE_CURRENT_BINARY_DIR}/${target}.cmi)
    target_compile_options(${target} PUBLIC -fmodules-ts)
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(BMI ${CMAKE_CURRENT_BINARY_DIR}/${target}.cppm)
    target_compile_options(${target} PUBLIC -fmodules-ts)
  endif ()
  set_target_properties(${target} PROPERTIES ADDITIONAL_CLEAN_FILES ${BMI})
endfunction()

include(CMakeParseArguments)

project(temploid-name-lookup CXX)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Add source to this project's executable.
add_library (mod OBJECT mod.cpp)
enable_module(mod)
add_executable (temploid-name-lookup main.cpp)
target_link_libraries(temploid-name-lookup mod)
set_source_files_properties(main.cpp PROPERTIES OBJECT_DEPENDS mod)

# TODO: Add tests and install targets if needed.
