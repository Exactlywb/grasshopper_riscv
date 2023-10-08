#-------------------------------------------------------------------------------
#
# Grasshopper build system -- utils library
#
#-------------------------------------------------------------------------------

set(SRCS
  alloc.c
  file_common.c
)

add_library(utils STATIC ${SRCS})
add_clang_format_run(utils ${CMAKE_CURRENT_SOURCE_DIR} ${SRCS})