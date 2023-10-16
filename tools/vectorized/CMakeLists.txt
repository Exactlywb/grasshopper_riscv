#-------------------------------------------------------------------------------
#
# Grasshopper build system -- vectorized grasshopper impl
#
#-------------------------------------------------------------------------------

set(SRCS
  main.c
)

add_executable(vectorized ${SRCS})
# TODO: reorganize clang-format
# add_clang_format_run(vectorized ${CMAKE_CURRENT_SOURCE_DIR} ${SRCS})