#-------------------------------------------------------------------------------
#
# Grasshopper build system -- tools root
#
#-------------------------------------------------------------------------------

set(TOOLSLIST
  simple
  vectorized
)

foreach(TOOL ${TOOLSLIST})
  add_executable(${TOOL} main.c ${TOOL}/${TOOL}.c)
endforeach()

set(TOOLSLIST "${TOOLSLIST}" PARENT_SCOPE)