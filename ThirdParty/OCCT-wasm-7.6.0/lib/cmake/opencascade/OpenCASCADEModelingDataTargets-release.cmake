#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "TKG2d" for configuration "Release"
set_property(TARGET TKG2d APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(TKG2d PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libTKG2d.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS TKG2d )
list(APPEND _IMPORT_CHECK_FILES_FOR_TKG2d "${_IMPORT_PREFIX}/lib/libTKG2d.a" )

# Import target "TKG3d" for configuration "Release"
set_property(TARGET TKG3d APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(TKG3d PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libTKG3d.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS TKG3d )
list(APPEND _IMPORT_CHECK_FILES_FOR_TKG3d "${_IMPORT_PREFIX}/lib/libTKG3d.a" )

# Import target "TKGeomBase" for configuration "Release"
set_property(TARGET TKGeomBase APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(TKGeomBase PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libTKGeomBase.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS TKGeomBase )
list(APPEND _IMPORT_CHECK_FILES_FOR_TKGeomBase "${_IMPORT_PREFIX}/lib/libTKGeomBase.a" )

# Import target "TKBRep" for configuration "Release"
set_property(TARGET TKBRep APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(TKBRep PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libTKBRep.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS TKBRep )
list(APPEND _IMPORT_CHECK_FILES_FOR_TKBRep "${_IMPORT_PREFIX}/lib/libTKBRep.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
