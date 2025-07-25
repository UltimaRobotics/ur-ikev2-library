#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libopenikev2::libopenikev2" for configuration "Release"
set_property(TARGET libopenikev2::libopenikev2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libopenikev2::libopenikev2 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libopenikev2.a"
  )

list(APPEND _cmake_import_check_targets libopenikev2::libopenikev2 )
list(APPEND _cmake_import_check_files_for_libopenikev2::libopenikev2 "${_IMPORT_PREFIX}/lib/libopenikev2.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
