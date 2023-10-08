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
  add_subdirectory(${TOOL})
endforeach()

set(TOOLSLIST "${TOOLSLIST}" PARENT_SCOPE)