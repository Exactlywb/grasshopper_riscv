#-------------------------------------------------------------------------------
#
# Grasshopper build system -- grasshopper library
#
#-------------------------------------------------------------------------------

set(SRCS
  grasshopper.c
  tables.c
)

add_library(grasshopper STATIC ${SRCS})
# TODO: reorganize clang-format
# add_clang_format_run(utils ${CMAKE_CURRENT_SOURCE_DIR} ${SRCS})
