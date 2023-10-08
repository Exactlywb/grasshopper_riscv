#-------------------------------------------------------------------------------
#
# Grasshopper build system -- main entry
#
#-------------------------------------------------------------------------------
#
# Structure of build is:
# root
#    /lib        -- for libraries with subsequent cmakes
#    /include    -- for includes
#    /cmake      -- for cmake modules
#    /tools      -- for tools and executables (grasshopper itself is a tool)
#
# You shall use CMAKE_BUILD_TYPE = DEBUG | RELEASE when building
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14)

project(Grasshopper)

set(CMAKE_CXX_STANDARD 20)

# clang-format run
include(cmake/clang-format.cmake)

# collecting libs
add_subdirectory(lib)
message(STATUS "Collected libs: ${LIBLIST}")

# set output to bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# common includes
foreach(LIB ${LIBLIST})
  target_include_directories(${LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIB})
  target_include_directories(${LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endforeach()

# collecting tools
add_subdirectory(tools)
message(STATUS "Collected tools: ${TOOLLIST}")

# link all libraries to every tool -- just in case
foreach(TOOL ${TOOLSLIST})
  target_include_directories(${TOOL} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(${TOOL} ${LIBLIST})
  message(STATUS ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
