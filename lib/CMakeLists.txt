#-------------------------------------------------------------------------------
#
# Grasshopper build system -- lib root
#
#-------------------------------------------------------------------------------

set(LIBLIST
  grasshopper
  utils
)

foreach(LIB ${LIBLIST})
  add_subdirectory(${LIB})
endforeach()

set(LIBLIST "${LIBLIST}" PARENT_SCOPE)