cmake_minimum_required(VERSION 3.2)

project(occt-webgl-sample)

set(CMAKE_CXX_STANDARD 11)
set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)
set(APP_TARGET occt-webgl-sample)

# option to enable or disable use of precompiled headers
if (NOT DEFINED SOURCE_MAP_BASE)
  set (SOURCE_MAP_BASE "" CACHE STRING "Path to source map server for debugging C++ code; e.g. http://localhost:9090/")
endif()

# customize build
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --bind")
if (NOT "${SOURCE_MAP_BASE}" STREQUAL "")
  set(CMAKE_CXX_FLAGS_DEBUG "-g4 --source-map-base ${SOURCE_MAP_BASE}")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s WASM=1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s MAX_WEBGL_VERSION=2")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s MODULARIZE=1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s EXPORT_NAME='createOccViewerModule'")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_FREETYPE=1")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s TOTAL_MEMORY=1024MB")

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASSERTIONS=1")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s SAFE_HEAP=1")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s BINARYEN_ASYNC_COMPILATION=0")

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file myFile")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --extern-post-js ${CMAKE_CURRENT_SOURCE_DIR}/occt-webgl-viewer.js")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
file(GLOB SOURCES
  *.h
  *.cpp
)
source_group ("Headers" FILES
  WasmOcctView.hpp
  GeometryManager.hpp
  AppManager.hpp
  Geometry.hpp
  LCRSNode.hpp
  LCRSTree.hpp)
source_group ("Sources" FILES
  WasmOcctView.cpp
  GeometryManager.cpp
  AppManager.cpp
  Geometry.cpp
  main.cpp)

# FreeType
#find_package(freetype REQUIRED NO_DEFAULT_PATH)
#if(freetype_FOUND)
#  message (STATUS "Using FreeType from \"${freetype_DIR}\"" )
#else()
#  message(WARNING "Could not find FreeType, please set freetype_DIR variable." )
#endif()

#set(OpenCASCADE_INCLUDE_DIR "D:/Library/OCCT-wasm-7.6.0/inc" CACHE STRING "")
#set(OpenCASCADE_LIBRARY_DIR "D:/Library/OCCT-wasm-7.6.0/lib" CACHE STRING "")
# Open CASCADE Technology
#find_package(OpenCASCADE REQUIRED NO_DEFAULT_PATH)
#if(OpenCASCADE_FOUND)
#  message (STATUS "Using OpenCASCADE from \"${OpenCASCADE_DIR}\"" )
  INCLUDE_DIRECTORIES(${OpenCASCADE_INCLUDE_DIR})
  LINK_DIRECTORIES(${OpenCASCADE_LIBRARY_DIR})
#else()
#  message(WARNING "Could not find OpenCASCADE, please set OpenCASCADE_DIR variable." )
#  set(OCCT_LIBRARY_DIR)
#  set(OCCT_BIN_DIR)
#endif()

set(OpenCASCADE_LIBS 
    TKBin TKBinL TKBinTObj TKBinXCAF TKBO TKBool TKBRep TKCAF TKCDF TKernel
    TKFeat TKFillet TKG2d TKG3d TKGeomAlgo TKGeomBase TKHLR TKIGES TKLCAF TKMath
    TKMesh TKMeshVS TKOffset TKOpenGles TKPrim TKRWMesh TKService TKShHealing TKStd TKStdL
    TKSTEP TKSTEP209 TKSTEPAttr TKSTEPBase TKSTL TKTObj TKTopAlgo TKV3d TKVCAF TKVRML
    TKXCAF TKXDEIGES TKXDESTEP TKXMesh TKXml TKXmlL TKXmlTObj TKXmlXCAF TKXSBase)


add_executable(${APP_TARGET} ${SOURCES})
target_link_libraries(
  ${APP_TARGET}
  ${OpenCASCADE_LIBS}
  freetype
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}" LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.wasm DESTINATION ${CMAKE_INSTALL_PREFIX})
if (NOT "${SOURCE_MAP_BASE}" STREQUAL "")
  if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.wasm.map DESTINATION ${CMAKE_INSTALL_PREFIX})
  endif()
endif()
install(FILES occt-webgl-sample.html DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${OpenCASCADE_RESOURCE_DIR}/DrawResources/OCC_logo.png  DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${OpenCASCADE_RESOURCE_DIR}/DrawResources/lamp.ico      DESTINATION ${CMAKE_INSTALL_PREFIX})
if (CMAKE_CXX_FLAGS MATCHES "-pthread")
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.worker.js DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
