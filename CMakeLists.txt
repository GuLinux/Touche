project(Touche)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(QJSON REQUIRED)
find_package(X11 REQUIRED)
find_package( KDE4 REQUIRED )
include(KDE4Defaults)


set(Touche_static_libraries
  ${CMAKE_BINARY_DIR}/lib/libTouche_Core.a
  ${CMAKE_BINARY_DIR}/lib/libTouche_Configuration.a
)

set(Touche_common_includes ${QT_INCLUDES}
  ${CMAKE_SOURCE_DIR}/Core ${CMAKE_BINARY_DIR}/Touche_Core
  ${CMAKE_SOURCE_DIR}/Settings ${CMAKE_BINARY_DIR}/Settings)

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
include_directories( ${KDE4_INCLUDES} )

set(Touche_libs ${QT_LIBRARIES} ${Touche_static_libraries} ${X11_LIBRARIES} ${QJSON_LIBRARIES} ${X11_XTest_LIB} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_subdirectory(Core)
add_subdirectory(Settings)

add_subdirectory(kde)
