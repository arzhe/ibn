#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "c-ares::cares" for configuration ""
set_property(TARGET c-ares::cares APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(c-ares::cares PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libcares.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS c-ares::cares )
list(APPEND _IMPORT_CHECK_FILES_FOR_c-ares::cares "${_IMPORT_PREFIX}/lib/libcares.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
