#-------------------------------------------------------------------------------
#
# Grasshopper build system -- simple grasshopper impl
#
#-------------------------------------------------------------------------------

set(SRCS
  grasshopper.c
  main.c
)

add_executable(simple ${SRCS})
add_clang_format_run(simple ${CMAKE_CURRENT_SOURCE_DIR} ${SRCS})